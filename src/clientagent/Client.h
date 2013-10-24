#pragma once
#include "util/NetworkClient.h"
#include "messagedirector/MessageDirector.h"

#include <queue>
#include <unordered_set>
#include <unordered_map>

class ClientAgent; // Forward declaration

enum ClientState
{
	CLIENT_STATE_NEW,
	CLIENT_STATE_ANONYMOUS,
	CLIENT_STATE_ESTABLISHED
};

struct VisibleObject
{
	uint32_t id;
	uint32_t parent;
	uint32_t zone;
	DCClass *dcc;
};

struct Interest
{
	uint16_t id;
	uint32_t parent;
	std::unordered_set<uint32_t> zones;
};

class InterestOperation
{
	public:
		uint16_t m_interest_id;
		uint32_t m_client_context;
		uint32_t m_parent;
		std::unordered_set<uint32_t> m_zones;

		bool m_has_total;
		uint32_t m_total;

		InterestOperation(uint16_t interest_id, uint32_t client_context,
		                  uint32_t parent, std::unordered_set<uint32_t> zones);

		bool is_ready(const std::unordered_map<uint32_t, VisibleObject> &dist_objs);
		void store_total(uint32_t total);
};

class ChannelTracker
{
	public:
		ChannelTracker(channel_t min, channel_t max);

		channel_t alloc_channel();
		void free_channel(channel_t channel);

	private:
		channel_t m_next;
		channel_t m_max;
		std::queue<channel_t> m_unused_channels;
};

class Client : public MDParticipantInterface
{
	public:
		~Client();

		// handle_datagram is the handler for datagrams received from the server
		void handle_datagram(Datagram &dg, DatagramIterator &dgi);
	protected:
		ClientAgent* m_client_agent; // The client_agent handling this client
		ClientState m_state; // current state of the Client state machine
		channel_t m_channel; // current channel client is listening on
		channel_t m_allocated_channel; // channel assigned to client at creation time
		uint32_t m_next_context;
		std::unordered_set<uint32_t> m_owned_objects;
		std::unordered_set<uint32_t> m_seen_objects;
		std::unordered_map<uint16_t, Interest> m_interests;
		std::unordered_map<uint32_t, InterestOperation*> m_pending_interests;
		std::unordered_map<uint32_t, VisibleObject> m_dist_objs;
		LogCategory *m_log;

		Client(ClientAgent* client_agent);

		// log_event sends an event to the EventLogger
		void log_event(const std::list<std::string> &event);

		// lookup_object returns the class of the object with a do_id.
		// If that object is not visible to the client, NULL will be returned instead.
		DCClass* lookup_object(uint32_t do_id);

		// lookup_interests returns a list of all the interests that a parent-zone pair is visible to.
		std::list<Interest> lookup_interests(uint32_t parent_id, uint32_t zone_id);

		// add_interest will start a new interest operation and retrieve all the objects an interest
		// from the server, subscribing to each zone in the interest.  If the interest already
		// exists, the interest will be updated with the new zones passed in by the argument.
		void add_interest(Interest &i, uint32_t context);

		// remove_interest find each zone an interest which is not part of another interest and
		// passes it to close_zones() to be removed from the client's visibility.
		void remove_interest(Interest &i, uint32_t context);

		// cloze_zones removes objects visible through the zones from the client and unsubscribes
		// from the associated location channels for those objects.
		void close_zones(uint32_t parent, const std::unordered_set<uint32_t> &killed_zones);


		/* Client Interface */
		// send_disconnect must close any connections with a connected client; the given reason and
		// error should be forwarded to the client. Additionaly, it is recommend to log the event.
		virtual void send_disconnect(uint16_t reason, const std::string &error_string, bool security=false);

		// send_datagram should foward the datagram to the client, or where appopriate parse
		// the packet and send the appropriate equivalent data.
		virtual void send_datagram(Datagram &dg) = 0;

		// handle_drop should immediately disconnect the client without sending any more data.
		virtual void handle_drop() = 0;

		// handle_add_object should inform the client of a new object. The datagram iterator
		// provided starts at the 'required fields' data, and may have optional fields following.
		virtual void handle_add_object(uint32_t do_id, uint32_t paren_id, uint32_t zone_id,
		                               uint16_t dc_id, DatagramIterator &dgi, bool other=false) = 0;

		// handle_add_ownership should inform the client it has control of a new object. The datagram
		// iterator provided starts at the 'required fields' data, and may have 'optional fields'.
		virtual void handle_add_ownership(uint32_t do_id, uint32_t paren_id, uint32_t zone_id,
		                                  uint16_t dc_id, DatagramIterator &dgi, bool other=false) = 0;

		// handle_set_field should inform the client that the field has been updated
		virtual void handle_set_field(uint32_t do_id, uint16_t field_id, const std::vector<uint8_t> &value) = 0;

		// handle_change_location should inform the client that the objects location has changed
		virtual void handle_change_location(uint32_t do_id, uint32_t new_parent, uint32_t new_zone) = 0;

		// handle_remove_object should send a mesage to remove the object from the connected client.
		virtual void handle_remove_object(uint32_t do_id) = 0;

		// handle_remove_ownership should notify the client it no has control of the object.
		virtual void handle_remove_ownership(uint32_t do_id) = 0;

		// handle_interest_done is called when all of the objects from an opened interest have been
		// received. Typically, informs the client that a particular group of objects is loaded.
		virtual void handle_interest_done(uint16_t interest_id, uint32_t context) = 0;
};
