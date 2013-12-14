// Filename: File.ipp
// Created by: drose (10 Jan, 2006)
//
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
namespace dclass   // open namespace
{


// all_objects_valid returns true if all of the classes read from the DC
//     file were defined and valid, or false if any of them were undefined
//     ("bogus classes").  If this is true, we might have read a partial file.
inline bool File::all_objects_valid() const
{
	return m_all_objects_valid;
}

// check_inherited_fields rebuilds all of the inherited fields tables, if necessary.
inline void File::check_inherited_fields()
{
	if(m_inherited_fields_stale)
	{
		rebuild_inherited_fields();
	}
}

// mark_inherited_fields_stale indicates that something has changed in one or more
//     of the inheritance chains or the set of fields; the next time check_inherited_fields()
//     is called, the inherited fields tables of all classes will be rebuilt.
inline void File::mark_inherited_fields_stale()
{
	m_inherited_fields_stale = true;
}


} // close namespace dclass