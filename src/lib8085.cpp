#include "lib8085.h"
#include <iostream>
#include <iomanip>

#define get_byte(h, l) ((h << 8) | l)

namespace lib8085
{
    Processor::Processor()
    {
        mem = new uint8_t[1 << 16];
        reset();
    }

    Processor::~Processor()
    {
        delete mem;
    }

    void Processor::reset()
    {
        reg_a = 0;
        reg_b = 0;
        reg_c = 0;
        reg_d = 0;
        reg_e = 0;
        reg_h = 0;
        reg_l = 0;

        stack_pointer   = 0;
        program_counter = 0;

        for(int i = 0; i < (1 << 16); i ++)
        {
            mem[i] = 0;
        }

        sign   = false;
        zero   = false;
        parity = false;
        carry  = false;
        auxiliary_carry = false;
    }

    void Processor::exec(int no_of_instructions)
    {
        uint16_t op_code = mem[program_counter++];

        switch(op_code)
        {
            case InstructionSet::ADD_A:
                {
                    add(reg_a, false);
                }
                break;
            case InstructionSet::ADD_M:
                {
                    uint16_t address = get_byte(reg_h, reg_l);

                    add(mem[address], false);
                }
                break;
            case InstructionSet::ADI:
                {
                    uint8_t addend = mem[program_counter++];

                    add(addend, false);
                }
                break;
            case InstructionSet::ANA_A:
                {
                    reg_a &= reg_a;

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;
                }
                break;
            case InstructionSet::MOV_A_A:
                {
                    reg_a = reg_a;
                }
                break;
            case InstructionSet::MOV_A_B:
                {
                    reg_a = reg_b;
                }
                break;
            case InstructionSet::MOV_A_M:
                {
                    uint16_t address = get_byte(reg_h, reg_l);
                    reg_a = mem[address];
                }
                break;
            case InstructionSet::JMP:
                {
                    program_counter = get_byte(mem[program_counter++], mem[program_counter++]);
                }
                break;
            default:
                {
                    // TODO: Handle error
                }
                break;
        }
    }

    void Processor::add(int addend, bool with_carry)
    {
        if(with_carry)
        {
            uint16_t res = reg_a + addend + carry;

            if(res > 0xff)
            {
                // Remove higher nibble and store value to reg_a
                reg_a = res & 0x0f;
            }
        }
        else
        {
            reg_a += addend;
        }
    }

    std::ostream& operator<<(std::ostream& out, const Processor& cpu)
    {
        out.fill('0');

        out << "-------------\n";
        out << "A: 0x" << std::setw(2) /* << std::hex */ << (int)cpu.reg_a << "(" << std::setw(2) << (int)cpu.reg_a << ") ";
        out << "B: 0x" << std::setw(2) /* << std::hex */ << (int)cpu.reg_b << "(" << std::setw(2) << (int)cpu.reg_b << ")\n";

        out << "C: 0x" << std::setw(2) /* << std::hex */ << (int)cpu.reg_c << "(" << std::setw(2) << (int)cpu.reg_c << ") ";
        out << "D: 0x" << std::setw(2) /* << std::hex */ << (int)cpu.reg_d << "(" << std::setw(2) << (int)cpu.reg_d << ")\n";

        out << "E: 0x" << std::setw(2) /* << std::hex */ << (int)cpu.reg_e << "(" << std::setw(2) << (int)cpu.reg_e << ")\n";

        out << "H: 0x" << std::setw(2) /* << std::hex */ << (int)cpu.reg_h << "(" << std::setw(2) << (int)cpu.reg_h << ") ";
        out << "L: 0x" << std::setw(2) /* << std::hex */ << (int)cpu.reg_l << "(" <<std::setw(2) <<  (int)cpu.reg_l << ")\n";

        out << "-------------\n";
        out << "S.Z.P.C.W\n";
        out << cpu.sign << ' ';
        out << cpu.zero << ' ';
        out << cpu.parity << ' ';
        out << cpu.carry << ' ';
        out << cpu.auxiliary_carry << '\n';
        out << "-------------\n";

        return out;
    }
}
