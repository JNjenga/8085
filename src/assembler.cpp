#include "assembler.h"
#include "instruction_set.h"
#include "assembler_util.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

namespace lib8085
{
    Assembler::Assembler() : Assembler(std::string())
    {
    }

    Assembler::Assembler(std::string& code) : _code(code), m_tokens(nullptr)
    {
        _opcode_strs = { "ACI" , "ADC" , "ADD" , "ADI" , "ANA" , "ANI" , "CALL" , "CC" , "CM," , "CMA" , "CMC" , "CMP" , "CNC" , "CNZ" , "CP," , "CPE" , "CPI" , "CPO" , "CZ," , "DAA" , "DAD" , "DCR" , "DCX" , "DI," , "EI," , "HLT" , "IN," , "INR" , "INX" , "JC," , "JM," , "JMP" , "JNC" , "JNZ" , "JP," , "JPE" , "JPO" , "JZ," , "LDA" , "LDAX" , "LHLD" , "LXI" , "MOV" , "MVI" , "NOP" , "ORA" , "ORI" , "OUT" , "PCHL" , "POP" , "PUSH" , "RAL" , "RAR" , "RC," , "RET" , "RIM" , "RLC" , "RM," , "RNC" , "RNZ" , "RP," , "RPE" , "RPO" , "RRC" , "RST_0" , "RST_1" , "RST_2" , "RST_3" , "RST_4" , "RST_5" , "RST_6" , "RST_7" , "RZ" , "SBB" , "SBI" , "SHLD" , "SIM" , "SPH" , "STA" , "STAX" , "STC" , "SUB" , "SUI" , "XCHG" , "XRA" , "XRI" , "XTHL" };
        // TODO: Initialize _opcode_strs
        // TODO: Initialize _directive_strs
    }

    void Assembler::assemble()
    {
        tokenize();

        if(!parse())
        {
            std::cout << "Parsing was not successful\n";
        };

        disassemble();
    }
    
    Assembler::~Assembler(){}

    void Assembler::print_tokens()
    {
        Token* tptr = m_tokens;
        while(tptr != nullptr)
        {
            std::cout << "tt: " << tptr->tt << ", str: " << tptr->token_string << ", line: " << tptr->line_number << ":" << tptr->col_number << std::endl;
            tptr = tptr->next;
        }
    }

    void Assembler::tokenize()
    {
        Token* t = new Token, *tnext;
        char c;
        bool is_comment = false;
        bool is_string = false;
        int line_number = 1;
        int col_number = 0;
        int token_col_number = 0;

        _symbol_table = std::unordered_map<std::string, lib8085::SymbolValue>();

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

                    t->line_number = line_number;
                    t->col_number = token_col_number;

                    tnext = new Token;
                    tnext->prev = t;
                    tnext->next = nullptr;

                    t->next = tnext;
                    t = tnext;

                    t->token_string = "";
                    token_col_number = col_number;
                }
                else
                {
                    t->token_string += c;
                }

                continue;
            }
            if(is_string)
            {
                if(c == '\n')
                {
                    std::cout << "Error: Expected closing quote character \' at " << token_col_number << "\' at line ";
                        return;
                }
                else
                {
                    t->token_string += c;
                }
            }
            else if(c == ':')
            {
                t->tt = TokenType::LABEL;
                t->line_number = line_number;
                t->col_number = token_col_number;

                tnext = new Token;
                tnext->prev = t;
                tnext->next = nullptr;

                t->next = tnext;
                t = tnext;

                _symbol_table.insert({ t->token_string, SymbolValue() });

                t->token_string = "";
                token_col_number = col_number;
            }
            // else if(c == '+' || c == '-' || c == '*' || c == '%')
            // {
            //     t->tt = TokenType::ARITHMETIC_OP;
            //     t->line_number = line_number;
            //     t->col_number = token_col_number;
            //     _tokens.push_back(t);

            //     t->token_string = "";
            //     token_col_number = col_number;
            // }
            else if(c == ' ' || c == ',' || c == '\n' ||  c == '\t' || c == '\r' || c == ';')
            {
                if(t->token_string.size() > 0)
                {
                    const std::string token_string = t->token_string;

                    if(is_opcode(token_string))
                    {
                        t->tt = TokenType::OPCODE;
                    }
                    else if(is_reg(token_string))
                    {
                        t->tt = TokenType::REG;
                        t->token_string[0] = std::toupper(token_string[0]);
                    }
                    else if(is_directive(token_string))
                    {
                        t->tt = TokenType::NAME;
                    }
                    else if(is_hex_operand(token_string))
                    {
                        t->tt = TokenType::OPERAND_HEX;
                    }
                    else if(is_dec_operand(token_string))
                    {
                        t->tt = TokenType::OPERAND_DEC;
                    }
                    else if(is_oct_operand(token_string))
                    {
                        t->tt = TokenType::OPERAND_OCT;
                    }
                    else if(is_bin_operand(token_string))
                    {
                        t->tt = TokenType::OPERAND_BIN;
                    }
                    else if(is_location_counter_operand(token_string))
                    {
                        t->tt = TokenType::OPERAND_LOCATION_COUNTER;
                    }
                    else
                    {
                        t->tt = TokenType::UNKNOWN;
                    }

                    t->line_number = line_number;
                    t->col_number = token_col_number;

                    tnext = new Token;
                    tnext->prev = t;
                    tnext->next = nullptr;

                    t->next = tnext;
                    t = tnext;

                    t->token_string = "";
                    token_col_number = col_number;
                }
            }
            else
            {
                t->token_string += c;
            }

            if(c == ';')
            {
                is_comment = true;
                t->tt = TokenType::COMMENT;
                token_col_number = col_number;
            }
            else if(c == '\'')
            {
                if(is_string)
                {
                    is_string = false;
                    t->tt = TokenType::OPERAND_ASCII_STR;
                    token_col_number = col_number;
                }
                else
                {
                    is_string = true;
                }
            }

            if(c == '\n')
            {
                is_comment = false;
                line_number ++;
                col_number = 0;
            }
        }

        t->line_number = line_number;
        t->col_number = 0;
        t->token_string = "_EOF";
        t->tt = TokenType::_EOF;

        t->next = nullptr;
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

    // TODO: If valid evaluate return value of actual string
    // This will remove the need to run a simliar thing in parse_data_byte
    bool Assembler::is_hex_operand(const std::string& str) const
    {
        char c;
        if(std::tolower(str.back()) != 'h')
        {
            return false;
        }

        size_t len = str.size();

        for(size_t i = 0; i < len-1; i ++)
        {
            c = std::tolower(str[i]);

            if((c > '9' && c < 'a') || (c < '0' || c > 'f'))
            {
                return false;
            }
        }

        return true;
    }

    // TODO: If valid evaluate return value of actual string
    // This will remove the need to run a simliar thing in parse_data_byte
    bool Assembler::is_dec_operand(const std::string& str) const
    {
        size_t len;

        if(std::tolower(str.back()) == 'd')
        {
            len = str.size() - 1;
        }
        else
        {
            len = str.size();
        }

        char c;
        for(size_t i = 0; i < len; i ++)
        {
            c = std::tolower(str[i]);

            if(c > '9' || c < '0')
            {
                return false;
            }
        }

        return true;
    }

    // TODO: If valid evaluate return value of actual string
    // This will remove the need to run a simliar thing in parse_data_byte
    bool Assembler::is_oct_operand(const std::string& str) const
    {
        char c;
        
        c = std::tolower(str.back());
        if(c != 'o' && c != 'q')
        {
            return false;
        }

        size_t len = str.size();

        for(size_t i = 0; i < len-1; i ++)
        {
            c = std::tolower(str[i]);

            if(c > '8' || c < '0')
            {
                return false;
            }
        }

        return true;
    }

    // TODO: If valid evaluate return value of actual string
    // This will remove the need to run a simliar thing in parse_data_byte
    bool Assembler::is_bin_operand(const std::string& str) const
    {
        char c;
        
        c = std::tolower(str.back());
        if(c != 'b')
        {
            return false;
        }

        size_t len = str.size();

        for(size_t i = 0; i < len-1; i ++)
        {
            c = std::tolower(str[i]);

            if(c != '0' && c != '1')
            {
                return false;
            }
        }

        return true;
    }

    // TODO: If valid evaluate return value of actual string
    // This will remove the need to run a simliar thing in parse_data_byte
    bool Assembler::is_location_counter_operand(const std::string& str) const
    {
        char c;
        
        c = std::tolower(str.front());
        if(c != '$')
        {
            return false;
        }

        // TODO: Validate str
        // Format: $+66

        return true;
    }

    bool Assembler::parse_data_byte(const Token& t, uint8_t& operand_byte)
    {
        char c;
        const std::string& ts = t.token_string;

        c = ts.back();
        // try parse hex data
        // Ends with 'H'
        if(c == 'H' || c == 'h')
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

                operand_byte <<= 4;
                operand_byte |= c;

            }
        }
        else
        {
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
                std::cout << "Operand size too big\n";
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
            // Check if it's a label or expression
            if(is_location_counter_operand(ts))
            {
                // TODO: Evaluate expression
            }
            else if(_symbol_table.find(ts) != _symbol_table.end())
            {
                SymbolValue& sv = (_symbol_table[t.token_string]);
                std::cout << "Reference " << _program_instructions.size() + 1 << "\n";
                sv.references.push_back(_program_instructions.size());

                operand_word = 0xffff;
                return true;
            }

            std::cout << "Invalid operand suffix\n";
            return false;
        }
        return true;
    }

    bool Assembler::disassemble()
    {
        std::unordered_map<lib8085::InstructionSet, OpcodeData>
            isa_opdata_map = AssemblerUtil::get_instraction_data_map();

        _disassembly = std::map<uint64_t, std::string>();

        OpcodeData opcode_data;
        uint8_t opcode;
        uint8_t operand_byte;
        uint16_t operand_word;
        std::stringstream ss;

        size_t len = _program_instructions.size();
        size_t instruction_address;
        for(size_t i = 0; i < len; i++)
        {
            instruction_address = i;
            std::stringstream ss;
            opcode = _program_instructions[i];
            std::unordered_map<lib8085::InstructionSet, OpcodeData>::const_iterator it
                = isa_opdata_map.find(static_cast<InstructionSet>(opcode));

            if(it == isa_opdata_map.end())
            {
                std::cerr << "Failed to disasseble opcode \"" << opcode << "\" at location " << i << "\n";
                return false;
            }

            opcode_data = it->second;

            ss << opcode_data.str << " ";

            if(opcode_data.operand_count == 1)
            {
                if(opcode_data.operand_size == 1)
                {
                    // TODO: Check for out of bounds ex
                    i++;
                    ss << "0x" << std::hex << (int)_program_instructions[i];
                }
                else if(opcode_data.operand_size == 2)
                {
                    operand_word = 0;
                    operand_word |= _program_instructions[i+2] << 8;
                    operand_word |= _program_instructions[i+1];
                    ss << "0x"<< std::hex << (int)operand_word;
                    i+=2;
                }
            }
            _disassembly.insert({ instruction_address, ss.str() });
        }

        return true;
    }

    bool Assembler::parse()
    {
        std::unordered_map<std::string, lib8085::OpcodeData> instruction_db = 
            AssemblerUtil::get_instraction_str_data_map();

        if(!m_tokens)
        {
            return false;
        }

        Token* t = m_tokens;
        std::string tstring;
        std::string opcode_str;
        Token* src_token;
        uint8_t operand_byte;
        uint16_t operand_word;

        while(t)
        {
            if(t->tt == TokenType::OPCODE)
            {
                tstring = t->token_string;

                Token* next_token_obj = t->next;

                if(next_token_obj && is_reg(next_token_obj->token_string))
                {
                    t = next_token_obj;

                    opcode_str = tstring + "_" + next_token_obj->token_string;

                    next_token_obj = next_token_obj->next;

                    if(next_token_obj && is_reg(next_token_obj->token_string))
                    {
                        t = next_token_obj;
                        opcode_str += "_" + next_token_obj->token_string;
                    }
                }
                else
                {
                    opcode_str = tstring;
                }

                std::unordered_map<std::string, OpcodeData>::const_iterator it
                    = instruction_db.find(opcode_str);

                if(it == instruction_db.end())
                {
                    std::cout << "Invalid instruction combination\'" << opcode_str << "\' at line "
                        << t->line_number << ":" << t->col_number << "\n";
                    return false;
                }

                std::cout << "\'" << opcode_str << "\' " << "Opcode found\n";

                _program_instructions.push_back(it->second.opcode);

                if(it->second.operand_count == 1)
                {
                    src_token = t->next;

                    if(!src_token)
                    {
                        std::cout << "Expected operand at line "
                            << t->line_number << ":" << t->col_number << "\n";
                        return false;
                    }

                    if(it->second.operand_size == 1)
                    {
                        if(parse_data_byte(*src_token, operand_byte))
                        {
                            std::cout << "Operand value: " << (int)operand_byte << std::endl;;
                        }
                        else
                        {
                            std::cout << "Invalid operand \'" << src_token->token_string << "\' at line "
                                << src_token->line_number << ":" << src_token->col_number << "\n";
                            return false;
                        }

                        _program_instructions.push_back(operand_byte);

                    }
                    else if(it->second.operand_size == 2)
                    {
                        if(parse_data_word(*src_token, operand_word))
                        {
                            std::cout << "Operand value: " << (int)operand_word << std::endl;;
                        }
                        else
                        {
                            std::cout << "Invalid operand \'" << src_token->token_string << "\' at line "
                                << src_token->line_number << ":" << src_token->col_number << "\n";
                            return false;
                        }

                        _program_instructions.push_back((operand_word & 0x00ff));
                        _program_instructions.push_back((operand_word & 0xff00) >> 8);
                    }
                }
            }
            else if(t->tt == TokenType::LABEL)
            {
                // std::cout << "Label: \"" << t->token_string << "\" = " << _program_instructions.size() << std::endl;
                SymbolValue& sv = (_symbol_table[t->token_string]);
                sv.value = (uint16_t)_program_instructions.size();
            }

            t = t->next; 
        }

        // Update label references
        for(auto& it: _symbol_table)
        {
            SymbolValue& sv = it.second;
            for(auto& ref : it.second.references)
            {
                // NOTE: Big endian
                // 0x0002 will appear 0x2000
                _program_instructions[ref] = (sv.value & 0x00ff);
                _program_instructions[ref+1] = (sv.value & 0xff00) >> 8;
            }
        }
        return true;
    }
}
