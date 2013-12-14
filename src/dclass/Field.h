// Filename: Field.h
// Created by: drose (11 Oct, 2000)
//
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//

#pragma once
#include "dcbase.h"
#include "PackerInterface.h"
#include "KeywordList.h"
namespace dclass   // open namespace
{


// Forward declarations
class Packer;
class AtomicField;
class MolecularField;
class Parameter;
class Switch;
class Class;
class HashGenerator;

// A Field is a single field of a Distributed Class, either atomic or molecular.
class EXPCL_DIRECT Field : public PackerInterface, public KeywordList
{
	public:
		Field();
		Field(const string &name, Class *dclass);
		virtual ~Field();

	PUBLISHED:
		// get_number returns a unique index number associated with this field.
		//     This is defined implicitly when the .dc file(s) are read.
		inline int get_number() const;

		// get_class returns the Class pointer for the class that contains this field.
		inline Class *get_class() const;


		// as_field returns the same pointer converted to a field pointer,
		//     if this is in fact a field; otherwise, returns NULL.
		virtual Field *as_field();
		virtual const Field *as_field() const;

		// as_atomic_field returns the same field pointer converted to an atomic field
		//     pointer, if this is in fact an atomic field; otherwise, returns NULL.
		virtual AtomicField *as_atomic_field();
		virtual const AtomicField *as_atomic_field() const;

		// as_molecular_field returns the same field pointer converted to a molecular field
		//     pointer, if this is in fact a molecular field; otherwise, returns NULL.
		virtual MolecularField *as_molecular_field();
		virtual const MolecularField *as_molecular_field() const;

		// as_parameter returns the same field pointer converted to a parameter
		//     pointer, if this is in fact a parameter; otherwise, returns NULL.
		virtual Parameter *as_parameter();
		virtual const Parameter *as_parameter() const;

		// format_data accepts a blob that represents the packed data for this field,
		//     returns a string formatting it for human consumption.
		//     Returns empty string if there is an error.
		string format_data(const string &packed_data, bool show_field_names = true);

		// parse_string given a human-formatted string (for instance, as
		//     returned by format_data(), above) that represents the value of this field,
		//     parse the string and return the corresponding packed data.
		//     Returns empty string if there is an error.
		string parse_string(const string &formatted_string);


		// validate_ranges verifies that all of the packed values in the field data are
		//     within the specified ranges and that there are no extra bytes on the end
		//     of the record.  Returns true if all fields are valid, false otherwise.
		bool validate_ranges(const string &packed_data) const;

		// has_default_value returns true if a default value has been explicitly
		//     established for this field, false otherwise.
		inline bool has_default_value() const;

		// get_default_value returns the default value for this field.
		//     If a default value hasn't been set, returns an implicit default.
		inline const string &get_default_value() const;

		// is_bogus_field returns true if the field has been flagged as a bogus field.
		//     This can occur during parsing, but should not occur in a normal valid dc file.
		inline bool is_bogus_field() const;

		// is_required returns true if the "required" flag is set for this field, false otherwise.
		inline bool is_required() const;
		// is_broadcast returns true if the "broadcast" flag is set for this field, false otherwise.
		inline bool is_broadcast() const;
		// is_ram returns true if the "ram" flag is set for this field, false otherwise.
		inline bool is_ram() const;
		// is_db returns true if the "db" flag is set for this field, false otherwise.
		inline bool is_db() const;
		// is_clsend returns true if the "clsend" flag is set for this field, false otherwise.
		inline bool is_clsend() const;
		// is_clrecv returns true if the "clrecv" flag is set for this field, false otherwise.
		inline bool is_clrecv() const;
		// is_ownsend returns true if the "ownsend" flag is set for this field, false otherwise.
		inline bool is_ownsend() const;
		// is_ownrecv returns true if the "ownrecv" flag is set for this field, false otherwise.
		inline bool is_ownrecv() const;
		// is_airecv returns true if the "airecv" flag is set for this field, false otherwise.
		inline bool is_airecv() const;

		// output and write output a string representation of this instance to <out>.
		inline void output(ostream &out) const;
		inline void write(ostream &out, int indent_level) const;
	public:
		// output and write output a string representation of this instance to <out>.
		virtual void output(ostream &out, bool brief) const = 0;
		virtual void write(ostream &out, bool brief, int indent_level) const = 0;

		// generate_hash accumulates the properties of this field into the hash.
		virtual void generate_hash(HashGenerator &hashgen) const;

		// pack_default_value packs the default value for this field into <pack_data>
		//     If a default value hasn't been set, packs an implicit default.
		virtual bool pack_default_value(PackData &pack_data, bool &pack_error) const;

		// set_name sets the name of this field.
		virtual void set_name(const string &name);


		// set_number assigns the unique number to this field.  This is normally
		//     called only by the Class interface as the field is added.
		inline void set_number(int number);
		// set_class assigns the class pointer to this field.  This is normally
		//     called only by the Class interface as the field is added.
		inline void set_class(Class *dclass);

		// set_default_value establishes a default value for this field.
		inline void set_default_value(const string &default_value);

	protected:
		// refresh_default_value recomputes the default value of the field by repacking it.
		void refresh_default_value();

	protected:
		Class *m_class; // the class that this field belongs to
		int m_number; // the unique index of the field in the .dc file
		bool m_default_value_stale; // is true if the default value hasn't been computed
		bool m_has_default_value; // is true if an explicity default has been set
		bool m_bogus_field; // is true if the field is incomplete (only encountered during parsing)

	private:
		string m_default_value; // the binary data of the default value encoded in a string

};

inline ostream &operator << (ostream &out, const Field &field)
{
	field.output(out);
	return out;
}



} // close namespace dclass

#include "Field.ipp"