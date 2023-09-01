#include "lib8085.h"
#include <iostream>
#include <iomanip>

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

        // for(int i = 0; i < (1 << 16); i ++)
        // {
        //     mem[i] = 0;
        // }

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

    uint16_t Processor::get_word(uint8_t a, uint8_t b)
    {
        uint16_t res = a;
        res <<= 8;
        res |= b;

        return res;
    }

    uint16_t Processor::get_imm_16()
    {
        uint8_t b = mem[program_counter++];
        uint8_t a = mem[program_counter++];
        return get_word(a, b);
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
                    uint16_t hl = get_word(reg_h, reg_l);
                    uint8_t operand = mem[hl];

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
                    uint16_t hl = get_word(reg_h, reg_l);
                    uint8_t operand = mem[hl];

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
                    uint16_t hl = get_word(reg_h, reg_l);
                    uint8_t operand = mem[hl];

                    reg_a &= operand;

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
            case CMA:
                {
                    reg_a ^= 11111111;

                }
                break;
            case CMC:
                {
                    carry = !carry;
                }
                break;
            case CMP_A:
                {
                    zero = true;
                }
                break;
            case CMP_B:
                {
                    cmp(reg_a, reg_b);
                }
                break;
            case CMP_C:
                {
                    cmp(reg_a, reg_c);
                }
                break;
            case CMP_D:
                {
                    cmp(reg_a, reg_d);
                }
                break;
            case CMP_E:
                {
                    cmp(reg_a, reg_e);
                }
                break;
            case CMP_H:
                {
                    cmp(reg_a, reg_h);
                }
                break;
            case CMP_L:
                {
                    cmp(reg_a, reg_l);
                }
                break;
            case CMP_M:
                {
                    uint16_t hl = get_word(reg_h, reg_l);
                    uint8_t operand = mem[hl];

                    cmp(reg_a, operand);
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
            // Call on minus
            case CM:
                {
                    if(sign)
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
            case CPI:
                {
                    uint8_t operand = get_imm();

                    cmp(reg_a, operand);
                }
                break;
            case CPO:
                {
                    if(!parity)
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
            case DAA:
                {
                    uint8_t msn = (reg_a & 11110000) >> 4;
                    uint8_t lsn = reg_a >> 4;

                    if(lsn > 9)
                    {
                        auxiliary_carry = true;
                        lsn += 6;
                    }

                    if(msn > 9 || carry)
                    {
                        msn += 6;
                    }

                }
                break;
            case DAD_B:
                {
                    uint16_t res = reg_b;
                    res <<= 4;
                    res |= reg_c;

                    uint16_t hl = reg_h;
                    hl <<= 4;
                    hl |= reg_l;

                    res += hl;
                    reg_h = res >> 4;
                    reg_l = (res << 4) >> 4;
                }
                break;
            case DAD_D:
                {
                    uint16_t res = reg_d;
                    res <<= 4;
                    res |= reg_e;

                    uint16_t hl = reg_h;
                    hl <<= 4;
                    hl |= reg_l;

                    res += hl;
                    reg_h = res >> 4;
                    reg_l = (res << 4) >> 4;
                }
                break;
            case DAD_H:
                {
                    uint16_t res = reg_h;
                    res <<= 4;
                    res |= reg_l;

                    res = res * 2;
                    reg_h = res >> 4;
                    reg_l = (res << 4) >> 4;
                }
                break;
            case DAD_SP:
                {
                    uint16_t res = reg_h;
                    res <<= 4;
                    res |= reg_l;

                    res = stack_pointer + res;
                    reg_h = res >> 4;
                    reg_l = (res << 4) >> 4;
                }
                break;
            case DCR_A:
                {
                    reg_a--;
                }
                break;
            case DCR_B:
                {
                    reg_b--;
                }
                break;
            case DCR_C:
                {
                    reg_c--;
                }
                break;
            case DCR_D:
                {
                    reg_d--;
                }
                break;
            case DCR_E:
                {
                    reg_e--;
                }
                break;
            case DCR_H:
                {
                    reg_h--;
                }
                break;
            case DCR_L:
                {
                    reg_l--;
                }
                break;
            case DCR_M:
                {
                    uint16_t hl = get_word(reg_h, reg_l);
                    uint8_t address = mem[hl];

                    mem[address]--;
                }
                break;
            case DCX_B:
                {
                    uint16_t rp = reg_b;
                    rp <<= 4;
                    rp |= reg_c;

                    rp --;
                    reg_b = rp >> 4;
                    reg_c = (rp << 4) >> 4;
                }
                break;
            case DCX_D:
                {
                    uint16_t rp = reg_d;
                    rp <<= 4;
                    rp |= reg_e;

                    rp --;
                    reg_d = rp >> 4;
                    reg_e = (rp << 4) >> 4;
                }
                break;
            case DCX_H:
                {
                    uint16_t rp = reg_h;
                    rp <<= 4;
                    rp |= reg_l;

                    rp --;
                    reg_h = rp >> 4;
                    reg_l = (rp << 4) >> 4;
                }
                break;
            case DCX_SP:
                {
                    stack_pointer --;
                }
                break;
            case DI:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case EI:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case HLT:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case IN:
                {
                    uint8_t operand = get_imm();
                    throw std::exception("Not implemented yet");
                }
                break;
            case INR_A:
                {
                    reg_a++;
                }
                break;
            case INR_B:
                {
                    reg_b++;
                }
                break;
            case INR_C:
                {
                    reg_c++;
                }
                break;
            case INR_D:
                {
                    reg_d++;
                }
                break;
            case INR_E:
                {
                    reg_e++;
                }
                break;
            case INR_H:
                {
                    reg_h++;
                }
                break;
            case INR_L:
                {
                    reg_l++;
                }
                break;
            case INR_M:
                {
                    uint16_t hl = get_word(reg_h, reg_l);
                    uint8_t address = mem[hl];

                    mem[address]++;
                }
                break;
            case INX_B:
                {
                    uint16_t rp = reg_b;
                    rp <<= 4;
                    rp |= reg_c;

                    rp ++;
                    reg_b = rp >> 4;
                    reg_c = (rp << 4) >> 4;
                }
                break;
            case INX_D:
                {
                    uint16_t rp = reg_d;
                    rp <<= 4;
                    rp |= reg_e;

                    rp ++;
                    reg_d = rp >> 4;
                    reg_e = (rp << 4) >> 4;
                }
                break;
            case INX_H:
                {
                    uint16_t rp = reg_h;
                    rp <<= 4;
                    rp |= reg_l;

                    rp ++;
                    reg_h = rp >> 4;
                    reg_l = (rp << 4) >> 4;
                }
                break;
            case INX_SP:
                {
                    stack_pointer ++;
                }
                break;
            case JC:
                {
                    uint16_t operand = get_imm_16();

                    if(carry)
                    {
                        program_counter = operand;
                    }
                }
                break;
            case JNC:
                {
                    uint16_t operand = get_imm_16();

                    if(!carry)
                    {
                        program_counter = operand;
                    }
                }
                break;
            case JP:
                {
                    uint16_t operand = get_imm_16();

                    if(!sign)
                    {
                        program_counter = operand;
                    }
                }
                break;
            case JMP:
                {
                    uint16_t operand = get_imm_16();

                    program_counter = operand;
                }
                break;
            case JM:
                {
                    uint16_t operand = get_imm_16();

                    if(sign)
                    {
                        program_counter = operand;
                    }
                }
                break;
            case JZ:
                {
                    uint16_t operand = get_imm_16();

                    if(zero)
                    {
                        program_counter = operand;
                    }
                }
                break;
            case JNZ:
                {
                    uint16_t operand = get_imm_16();

                    if(!zero)
                    {
                        program_counter = operand;
                    }
                }
                break;
            case JPE:
                {
                    uint16_t operand = get_imm_16();

                    if(parity)
                    {
                        program_counter = operand;
                    }
                }
                break;
            case JPO:
                {
                    uint16_t operand = get_imm_16();

                    if(!parity)
                    {
                        program_counter = operand;
                    }
                }
                break;
            case LDA:
                {
                    uint16_t operand = get_imm_16();

                    reg_a = mem[operand];
                }
                break;
            case LDAX_B:
                {
                    uint16_t address = reg_b;
                    address <<= 4;
                    address |= reg_c;

                    reg_a = mem[address];
                }
                break;
            case LDAX_D:
                {
                    uint16_t address = reg_d;
                    address <<= 4;
                    address |= reg_e;

                    reg_a = mem[address];
                }
                break;
            case LHLD:
                {
                    uint16_t address = get_imm_16();

                    reg_l = mem[address];
                    reg_h = mem[address+1];
                }
                break;
            case LXI_B:
                {
                    uint16_t address = get_imm_16();

                    reg_b = mem[address];
                    reg_c = mem[address+1];
                }
                break;
            case LXI_D:
                {
                    uint16_t address = get_imm_16();

                    reg_d = mem[address];
                    reg_e = mem[address+1];
                }
                break;
            case LXI_H:
                {
                    uint16_t address = get_imm_16();

                    reg_h = mem[address];
                    reg_l = mem[address+1];
                }
                break;
            case LXI_SP:
                {
                    uint16_t address = get_imm_16();

                    stack_pointer = mem[address];
                    stack_pointer <<= 8;
                    stack_pointer |= mem[address+1];
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
            case MVI_A:
                {
                    uint8_t operand = get_imm();
                    reg_a = operand;
                }
                break;
            case MVI_B:
                {
                    uint8_t operand = get_imm();
                    reg_b = operand;
                }
                break;
            case MVI_C:
                {
                    uint8_t operand = get_imm();
                    reg_c = operand;
                }
                break;
            case MVI_D:
                {
                    uint8_t operand = get_imm();
                    reg_d = operand;
                }
                break;
            case MVI_E:
                {
                    uint8_t operand = get_imm();
                    reg_e = operand;
                }
                break;
            case MVI_H:
                {
                    uint8_t operand = get_imm();
                    reg_h = operand;
                }
                break;
            case MVI_L:
                {
                    uint8_t operand = get_imm();
                    reg_l = operand;
                }
                break;
            case MVI_M:
                {
                    uint16_t hl = get_word(reg_h, reg_l);
                    uint8_t operand = mem[hl];

                    reg_a = operand;
                }
                break;
            case NOP:
                {
                }
                break;
            case ORA_A:
                {
                    or(reg_a, reg_a);
                }
                break;
            case ORA_B:
                {
                    or(reg_a, reg_b);
                }
                break;
            case ORA_C:
                {
                    or(reg_a, reg_c);
                }
                break;
            case ORA_D:
                {
                    or(reg_a, reg_d);
                }
                break;
            case ORA_E:
                {
                    or(reg_a, reg_e);
                }
                break;
            case ORA_H:
                {
                    or(reg_a, reg_h);
                }
                break;
            case ORA_L:
                {
                    or(reg_a, reg_l);
                }
                break;
            case ORA_M:
                {
                    uint16_t hl = get_word(reg_h, reg_l);
                    uint8_t operand = mem[hl];

                    or(reg_a, operand);
                }
                break;
            case OUT:
                {
                    uint8_t operand = get_imm();
                    throw std::exception("Not implemented yet");
                }
                break;
            case PCHL:
                {
                    uint16_t hl = get_word(reg_h, reg_l);
                    program_counter = hl;
                }
                break;
            case POP_B:
                {
                    uint16_t data = mem[stack_pointer++];
                    reg_c = data;

                    data = mem[stack_pointer++];
                    reg_b = data;
                }
                break;
            case POP_D:
                {
                    uint16_t data = mem[stack_pointer++];
                    reg_e = data;

                    data = mem[stack_pointer++];
                    reg_d = data;
                }
                break;
            case POP_H:
                {
                    uint16_t data = mem[stack_pointer++];
                    reg_l = data;

                    data = mem[stack_pointer++];
                    reg_h = data;
                }
                break;
            case POP_PSW:
                {
                    uint16_t data = mem[stack_pointer++];

                    if(data & 10000000 == 10000000)
                        sign = true;
                    else
                        sign = false;
                    if(data & 01000000 == 01000000)
                        zero = true;
                    else
                        zero = false;

                    if(data & 00010000 == 00010000)
                        auxiliary_carry = true;
                    else
                        auxiliary_carry = false;

                    if(data & 00000100 == 00000100)
                        parity = true;
                    else
                        parity = false;

                    if(data & 00000001 == 00000001)
                        carry = true;
                    else
                        carry = false;
                }
                break;
            case RAL:
                {
                    if(reg_a & 10000000)
                    {
                        carry = true;
                    }
                    else
                    {
                        carry = false;
                    }

                    reg_a <<= 1;
                }
                break;
            case RAR:
                {
                    reg_a >>= 1;

                    if(carry)
                    {
                        reg_a |= 10000000;
                    }
                    else
                    {
                        reg_a ^= (reg_a >> 7) << 7;
                    }
                }
                break;
            case RC:
                {
                    if(carry)
                    {
                        // TODO: Check for overflow errors
                        program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                    }
                }
                break;
            case RET:
                {
                    // TODO: Check for overflow errors
                    program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                }
                break;
            case RIM:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RLC:
                {
                    uint8_t tmp = reg_a;

                    reg_a <<= 1;

                    if(tmp & 10000000)
                    {
                        carry = true;
                        reg_a |= 00000001;
                    }
                    else
                    {
                        carry = false;
                        reg_a ^= (reg_a << 7) >> 7;
                    }

                }
                break;
            case RM:
                {
                    if(sign)
                    {
                        // TODO: Check for overflow errors
                        program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                    }
                }
                break;
            case RNC:
                {
                    if(!carry)
                    {
                        // TODO: Check for overflow errors
                        program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                    }
                }
                break;
            case RNZ:
                {
                    if(!zero)
                    {
                        // TODO: Check for overflow errors
                        program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                    }
                }
                break;
            case RP:
                {
                    if(!sign)
                    {
                        // TODO: Check for overflow errors
                        program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                    }
                }
                break;
            case RPE:
                {
                    if(parity)
                    {
                        // TODO: Check for overflow errors
                        program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                    }
                }
                break;
            case RPO:
                {
                    if(!parity)
                    {
                        // TODO: Check for overflow errors
                        program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                    }
                }
                break;
            case RRC:
                {
                    uint8_t tmp = reg_a;

                    reg_a <<= 1;

                    if(tmp & 00000001)
                    {
                        carry = true;
                        reg_a |= 00000001;
                    }
                    else
                    {
                        carry = false;
                        reg_a ^= (reg_a >> 7) << 7;
                    }
                }
                break;
            case RST_0:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RST_1:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RST_2:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RST_3:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RST_4:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RST_5:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RST_6:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RST_7:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case RZ:
                {
                    if(zero)
                    {
                        // TODO: Check for overflow errors
                        program_counter = get_word(mem[stack_pointer++], mem[stack_pointer++]);
                    }
                }
                break;
            case SBB_A:
                {
                    sub(reg_a, true);
                }
                break;
            case SBB_B:
                {
                    sub(reg_b, true);
                }
                break;
            case SBB_C:
                {
                    sub(reg_c, true);
                }
                break;
            case SBB_D:
                {
                    sub(reg_d, true);
                }
                break;
            case SBB_E:
                {
                    sub(reg_e, true);
                }
                break;
            case SBB_H:
                {
                    sub(reg_h, true);
                }
                break;
            case SBB_L:
                {
                    sub(reg_l, true);
                }
                break;
            case SBB_M:
                {
                    uint8_t operand = mem[get_word(reg_h, reg_l)];
                    sub(operand, true);
                }
                break;
            case SBI:
                {
                    uint8_t operand = get_imm();
                    sub(operand, true);
                }
                break;
            case SHLD:
                {
                    uint16_t address = get_imm_16();
                    mem[address] = reg_l;
                    mem[address++] = reg_h;
                }
                break;
            case SIM:
                {
                    throw std::exception("Not implemented yet");
                }
                break;
            case SPHL:
                {
                    stack_pointer = reg_h;
                    stack_pointer <<= 8;
                    stack_pointer &= 0xff00;
                    stack_pointer |= reg_l;
                }
                break;
            case STA:
                {
                    uint16_t address = get_imm_16();
                    mem[address] = reg_a;
                }
                break;
            case STAX_B:
                {
                    uint16_t address = get_word(reg_b, reg_c);
                    mem[address] = reg_a;
                }
                break;
            case STAX_D:
                {
                    uint16_t address = get_word(reg_d, reg_e);
                    mem[address] = reg_a;
                }
                break;
            case STAX_H:
                {
                    uint16_t address = get_word(reg_h, reg_l);
                    mem[address] = reg_a;
                }
                break;
            case STC:
                {
                    carry = true;
                }
                break;
            case SUB_A:
                {
                    sub(reg_a, false);
                }
                break;
            case SUB_B:
                {
                    sub(reg_b, false);
                }
                break;
            case SUB_C:
                {
                    sub(reg_c, false);
                }
                break;
            case SUB_D:
                {
                    sub(reg_d, false);
                }
                break;
            case SUB_E:
                {
                    sub(reg_e, false);
                }
                break;
            case SUB_H:
                {
                    sub(reg_h, false);
                }
                break;
            case SUB_L:
                {
                    sub(reg_l, false);
                }
                break;
            case SUB_M:
                {
                    uint16_t address = get_word(reg_h, reg_l);

                    sub(mem[address], false);
                }
                break;
            case SUI:
                {
                    uint8_t operand = get_imm();
                    sub(operand, false);
                }
                break;
            case XCHG:
                {
                    uint8_t tmp = reg_h;
                    reg_h = reg_d;
                    reg_d = tmp;

                    tmp = reg_l;
                    reg_l = reg_e;
                    reg_e = tmp;
                }
                break;
            case XRA_A:
                {
                    xor(reg_a, reg_a);
                }
                break;
            case XRA_B:
                {
                    xor(reg_a, reg_b);
                }
                break;
            case XRA_C:
                {
                    xor(reg_a, reg_c);
                }
                break;
            case XRA_D:
                {
                    xor(reg_a, reg_d);
                }
                break;
            case XRA_E:
                {
                    xor(reg_a, reg_e);
                }
                break;
            case XRA_H:
                {
                    xor(reg_a, reg_h);
                }
                break;
            case XRA_L:
                {
                    xor(reg_a, reg_l);
                }
                break;
            case XRA_M:
                {
                    uint8_t operand = mem[get_word(reg_h, reg_l)];
                    xor(reg_a, operand);
                }
                break;
            case XRI:
                {
                    uint8_t operand = get_imm();
                    xor(reg_a, operand);
                }
                break;
            case XTHL:
                {
                    uint8_t tmp = mem[stack_pointer];
                    mem[stack_pointer] = reg_l;
                    reg_l = tmp;

                    tmp = mem[stack_pointer+1];
                    mem[stack_pointer+1] = reg_h;
                    reg_h = tmp;
                }
                break;
            default:
                {
                    // TODO: Handle error
                }
                break;
        }
    }

    uint8_t Processor::xor(uint8_t a, uint8_t b)
    {
        a ^= b;

        if(a < 0)
        {
            sign = true;
            zero = false;
        }
        else if(a > 0)
        {
            sign = false; 
            zero = false;
        }
        else if(a == 0)
        {
            sign = false;
            zero = true;
        }

        carry = 0;
        auxiliary_carry = 0;

        return a;
    }

    void Processor::add(int addend, bool with_carry)
    {
        if(with_carry)
        {
            uint16_t res = reg_a + addend + carry;

            if(res > 0xff)
            {
                carry = true;
            }

            // Remove higher nibble and store value to reg_a
            reg_a = (uint8_t)(res & 0x00ff);
        }
        else
        {
            reg_a += addend;
        }
    }

    void Processor::sub(uint8_t subtrahend, bool with_carry)
    {
        uint16_t res = reg_a + ~subtrahend + 1;

        reg_a = reg_a + ~subtrahend + 1;

        if(with_carry)
        {
            reg_a += carry;
        }

        if(res > 0xff)
        {
            carry = false;
        }
        else
        {
            carry = true;
        }
    }

    uint8_t Processor::or(uint8_t a, uint8_t b)
    {
        a |= b;

        if(a < 0)
        {
            sign = true;
            zero = false;
        }
        else if(a > 0)
        {
            sign = false; 
            zero = false;
        }
        else if(a == 0)
        {
            sign = false;
            zero = true;
        }

        carry = 0;
        auxiliary_carry = 0;

        return a;
    }

    void Processor::cmp(uint8_t a, uint8_t b)
    {
        if(a == b)
        {
            zero = true;
        }
        else if(a < b)
        {
            carry = 1;
        }
        else if(a > b)
        {
            carry = 0; zero = 0;
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
