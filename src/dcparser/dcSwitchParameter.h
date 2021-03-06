// Filename: dcClassParameter.h
// Created by:  drose (29Jun04)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef DCSWITCHPARAMETER_H
#define DCSWITCHPARAMETER_H

#include "dcbase.h"
#include "dcParameter.h"

class DCSwitch;

////////////////////////////////////////////////////////////////////
//       Class : DCSwitchParameter
// Description : This represents a switch object used as a
//               parameter itself, which packs the appropriate fields
//               of the switch into the message.
////////////////////////////////////////////////////////////////////
class EXPCL_DIRECT DCSwitchParameter : public DCParameter
{
	public:
		DCSwitchParameter(const DCSwitch *dswitch);
		DCSwitchParameter(const DCSwitchParameter &copy);

	PUBLISHED:
		virtual DCSwitchParameter *as_switch_parameter();
		virtual const DCSwitchParameter *as_switch_parameter() const;
		virtual DCParameter *make_copy() const;
		virtual bool is_valid() const;

		const DCSwitch *get_switch() const;

	public:
		virtual DCPackerInterface *get_nested_field(int n) const;

		const DCPackerInterface *apply_switch(const char *value_data, size_t length) const;

		virtual void output_instance(ostream &out, bool brief, const string &prename,
		                             const string &name, const string &postname) const;
		virtual void write_instance(ostream &out, bool brief, int indent_level,
		                            const string &prename, const string &name,
		                            const string &postname) const;
		virtual void generate_hash(HashGenerator &hashgen) const;
		virtual bool pack_default_value(DCPackData &pack_data, bool &pack_error) const;

	protected:
		virtual bool do_check_match(const DCPackerInterface *other) const;
		virtual bool do_check_match_switch_parameter(const DCSwitchParameter *other) const;

	private:
		const DCSwitch *_dswitch;
};

#endif
