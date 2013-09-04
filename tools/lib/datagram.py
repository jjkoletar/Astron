import struct

__all__ = ['Datagram']

DATATYPES = {
    'int8': '<b',
    'uint8': '<B',
    'int16': '<h',
    'uint16': '<H',
    'int32': '<i',
    'uint32': '<I',
    'int64': '<q',
    'uint64': '<Q',
}

CONSTANTS = {
    'CONTROL_CHANNEL': 4001,
    'CONTROL_ADD_CHANNEL': 2001,
    'CONTROL_REMOVE_CHANNEL': 2002,

    'CONTROL_ADD_RANGE': 2008,
    'CONTROL_REMOVE_RANGE': 2009,

    'CONTROL_ADD_POST_REMOVE': 2010,
    'CONTROL_CLEAR_POST_REMOVE': 2011,
}

locals().update(CONSTANTS)
__all__.extend(CONSTANTS.keys())

class Datagram(object):
    def __init__(self, data=b''):
        self._data = data

        def make_adder(v):
            def adder(data):
                self.add_raw(struct.pack(v, data))
            return adder

        for k,v in DATATYPES.items():
            adder = make_adder(v)
            setattr(self, 'add_' + k, adder)

    def add_raw(self, data):
        self._data += data

    def add_string(self, string):
        self.add_uint16(len(string))
        self.add_raw(string)

    def get_data(self):
        return self._data

    def get_payload(self):
        return self._data[8*ord(self._data[0])+1:]

    def get_channels(self):
        return set(struct.unpack('<x' + 'Q'*ord(self._data[0]),
                                 self._data[:8*ord(self._data[0])+1]))

    def is_subset_of(self, other):
        return self.get_payload() == other.get_payload() and \
               self.get_channels() <= other.get_channels()

    # Common datagram type helpers:
    @classmethod
    def create(cls, recipients, sender, msgtype):
        dg = cls()
        dg.add_uint8(len(recipients))
        for recipient in recipients: dg.add_uint64(recipient)
        dg.add_uint64(sender)
        dg.add_uint16(msgtype)
        return dg

    @classmethod
    def create_control(cls):
        dg = cls()
        dg.add_uint8(1)
        dg.add_uint64(CONTROL_CHANNEL)
        return dg

    @classmethod
    def create_add_channel(cls, channel):
        dg = cls.create_control()
        dg.add_uint16(CONTROL_ADD_CHANNEL)
        dg.add_uint64(channel)
        return dg

    @classmethod
    def create_remove_channel(cls, channel):
        dg = cls.create_control()
        dg.add_uint16(CONTROL_REMOVE_CHANNEL)
        dg.add_uint64(channel)
        return dg

    @classmethod
    def create_add_range(cls, upper, lower):
        dg = cls.create_control()
        dg.add_uint16(CONTROL_ADD_RANGE)
        dg.add_uint64(upper)
        dg.add_uint64(lower)
        return dg

    @classmethod
    def create_remove_range(cls, upper, lower):
        dg = cls.create_control()
        dg.add_uint16(CONTROL_REMOVE_RANGE)
        dg.add_uint64(upper)
        dg.add_uint64(lower)
        return dg

    @classmethod
    def create_add_post_remove(cls, datagram):
        dg = cls.create_control()
        dg.add_uint16(CONTROL_ADD_POST_REMOVE)
        dg.add_string(datagram.get_data())
        return dg

    @classmethod
    def create_clear_post_remove(cls):
        dg = cls.create_control()
        dg.add_uint16(CONTROL_CLEAR_POST_REMOVE)
        return dg

class DatagramIterator(object):
    def __init__(self, datagram, offset = 0):
        self._datagram = datagram
        self._data = datagram.get_data()
        self._offset = offset

    def read_header(self):
        channels = self._datagram.channels()
        self.seek(8*ord(self._data[0])+1)
        sender = self.read_uint64()
        msg_type = self.read_uint16()
        payload_length = len(self.data) - offset_
        return (channels, sender, msg_type, payload_length)

        self._offset += 4
        if self._offset > len(self.data):
            raise EOFError('End of Datagram')

        return struct.unpack("<H", self._data[self._offset-4:self._offset])


    def read_uint8(self):
        self._offset += 2
        if self._offset > len(self.data):
            raise EOFError('End of Datagram')

        return struct.unpack("<B", self._data[self._offset-2:self._offset])

    def read_uint16(self):
        self._offset += 4
        if self._offset > len(self.data):
            raise EOFError('End of Datagram')

        return struct.unpack("<H", self._data[self._offset-4:self._offset])

    def read_uint64(self):
        self._offset += 8
        if self._offset > len(self.data):
            raise EOFError('End of Datagram')

        return struct.unpack("<H", self._data[self._offset-8:self._offset])

    def seek(self, offset):
        self._offset = offset

    def tell(self):
        return self._offset
        msg_type = self.read_uint16()

        return (channels, sender, msg_type, payload_length)