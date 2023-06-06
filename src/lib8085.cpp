#include "lib8085.h"
#include <iostream>
#include <iomanip>

#define get_word(h, l) ((h << 8) | l)
#define get_hbyte(w) (w >> 8)
#define get_lbyte(w) ((w << 8) >> 8)

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

    uint8_t Processor::get_imm()
    {
        return mem[program_counter++];
    }

    uint8_t Processor::get_hl_mem()
    {
        uint16_t address = get_word(reg_h, reg_l);

        return mem[address];
    }

    uint16_t Processor::get_imm_16()
    {
        return get_word(mem[program_counter++], mem[program_counter++]);
    }

    void Processor::push_stack(uint8_t val)
    {
        mem[--stack_pointer] = val;
    }

    void Processor::push_stack_16(uint16_t val)
    {
        mem[--stack_pointer] = get_lbyte(val);
        mem[--stack_pointer] = get_hbyte(val);
    }

    uint8_t Processor::pop_stack()
    {
        return mem[stack_pointer++];
    }

    uint16_t Processor::pop_stack_16()
    {
        return get_word(mem[stack_pointer++], mem[stack_pointer++]);
    }

    void Processor::exec(int no_of_instructions)
    {
        uint16_t op_code = mem[program_counter++];

        switch(op_code)
        {
            case ACI:
                {
                    uint8_t operand = get_imm();
                    add(operand, true);
                }
                break;
            case ADC_A:
                {
                    add(reg_a, true);
                }
                break;
            case ADC_B:
                {
                    add(reg_b, true);
                }
                break;
            case ADC_C:
                {
                    add(reg_c, true);
                }
                break;
            case ADC_D:
                {
                    add(reg_d, true);
                }
                break;
            case ADC_E:
                {
                    add(reg_e, true);
                }
                break;
            case ADC_H:
                {
                    add(reg_h, true);
                }
                break;
            case ADC_L:
                {
                    add(reg_l, true);
                }
                break;
            case ADC_M:
                {
                    uint8_t operand = get_hl_mem();

                    add(operand, true);
                }
                break;
            case ADD_A:
                {
                    add(reg_a, false);
                }
                break;
            case ADD_B:
                {
                    add(reg_b, false);
                }
                break;
            case ADD_C:
                {
                    add(reg_c, false);
                }
                break;
            case ADD_D:
                {
                    add(reg_d, false);
                }
                break;
            case ADD_E:
                {
                    add(reg_e, false);
                }
                break;
            case ADD_H:
                {
                    add(reg_h, false);
                }
                break;
            case ADD_L:
                {
                    add(reg_l, false);
                }
                break;
            case ADD_M:
                {
                    uint8_t operand = get_hl_mem();

                    add(operand, false);
                }
                break;
            case ADI:
                {
                    uint8_t addend = get_imm();

                    add(addend, false);
                }
                break;
            case ANA_A:
                {
                    reg_a &= reg_a;

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case ANA_B:
                {
                    reg_a &= reg_b;

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case ANA_C:
                {
                    reg_a &= reg_c;

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case ANA_D:
                {
                    reg_a &= reg_d;

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case ANA_E:
                {
                    reg_a &= reg_e;

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case ANA_H:
                {
                    reg_a &= reg_h;

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case ANA_L:
                {
                    reg_a &= reg_l;

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case ANA_M:
                {
                    reg_a &= get_hl_mem();

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case ANI:
                {
                    reg_a &= get_imm();

                    sign = reg_a < 0 ? true : false;
                    zero = reg_a == 0 ? true : false;

                    carry = false;
                    auxiliary_carry = false;
                }
                break;
            case CALL:
                {
                    uint16_t address = get_imm_16();
                    uint16_t next_ins = program_counter + 1;
                    program_counter = address;

                    push_stack_16(next_ins);
                }
                break;
            case CC:
                {
                    if(carry)
                    {
                        uint16_t address = get_imm_16();
                        uint16_t next_ins = program_counter + 1;
                        program_counter = address;

                        push_stack_16(next_ins);
                    }
                    else
                    {
                        program_counter += 2;
                    }
                }
                break;
            case CNC:
                {
                    if(!carry)
                    {
                        uint16_t address = get_imm_16();
                        uint16_t next_ins = program_counter + 1;
                        program_counter = address;

                        push_stack_16(next_ins);
                    }
                    else
                    {
                        program_counter += 2;
                    }
                }
                break;
            case CP:
                {
                    if(!sign)
                    {
                        uint16_t address = get_imm_16();
                        uint16_t next_ins = program_counter + 1;
                        program_counter = address;

                        push_stack_16(next_ins);
                    }
                    else
                    {
                        program_counter += 2;
                    }
                }
                break;
            case CM:
                {
                    if(!sign)
                    {
                        uint16_t address = get_imm_16();
                        uint16_t next_ins = program_counter + 1;
                        program_counter = address;

                        push_stack_16(next_ins);
                    }
                    else
                    {
                        program_counter += 2;
                    }
                }
                break;
            case CZ:
                {
                    if(zero)
                    {
                        uint16_t address = get_imm_16();
                        uint16_t next_ins = program_counter + 1;
                        program_counter = address;

                        push_stack_16(next_ins);
                    }
                    else
                    {
                        program_counter += 2;
                    }
                }
                break;
            case CNZ:
                {
                    if(!zero)
                    {
                        uint16_t address = get_imm_16();
                        uint16_t next_ins = program_counter + 1;
                        program_counter = address;

                        push_stack_16(next_ins);
                    }
                    else
                    {
                        program_counter += 2;
                    }
                }
                break;
            case CPE:
                {
                    if(parity)
                    {
                        uint16_t address = get_imm_16();
                        uint16_t next_ins = program_counter + 1;
                        program_counter = address;

                        push_stack_16(next_ins);
                    }
                    else
                    {
                        program_counter += 2;
                    }
                }
                break;
            case MOV_A_A:
                {
                    reg_a = reg_a;
                }
                break;
            case MOV_A_B:
                {
                    reg_a = reg_b;
                }
                break;
            case MOV_A_M:
                {
                    uint16_t address = get_word(reg_h, reg_l);
                    reg_a = mem[address];
                }
                break;
            case JMP:
                {
                    program_counter = get_word(mem[program_counter++], mem[program_counter++]);
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

#if 0
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
#endif
}
