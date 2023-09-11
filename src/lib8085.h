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
		bool sign; // Set on if 7th bit of acc is on, otherwise off
		bool zero;
		bool parity;
		bool carry;
		bool auxiliary_carry;

        Processor();

        ~Processor();

		void exec(int no_of_instructions);
        void reset();
        void print();

        uint8_t get_imm();
        uint16_t get_imm_16();

        uint16_t get_word(uint8_t a, uint8_t b);

        void push_stack(uint8_t val);
        void push_stack_16(uint16_t val);

        uint8_t pop_stack();
        uint16_t pop_stack_16();

        // friend std::ostream& operator<<(std::ostream&, const Processor&);

        private:
        void add(int addend, bool with_carry);
        void cmp(uint8_t a, uint8_t b);
        uint8_t or(uint8_t a, uint8_t b);
        void sub(uint8_t subtrahend, bool with_carry);
        uint8_t xor(uint8_t a, uint8_t b);
    };

}
