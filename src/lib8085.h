#pragma once
#include <cstdint>
#include "instruction_set.h"
#include <iostream>

namespace lib8085
{
	class Processor
	{
		public:
		uint8_t reg_a, reg_b, reg_c, reg_d, reg_e, reg_h, reg_l;
		uint16_t program_counter, stack_pointer;

		uint8_t* mem;

		// Flags
		bool sign;
		bool zero;
		bool parity;
		bool carry;
		bool auxiliary_carry;

        Processor();

        ~Processor();

		void exec(int no_of_instructions);
        void reset();
        void print();
        friend std::ostream& operator<<(std::ostream&, const Processor&);

        private:
        void add(int addend, bool with_carry);
    };

}
