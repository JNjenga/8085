#include "assembler.h"
#include "instruction_set.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

namespace lib8085
{
    Assembler::Assembler(std::string& code) : _code(code), _current_token(0)
    {
        _opcode_strs = { "ACI" , "ADC" , "ADD" , "ADI" , "ANA" , "ANI" , "CALL" , "CC" , "CM," , "CMA" , "CMC" , "CMP" , "CNC" , "CNZ" , "CP," , "CPE" , "CPI" , "CPO" , "CZ," , "DAA" , "DAD" , "DCR" , "DCX" , "DI," , "EI," , "HLT" , "IN," , "INR" , "INX" , "JC," , "JM," , "JMP" , "JNC" , "JNZ" , "JP," , "JPE" , "JPO" , "JZ," , "LDA" , "LDAX" , "LHLD" , "LXI" , "MOV" , "MVI" , "NOP" , "ORA" , "ORI" , "OUT" , "PCHL" , "POP" , "PUSH" , "RAL" , "RAR" , "RC," , "RET" , "RIM" , "RLC" , "RM," , "RNC" , "RNZ" , "RP," , "RPE" , "RPO" , "RRC" , "RST_0" , "RST_1" , "RST_2" , "RST_3" , "RST_4" , "RST_5" , "RST_6" , "RST_7" , "RZ" , "SBB" , "SBI" , "SHLD" , "SIM" , "SPH" , "STA" , "STAX" , "STC" , "SUB" , "SUI" , "XCHG" , "XRA" , "XRI" , "XTHL" };
        // TODO: Initialize _opcode_strs
        // TODO: Initialize _directive_strs
    }

    Assembler::~Assembler(){}

    void Assembler::print_tokens()
    {
        for(auto& t : _tokens)
        {
            std::cout << "tt: " << t.tt << ", str: " << t.token_string << ", line: " << t.line_number << ":" << t.col_number << std::endl;
        }
    }

    void Assembler::tokenize()
    {
        Token t;
        char c;
        bool is_comment = false;
        int line_number = 1;
        int col_number = 0;
        int token_col_number = 0;

        for(size_t i = 0; i < _code.size(); i ++)
        {
            c = _code[i];
            col_number++;

            if(is_comment)
            {
                if(c == '\n')
                {
                    is_comment = false;
                    line_number ++;

                    t.line_number = line_number;
                    t.col_number = token_col_number;
                    _tokens.push_back(t);

                    t.token_string = "";
                    token_col_number = col_number;
                }
                else
                {
                    t.token_string += c;
                }

                continue;
            }
            else if(c == ':')
            {
                t.tt = TokenType::LABEL;
                t.line_number = line_number;
                t.col_number = token_col_number;
                _tokens.push_back(t);

                t.token_string = "";
                token_col_number = col_number;

                // Todo: Store in symbol table
             }
             else if(c == ' ' || c == ',' || c == '\n' ||  c == '\t' || c == '\r' || c == ';')
             {
                 if(t.token_string.size() > 0)
                 {
                     if(is_opcode(t.token_string))
                     {
                         t.tt = TokenType::OPCODE;
                    }
                    else if(is_reg(t.token_string))
                    {
                        t.tt = TokenType::REG;
                        t.token_string[0] = std::toupper(t.token_string[0]);
                    }
                    else if(is_directive(t.token_string))
                    {
                        t.tt = TokenType::NAME;
                    }
                    else
                    {
                        t.tt = TokenType::UNKNOWN;
                    }

                    t.line_number = line_number;
                    t.col_number = token_col_number;
                    _tokens.push_back(t);

                    t.token_string = "";
                    token_col_number = col_number;
                }
            }
            else
            {
                t.token_string += c;
            }

            if(c == ';')
            {
                is_comment = true;
                t.tt = TokenType::COMMENT;
                token_col_number = col_number;
            }

            if(c == '\n')
            {
                is_comment = false;
                line_number ++;
                col_number = 0;
            }
        }

        t.line_number = line_number;
        t.col_number = 0;
        t.token_string = "_EOF";
        t.tt = TokenType::_EOF;

        _tokens.push_back(t);
    }

    void Assembler::parse()
    {
        if(_tokens.size() == 0)
        {
            return;
        }

        Token t = _tokens[0];
        std::string tstring;
        Token src_token;
        uint8_t operand_byte;
        uint16_t operand_word;

        while(t.tt != TokenType::_EOF)
        {
            tstring = t.token_string;
            if(t.tt == TokenType::OPCODE)
            {
                if(tstring == "ACI")
                {
                    src_token = next_token();

                    if(parse_data_byte(src_token, operand_byte))
                    {
                        std::cout << "Operand value: " << (int)operand_byte << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::ACI);
                    _program_instructions.push_back(operand_byte);
                }
                else if(tstring == "ADC")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "A")
                        opcode = InstructionSet::ADC_A;
                    else if(src_token.token_string == "B")
                        opcode = InstructionSet::ADC_B;
                    else if(src_token.token_string == "C")
                        opcode = InstructionSet::ADC_C;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::ADC_D;
                    else if(src_token.token_string == "E")
                        opcode = InstructionSet::ADC_E;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::ADC_H;
                    else if(src_token.token_string == "L")
                        opcode = InstructionSet::ADC_L;
                    else
                        opcode = InstructionSet::ADC_M;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "ADD")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "A")
                        opcode = InstructionSet::ADD_A;
                    else if(src_token.token_string == "B")
                        opcode = InstructionSet::ADD_B;
                    else if(src_token.token_string == "C")
                        opcode = InstructionSet::ADD_C;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::ADD_D;
                    else if(src_token.token_string == "E")
                        opcode = InstructionSet::ADD_E;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::ADD_H;
                    else if(src_token.token_string == "L")
                        opcode = InstructionSet::ADD_L;
                    else
                        opcode = InstructionSet::ADD_M;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "ANI")
                {
                    src_token = next_token();

                    if(parse_data_byte(src_token, operand_byte))
                    {
                        std::cout << "Operand value: " << (int)operand_byte << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::ANI);
                    _program_instructions.push_back(operand_byte);
                }
                else if(tstring == "CALL")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CALL);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CC")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CC);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CM")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CM);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CMA")
                {
                    _program_instructions.push_back(InstructionSet::CMA);
                }
                else if(tstring == "CMC")
                {
                    _program_instructions.push_back(InstructionSet::CMC);
                }
                else if(tstring == "CMP")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "A")
                        opcode = InstructionSet::CMP_A;
                    else if(src_token.token_string == "B")
                        opcode = InstructionSet::CMP_B;
                    else if(src_token.token_string == "C")
                        opcode = InstructionSet::CMP_C;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::CMP_D;
                    else if(src_token.token_string == "E")
                        opcode = InstructionSet::CMP_E;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::CMP_H;
                    else if(src_token.token_string == "L")
                        opcode = InstructionSet::CMP_L;
                    else
                        opcode = InstructionSet::CMP_M;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "CNC")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CNC);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CNZ")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CNZ);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CP")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CP);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CPE")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CPE);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CPI")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CPI);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CPO")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CPO);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "CZ")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::CZ);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "DAA")
                {
                    _program_instructions.push_back(InstructionSet::DAA);
                }
                else if(tstring == "DAD")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "B")
                        opcode = InstructionSet::DAD_B;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::DAD_D;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::DAD_H;
                    else
                        opcode = InstructionSet::DAD_SP;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "DCR")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "A")
                        opcode = InstructionSet::DCR_A;
                    else if(src_token.token_string == "B")
                        opcode = InstructionSet::DCR_B;
                    else if(src_token.token_string == "C")
                        opcode = InstructionSet::DCR_C;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::DCR_D;
                    else if(src_token.token_string == "E")
                        opcode = InstructionSet::DCR_E;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::DCR_H;
                    else if(src_token.token_string == "L")
                        opcode = InstructionSet::DCR_L;
                    else
                        opcode = InstructionSet::DCR_M;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "DCX")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "B")
                        opcode = InstructionSet::DCX_B;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::DCX_D;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::DCX_H;
                    else
                        opcode = InstructionSet::DCX_SP;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "DI")
                {
                    _program_instructions.push_back(InstructionSet::DI);
                }
                else if(tstring == "EI")
                {
                    _program_instructions.push_back(InstructionSet::EI);
                }
                else if(tstring == "HLT")
                {
                    _program_instructions.push_back(InstructionSet::HLT);
                }
                else if(tstring == "IN")
                {
                    src_token = next_token();

                    if(parse_data_byte(src_token, operand_byte))
                    {
                        std::cout << "Operand value: " << (int)operand_byte << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::IN);
                    _program_instructions.push_back(operand_byte);
                }
                else if(tstring == "INR")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "A")
                        opcode = InstructionSet::INR_A;
                    else if(src_token.token_string == "B")
                        opcode = InstructionSet::INR_B;
                    else if(src_token.token_string == "C")
                        opcode = InstructionSet::INR_C;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::INR_D;
                    else if(src_token.token_string == "E")
                        opcode = InstructionSet::INR_E;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::INR_H;
                    else if(src_token.token_string == "L")
                        opcode = InstructionSet::INR_L;
                    else
                        opcode = InstructionSet::INR_M;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "INX")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "B")
                        opcode = InstructionSet::INX_B;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::INX_D;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::INX_H;
                    else
                        opcode = InstructionSet::INX_SP;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "JC")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::JC);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "JM")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::JM);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "JMP")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::JMP);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "JNC")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::JNC);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "JP")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::JP);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "JPE")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::JPE);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "JPO")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::JPO);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "JZ")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::JZ);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "LDA")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::LDA);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "LDAX")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "B")
                        opcode = InstructionSet::LDAX_B;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::LDAX_D;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "LHLD")
                {
                    src_token = next_token();

                    if(parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Operand value: " << (int)operand_word << std::endl;;
                    }
                    else
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::LHLD);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "LXI")
                {
                    dest_token = next_token();

                    if(dest_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << dest_token.token_string << "\' at line "
                            << dest_token.line_number << ":" << dest_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    src_token = next_token();

                    if(!parse_data_word(src_token, operand_word))
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(dest_token.token_string == "B")
                        opcode = InstructionSet::LXI_B;
                    else if(dest_token.token_string == "D")
                        opcode = InstructionSet::LXI_D;
                    else if(dest_token.token_string == "H")
                        opcode = InstructionSet::LXI_H;
                    else
                        opcode = InstructionSet::LXI_SP;

                    _program_instructions.push_back(opcode);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "MOV")
                {
                    dest_token = next_token();

                    if(dest_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << dest_token.token_string << "\' at line "
                            << dest_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    const std::string& reg1 = dest_token.token_string;
                    const std::string& reg2 = src_token.token_string;

                    uint8_t opcode;
                    if(reg1 == reg2)
                        opcode = InstructionSet::NOP;
                    else if(reg1 == "A" && reg2 == "B")
                        opcode = InstructionSet::MOV_A_B;
                    else if(reg1 == "A" && reg2 == "C")
                        opcode = InstructionSet::MOV_A_C;
                    else if(reg1 == "A" && reg2 == "D")
                        opcode = InstructionSet::MOV_A_D;
                    else if(reg1 == "A" && reg2 == "E")
                        opcode = InstructionSet::MOV_A_E;
                    else if(reg1 == "A" && reg2 == "H")
                        opcode = InstructionSet::MOV_A_H;
                    else if(reg1 == "A" && reg2 == "L")
                        opcode = InstructionSet::MOV_A_L;
                    else if(reg1 == "A" && reg2 == "M")
                        opcode = InstructionSet::MOV_A_M;
                    else if(reg1 == "B" && reg2 == "A")
                        opcode = InstructionSet::MOV_B_A;
                    else if(reg1 == "B" && reg2 == "C")
                        opcode = InstructionSet::MOV_B_C;
                    else if(reg1 == "B" && reg2 == "D")
                        opcode = InstructionSet::MOV_B_D;
                    else if(reg1 == "B" && reg2 == "E")
                        opcode = InstructionSet::MOV_B_E;
                    else if(reg1 == "B" && reg2 == "H")
                        opcode = InstructionSet::MOV_B_H;
                    else if(reg1 == "B" && reg2 == "L")
                        opcode = InstructionSet::MOV_B_L;
                    else if(reg1 == "B" && reg2 == "M")
                        opcode = InstructionSet::MOV_B_M;
                    else if(reg1 == "C" && reg2 == "A")
                        opcode = InstructionSet::MOV_C_A;
                    else if(reg1 == "C" && reg2 == "B")
                        opcode = InstructionSet::MOV_C_B;
                    else if(reg1 == "C" && reg2 == "D")
                        opcode = InstructionSet::MOV_C_D;
                    else if(reg1 == "C" && reg2 == "E")
                        opcode = InstructionSet::MOV_C_E;
                    else if(reg1 == "C" && reg2 == "H")
                        opcode = InstructionSet::MOV_C_H;
                    else if(reg1 == "C" && reg2 == "L")
                        opcode = InstructionSet::MOV_C_L;
                    else if(reg1 == "C" && reg2 == "M")
                        opcode = InstructionSet::MOV_C_M;
                    else if(reg1 == "D" && reg2 == "A")
                        opcode = InstructionSet::MOV_D_A;
                    else if(reg1 == "D" && reg2 == "B")
                        opcode = InstructionSet::MOV_D_B;
                    else if(reg1 == "D" && reg2 == "C")
                        opcode = InstructionSet::MOV_D_C;
                    else if(reg1 == "D" && reg2 == "E")
                        opcode = InstructionSet::MOV_D_E;
                    else if(reg1 == "D" && reg2 == "H")
                        opcode = InstructionSet::MOV_D_H;
                    else if(reg1 == "D" && reg2 == "L")
                        opcode = InstructionSet::MOV_D_L;
                    else if(reg1 == "D" && reg2 == "M")
                        opcode = InstructionSet::MOV_D_M;
                    else if(reg1 == "E" && reg2 == "A")
                        opcode = InstructionSet::MOV_E_A;
                    else if(reg1 == "E" && reg2 == "B")
                        opcode = InstructionSet::MOV_E_B;
                    else if(reg1 == "E" && reg2 == "C")
                        opcode = InstructionSet::MOV_E_C;
                    else if(reg1 == "E" && reg2 == "D")
                        opcode = InstructionSet::MOV_E_D;
                    else if(reg1 == "E" && reg2 == "H")
                        opcode = InstructionSet::MOV_E_H;
                    else if(reg1 == "E" && reg2 == "L")
                        opcode = InstructionSet::MOV_E_L;
                    else if(reg1 == "E" && reg2 == "M")
                        opcode = InstructionSet::MOV_E_M;
                    else if(reg1 == "H" && reg2 == "A")
                        opcode = InstructionSet::MOV_H_A;
                    else if(reg1 == "H" && reg2 == "B")
                        opcode = InstructionSet::MOV_H_B;
                    else if(reg1 == "H" && reg2 == "C")
                        opcode = InstructionSet::MOV_H_C;
                    else if(reg1 == "H" && reg2 == "D")
                        opcode = InstructionSet::MOV_H_D;
                    else if(reg1 == "H" && reg2 == "E")
                        opcode = InstructionSet::MOV_H_E;
                    else if(reg1 == "H" && reg2 == "L")
                        opcode = InstructionSet::MOV_H_L;
                    else if(reg1 == "H" && reg2 == "M")
                        opcode = InstructionSet::MOV_H_M;
                    else if(reg1 == "L" && reg2 == "A")
                        opcode = InstructionSet::MOV_L_A;
                    else if(reg1 == "L" && reg2 == "B")
                        opcode = InstructionSet::MOV_L_B;
                    else if(reg1 == "L" && reg2 == "C")
                        opcode = InstructionSet::MOV_L_C;
                    else if(reg1 == "L" && reg2 == "D")
                        opcode = InstructionSet::MOV_L_D;
                    else if(reg1 == "L" && reg2 == "E")
                        opcode = InstructionSet::MOV_L_E;
                    else if(reg1 == "L" && reg2 == "H")
                        opcode = InstructionSet::MOV_L_H;
                    else if(reg1 == "L" && reg2 == "M")
                        opcode = InstructionSet::MOV_L_M;
                    else if(reg1 == "M" && reg2 == "A")
                        opcode = InstructionSet::MOV_M_A;
                    else if(reg1 == "M" && reg2 == "B")
                        opcode = InstructionSet::MOV_M_B;
                    else if(reg1 == "M" && reg2 == "C")
                        opcode = InstructionSet::MOV_M_C;
                    else if(reg1 == "M" && reg2 == "D")
                        opcode = InstructionSet::MOV_M_D;
                    else if(reg1 == "M" && reg2 == "E")
                        opcode = InstructionSet::MOV_M_E;
                    else if(reg1 == "M" && reg2 == "H")
                        opcode = InstructionSet::MOV_M_H;
                    else if(reg1 == "M" && reg2 == "L")
                        opcode = InstructionSet::MOV_M_L;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "MVI")
                {
                    dest_token = next_token();

                    if(dest_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << dest_token.token_string << "\' at line "
                            << dest_token.line_number << ":" << dest_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    src_token = next_token();

                    if(!parse_data_byte(src_token, operand_byte))
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(dest_token.token_string == "A")
                        opcode = InstructionSet::MVI_A;
                    else if(dest_token.token_string == "B")
                        opcode = InstructionSet::MVI_B;
                    else if(dest_token.token_string == "C")
                        opcode = InstructionSet::MVI_C;
                    else if(dest_token.token_string == "D")
                        opcode = InstructionSet::MVI_D;
                    else if(dest_token.token_string == "E")
                        opcode = InstructionSet::MVI_E;
                    else if(dest_token.token_string == "L")
                        opcode = InstructionSet::MVI_L;
                    else if(dest_token.token_string == "H")
                        opcode = InstructionSet::MVI_H;
                    else if(dest_token.token_string == "M")
                        opcode = InstructionSet::MVI_M;

                    _program_instructions.push_back(opcode);
                    _program_instructions.push_back(operand_byte);
                }
                else if(tstring == "NOP")
                {
                    _program_instructions.push_back(InstructionSet::NOP);
                }
                else if(tstring == "ORA")
                {
                    src_token = next_token();

                    if(src_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    uint8_t opcode;
                    if(src_token.token_string == "A")
                        opcode = InstructionSet::ORA_A;
                    else if(src_token.token_string == "B")
                        opcode = InstructionSet::ORA_B;
                    else if(src_token.token_string == "C")
                        opcode = InstructionSet::ORA_C;
                    else if(src_token.token_string == "D")
                        opcode = InstructionSet::ORA_D;
                    else if(src_token.token_string == "E")
                        opcode = InstructionSet::ORA_E;
                    else if(src_token.token_string == "H")
                        opcode = InstructionSet::ORA_H;
                    else if(src_token.token_string == "L")
                        opcode = InstructionSet::ORA_L;
                    else
                        opcode = InstructionSet::ORA_M;

                    _program_instructions.push_back(opcode);
                }
                else if(tstring == "ORI")
                {
                    src_token = next_token();

                    if(!parse_data_byte(src_token, operand_byte))
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::ORI);
                    _program_instructions.push_back(operand_byte);
                }
                else if(tstring == "OUT")
                {
                    src_token = next_token();

                    if(!parse_data_byte(src_token, operand_byte))
                    {
                        std::cout << "Invalid operand \'" << src_token.token_string << "\' at line "
                            << src_token.line_number << ":" << src_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }

                    _program_instructions.push_back(InstructionSet::OUT);
                    _program_instructions.push_back(operand_byte);
                }
                else if(tstring == "PCHL")
                {
                    _program_instructions.push_back(InstructionSet::PCHL);
                }
                else if(tstring == "POP")
                {
                    dest_token = next_token();

                    if(dest_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << dest_token.token_string << "\' at line "
                            << dest_token.line_number << ":" << dest_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }


                    uint8_t opcode;
                    if(dest_token.token_string == "B")
                        opcode = InstructionSet::POP_B;
                    else if(dest_token.token_string == "D")
                        opcode = InstructionSet::POP_D;
                    else if(dest_token.token_string == "H")
                        opcode = InstructionSet::POP_H;
                    else
                        opcode = InstructionSet::POP_PSW;

                    _program_instructions.push_back(opcode);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "PUSH")
                {
                    dest_token = next_token();

                    if(dest_token.tt != TokenType::REG)
                    {
                        std::cout << "Invalid operand \'" << dest_token.token_string << "\' at line "
                            << dest_token.line_number << ":" << dest_token.col_number << "\n"
                            << "Expected register as operand\n";
                        return;
                    }


                    uint8_t opcode;
                    if(dest_token.token_string == "B")
                        opcode = InstructionSet::PUSH_B;
                    else if(dest_token.token_string == "D")
                        opcode = InstructionSet::PUSH_D;
                    else if(dest_token.token_string == "H")
                        opcode = InstructionSet::PUSH_H;
                    else
                        opcode = InstructionSet::PUSH_PSW;

                    _program_instructions.push_back(opcode);
                    _program_instructions.push_back((operand_word >> 4) << 4);
                    _program_instructions.push_back((operand_word << 4) >> 4);
                }
                else if(tstring == "RAL")
                {
                    _program_instructions.push_back(InstructionSet::RAL);
                }
                else if(tstring == "RAR")
                {
                    _program_instructions.push_back(InstructionSet::RAR);
                }
                else if(tstring == "RC")
                {
                    _program_instructions.push_back(InstructionSet::RC);
                }
                else if(tstring == "RET")
                {
                    _program_instructions.push_back(InstructionSet::RET);
                }
                else if(tstring == "RIM")
                {
                    _program_instructions.push_back(InstructionSet::RIM);
                }
                else if(tstring == "RLC")
                {
                    _program_instructions.push_back(InstructionSet::RLC);
                }
                else if(tstring == "RM")
                {
                    _program_instructions.push_back(InstructionSet::RM);
                }
                else if(tstring == "RLC")
                {
                    _program_instructions.push_back(InstructionSet::RLC);
                }
                else if(tstring == "RNC")
                {
                    _program_instructions.push_back(InstructionSet::RNC);
                }
                else if(tstring == "RNZ")
                {
                    _program_instructions.push_back(InstructionSet::RNZ);
                }
                else if(tstring == "RP")
                {
                    _program_instructions.push_back(InstructionSet::RP);
                }
                else if(tstring == "RPE")
                {
                    _program_instructions.push_back(InstructionSet::RPE);
                }
                else if(tstring == "RPO")
                {
                    _program_instructions.push_back(InstructionSet::RPO);
                }
                else if(tstring == "RRC")
                {
                    _program_instructions.push_back(InstructionSet::RRC);
                }
            }

            t = next_token();
        }

    }

    bool Assembler::is_reg(const std::string& str) const
    {
        std::string tmp = str;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);

        if( tmp == "A" || tmp == "B"
                || tmp == "C" || tmp == "D"
                || tmp == "E" || tmp == "H"
                || tmp == "L" || tmp == "M"
                || tmp == "SP")
                {
            return true;
        }
        return false;
    }

    bool Assembler::is_directive(const std::string& str) const
    {
        if(std::binary_search(_directive_strs.begin(), _directive_strs.end(), str))
        {
            return true;
        }
        return false;
    }

    bool Assembler::is_opcode(const std::string& str) const
    {
        if(std::binary_search(_opcode_strs.begin(), _opcode_strs.end(), str))
        {
            return true;
        }
        return false;
    }

    Token& Assembler::next_token()
    {
        Token& t = peek_token();
        _current_token++;

        while(t.tt == TokenType::COMMENT);
        {
            t = peek_token();
            _current_token ++;
        }
        return t;
    }

    Token& Assembler::peek_token()
    {
        if(_current_token >= _tokens.size())
        {
            return _tokens[_tokens.size()];
        }

        return _tokens[_current_token];
    }

    bool Assembler::parse_data_byte(const Token& t, uint8_t& operand_byte)
    {
        char c;
        const std::string& ts = t.token_string;
        // try parse hex data
        // Ends with 'H'
        if(ts.back() == 'H' || ts.back() == 'h')
        {
            if(ts.size() > 3)
            {
                std::cout << "Operand size\n";
                return false;
            }

            for(size_t i = 0; i < ts.size()-1; i ++)
            {
                c = std::tolower(ts[i]);

                if(c >= '0' && c <= '9')
                {
                    c -= '0';
                }
                else if(c >= 'a' && c <= 'f')
                {
                    c = (c - 'a') + 10;
                }
                else
                {
                    std::cout << "Invalid character " << c;
                    return false;
                }

                std::cout << "C: "  << (int)c<< "\n";

                operand_byte <<= 4;
                operand_byte |= c;

            }
        }
        else
        {
            std::cout << "Invalid operand suffix\n";
            return false;
        }
        return true;
    }

    bool Assembler::parse_data_word(const Token& t, uint16_t& operand_word)
    {
        char c;
        const std::string& ts = t.token_string;
        // try parse hex data
        // Ends with 'H'
        if(ts.back() == 'H' || ts.back() == 'h')
        {
            if(ts.size() > 5)
            {
                std::cout << "Operand size\n";
                return false;
            }

            for(size_t i = 0; i < ts.size()-1; i ++)
            {
                c = std::tolower(ts[i]);

                if(c >= '0' && c <= '9')
                {
                    c -= '0';
                }
                else if(c >= 'a' && c <= 'f')
                {
                    c = (c - 'a') + 10;
                }
                else
                {
                    std::cout << "Invalid character " << c;
                    return false;
                }

                operand_word <<= 4;
                operand_word |= c;

            }
        }
        else
        {
            std::cout << "Invalid operand suffix\n";
            return false;
        }
        return true;
    }

}
