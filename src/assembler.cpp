#include "assembler.h"
#include "instruction_set.h"
#include <iostream>
#include <vector>
#include <algorithm>

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

        while(t.tt != TokenType::_EOF)
        {
            if(t.tt == TokenType::OPCODE)
            {
            }

            t = next_token();
        }

    }

    bool Assembler::is_reg(const std::string& str) const
    {
        if(str.size() == 1 && (
                    str == "A" || str == "B"
                    || str == "C" || str == "D"
                    || str == "E" || str == "H"
                    || str == "L" || str == "M"))
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
}
