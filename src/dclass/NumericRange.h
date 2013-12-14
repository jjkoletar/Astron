// Filename: dcNumericRange.h
// Created by:  drose (21Jun04)
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

#ifndef DCNUMERICRANGE_H
#define DCNUMERICRANGE_H

#include "dcbase.h"
#include "hashGenerator.h"
#include "dcPacker.h"

////////////////////////////////////////////////////////////////////
//       Class : DCNumericRange
// Description : Represents a range of legal integer or floating-point
//               values.  This is used to constrain simple numeric
//               types, as well as array sizes.
////////////////////////////////////////////////////////////////////
template <class NUM>
class DCNumericRange
{
	public:
		typedef NUM Number;

		inline DCNumericRange();
		inline DCNumericRange(Number min, Number max);
		inline DCNumericRange(const DCNumericRange &copy);
		inline void operator = (const DCNumericRange &copy);

		bool is_in_range(Number num) const;
		inline void validate(Number num, bool &range_error) const;

		inline bool has_one_value() const;
		inline Number get_one_value() const;

		void generate_hash(HashGenerator &hashgen) const;

		void output(ostream &out, Number divisor = 1) const;
		void output_char(ostream &out, Number divisor = 1) const;

	public:
		inline void clear();
		bool add_range(Number min, Number max);

		inline bool is_empty() const;
		inline int get_num_ranges() const;
		inline Number get_min(int n) const;
		inline Number get_max(int n) const;

	private:
		class MinMax
		{
			public:
				inline bool operator < (const MinMax &other) const;

				Number _min;
				Number _max;
		};
		inline void output_minmax(ostream &out, Number divisor, const MinMax &range) const;
		inline void output_minmax_char(ostream &out, const MinMax &range) const;

		typedef pvector<MinMax> Ranges;
		Ranges _ranges;
};

#include "dcNumericRange.I"

typedef DCNumericRange<int> DCIntRange;
typedef DCNumericRange<unsigned int> DCUnsignedIntRange;
typedef DCNumericRange<int64_t> DCInt64Range;
typedef DCNumericRange<uint64_t> DCUnsignedInt64Range;
typedef DCNumericRange<double> DCDoubleRange;

#endif