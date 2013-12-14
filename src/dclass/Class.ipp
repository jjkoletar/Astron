// Filename: Class.ipp
// Created by: drose (15 Sep, 2004)
//
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
namespace dclass   // open namespace
{


// get_file returns the File object that contains the class.
inline File *Class::get_file() const
{
	return m_file;
}

// get_name returns the name of this class.
inline const string& Class::get_name() const
{
	return m_name;
}

// get_number returns a unique index number associated with this class.
//     This is defined implicitly when a .dc file is read.
inline int Class::get_index() const
{
	return m_number;
}

// is_struct returns true if the class has been identified with the "struct"
//     keyword in the dc file, false if it was declared with "dclass".
inline bool Class::is_struct() const
{
	return m_is_struct;
}

// is_bogus_class returns true if the class has been flagged as a bogus class.
//     This is set for classes that are generated by the parser as placeholder
//     for missing classes, as when reading a partial file; it should not occur
//     in a normal valid dc file.
inline bool Class::is_bogus_class() const
{
	return m_bogus_class;
}


} // close namespace dclass