// Filename: PackerInterface.ipp
// Created by: drose (18 Jun, 2004)
//
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//

#include <string.h> // jjkoletar: include string.h so that *nix likes the memcpy calls
namespace dclass   // open namespace dclass
{

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::get_name
//       Access: Published
//  Description: Returns the name of this field, or empty string
//               if the field is unnamed.
////////////////////////////////////////////////////////////////////
inline const string &PackerInterface::
get_name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::check_match
//       Access: Published
//  Description: Returns true if the other interface is bitwise the
//               same as this one--that is, a uint32 only matches a
//               uint32, etc. Names of components, and range limits,
//               are not compared.
////////////////////////////////////////////////////////////////////
inline bool PackerInterface::
check_match(const PackerInterface *other) const
{
	return do_check_match(other);
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::has_fixed_byte_size
//       Access: Public
//  Description: Returns true if this field type always packs to the
//               same number of bytes, false if it is variable.
////////////////////////////////////////////////////////////////////
inline bool PackerInterface::
has_fixed_byte_size() const
{
	return m_has_fixed_byte_size;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::get_fixed_byte_size
//       Access: Public
//  Description: If has_fixed_byte_size() returns true, this returns
//               the number of bytes this field type will use.
////////////////////////////////////////////////////////////////////
inline size_t PackerInterface::
get_fixed_byte_size() const
{
	return m_fixed_byte_size;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::has_fixed_structure
//       Access: Public
//  Description: Returns true if this field type always has the same
//               structure regardless of the data in the stream, or
//               false if its structure may vary.  This is almost, but
//               not quite, the same thing as has_fixed_byte_size.
//               The difference is that a Switch may have multiple
//               cases all with the same byte size, but they will
//               still (presumably) have different structures, in the
//               sense that the actual list of fields varies according
//               to the live data.
////////////////////////////////////////////////////////////////////
inline bool PackerInterface::
has_fixed_structure() const
{
	return m_has_fixed_structure;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::has_range_limits
//       Access: Public
//  Description: Returns true if this field, or any sub-field of this
//               field, has a limit imposed in the  file on its
//               legal values.  If this is false, then
//               unpack_validate() is trivial.
////////////////////////////////////////////////////////////////////
inline bool PackerInterface::
has_range_limits() const
{
	return m_has_range_limits;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::get_num_length_bytes
//       Access: Public
//  Description: Returns the number of bytes that should be written
//               into the stream on a push() to record the number of
//               bytes in the record up until the next pop().  This is
//               only meaningful if m_has_nested_fields is true.
////////////////////////////////////////////////////////////////////
inline size_t PackerInterface::
get_num_length_bytes() const
{
	return m_num_length_bytes;
}


////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::has_nested_fields
//       Access: Public
//  Description: Returns true if this field type has any nested fields
//               (and thus expects a push() .. pop() interface to the
//               Packer), or false otherwise.  If this returns true,
//               get_num_nested_fields() may be called to determine
//               how many nested fields are expected.
////////////////////////////////////////////////////////////////////
inline bool PackerInterface::
has_nested_fields() const
{
	return m_has_nested_fields;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::get_num_nested_fields
//       Access: Public
//  Description: Returns the number of nested fields required by this
//               field type.  These may be array elements or structure
//               elements.  The return value may be -1 to indicate the
//               number of nested fields is variable.
////////////////////////////////////////////////////////////////////
inline int PackerInterface::
get_num_nested_fields() const
{
	return m_num_nested_fields;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::get_pack_type
//       Access: Public
//  Description: Returns the type of value expected by this field.
////////////////////////////////////////////////////////////////////
inline PackType PackerInterface::
get_pack_type() const
{
	return m_pack_type;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_int8
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_int8(char *buffer, int8_t value)
{
	buffer[0] = (char)(value);

}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_int16
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_int16(char *buffer, int16_t value)
{
	buffer[0] = (char)(value & 0xff);
	buffer[1] = (char)((value >> 8) & 0xff);
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_int32
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_int32(char *buffer, int32_t value)
{
	buffer[0] = (char)(value & 0xff);
	buffer[1] = (char)((value >> 8) & 0xff);
	buffer[2] = (char)((value >> 16) & 0xff);
	buffer[3] = (char)((value >> 24) & 0xff);
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_int64
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_int64(char *buffer, int64_t value)
{
	buffer[0] = (char)(value & 0xff);
	buffer[1] = (char)((value >> 8) & 0xff);
	buffer[2] = (char)((value >> 16) & 0xff);
	buffer[3] = (char)((value >> 24) & 0xff);
	buffer[4] = (char)((value >> 32) & 0xff);
	buffer[5] = (char)((value >> 40) & 0xff);
	buffer[6] = (char)((value >> 48) & 0xff);
	buffer[7] = (char)((value >> 56) & 0xff);
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_uint8
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_uint8(char *buffer, uint8_t value)
{
	buffer[0] = (char)(value);
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_uint16
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_uint16(char *buffer, uint16_t value)
{
	buffer[0] = (char)(value & 0xff);
	buffer[1] = (char)((value >> 8) & 0xff);
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_length_tag
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_length_tag(char *buffer, length_tag_t value)
{
	// NOTE: Wondering if maybe memcpy would be better for this?
	//       Did it this way to copy other pack_value's (are they done that way for performance?)
	for(size_t i = 0; i < sizeof(length_tag_t); ++i)
	{
		buffer[i] = (char)((value >> 8 * i) & 0xff);
	}
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_uint32
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_uint32(char *buffer, uint32_t value)
{
	buffer[0] = (char)(value & 0xff);
	buffer[1] = (char)((value >> 8) & 0xff);
	buffer[2] = (char)((value >> 16) & 0xff);
	buffer[3] = (char)((value >> 24) & 0xff);
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_uint64
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_uint64(char *buffer, uint64_t value)
{
	buffer[0] = (char)(value & 0xff);
	buffer[1] = (char)((value >> 8) & 0xff);
	buffer[2] = (char)((value >> 16) & 0xff);
	buffer[3] = (char)((value >> 24) & 0xff);
	buffer[4] = (char)((value >> 32) & 0xff);
	buffer[5] = (char)((value >> 40) & 0xff);
	buffer[6] = (char)((value >> 48) & 0xff);
	buffer[7] = (char)((value >> 56) & 0xff);
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_pack_float64
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
do_pack_float64(char *buffer, double value)
{
#ifdef WORDS_BIGENDIAN
	// Reverse the byte ordering for big-endian machines.
	char *p = (char *)&value;
	for(size_t i = 0; i < 8; i++)
	{
		buffer[i] = p[7 - i];
	}
#else
	memcpy(buffer, &value, 8);
#endif
}


////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_int8
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline int8_t PackerInterface::
do_unpack_int8(const char *buffer)
{
	return (int8_t)buffer[0];
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_int16
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline int16_t PackerInterface::
do_unpack_int16(const char *buffer)
{
	return *(int16_t*)buffer;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_int32
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline int32_t PackerInterface::
do_unpack_int32(const char *buffer)
{
	return *(int32_t*)buffer;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_int64
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline int64_t PackerInterface::
do_unpack_int64(const char *buffer)
{
	return *(int64_t*)buffer;
}
////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_uint8
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline uint8_t PackerInterface::
do_unpack_uint8(const char *buffer)
{
	return *(uint8_t*)buffer;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_uint16
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline uint16_t PackerInterface::
do_unpack_uint16(const char *buffer)
{
	return *(uint16_t*)buffer;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_length_tag
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline length_tag_t PackerInterface::
do_unpack_length_tag(const char *buffer)
{
	return *(length_tag_t*)buffer;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_uint32
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline uint32_t PackerInterface::
do_unpack_uint32(const char *buffer)
{
	return *(uint32_t*)buffer;
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_uint64
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline uint64_t PackerInterface::
do_unpack_uint64(const char *buffer)
{
	return *(uint64_t*)buffer;
}


////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::do_unpack_float64
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
inline double PackerInterface::
do_unpack_float64(const char *buffer)
{
#ifdef WORDS_BIGENDIAN
	char reverse[8];

	// Reverse the byte ordering for big-endian machines.
	for(size_t i = 0; i < 8; i++)
	{
		reverse[i] = buffer[7 - i];
	}
	return *(double *)reverse;
#else
	return *(double *)buffer;
#endif  // WORDS_BIGENDIAN
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::validate_int_limits
//       Access: Public, Static
//  Description: Confirms that the signed value fits within num_bits
//               bits.  Sets range_error true if it does not.
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
validate_int_limits(int value, int num_bits, bool &range_error)
{
	// What we're really checking is that all of the bits above the
	// lower (num_bits - 1) bits are the same--either all 1 or all 0.

	// First, turn on the lower (num_bits - 1).
	int mask = ((int)1 << (num_bits - 1)) - 1;
	value |= mask;

	// The result should be either mask (all high bits are 0) or -1 (all
	// high bits are 1).  If it is anything else we have a range error.
	if(value != mask && value != -1)
	{
		range_error = true;
	}
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::validate_int64_limits
//       Access: Public, Static
//  Description: Confirms that the signed value fits within num_bits
//               bits.  Sets range_error true if it does not.
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
validate_int64_limits(int64_t value, int num_bits, bool &range_error)
{
	int64_t mask = ((int64_t)1 << (num_bits - 1)) - 1;
	value |= mask;

	if(value != mask && value != -1)
	{
		range_error = true;
	}
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::validate_uint_limits
//       Access: Public, Static
//  Description: Confirms that the unsigned value fits within num_bits
//               bits.  Sets range_error true if it does not.
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
validate_uint_limits(unsigned int value, int num_bits, bool &range_error)
{
	// Here we're really checking that all of the bits above the lower
	// num_bits bits are all 0.

	unsigned int mask = ((unsigned int)1 << num_bits) - 1;
	value &= ~mask;

	if(value != 0)
	{
		range_error = true;
	}
}

////////////////////////////////////////////////////////////////////
//     Function: PackerInterface::validate_uint64_limits
//       Access: Public, Static
//  Description: Confirms that the unsigned value fits within num_bits
//               bits.  Sets range_error true if it does not.
////////////////////////////////////////////////////////////////////
inline void PackerInterface::
validate_uint64_limits(uint64_t value, int num_bits, bool &range_error)
{
	uint64_t mask = ((uint64_t)1 << num_bits) - 1;
	value &= ~mask;

	if(value != 0)
	{
		range_error = true;
	}
}


} // close namespace dclass