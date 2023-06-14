#include "assembler.h"
#include "instruction_set.h"
#include <iostream>
#include <vector>
#include <algorithm>

namespace lib8085
{
    Assembler::Assembler(std::string& code) : _code(code)
    {
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
        bool token_complete = false;

        for(size_t i = 0; i < _code.size(); i ++)
        {
            c = _code[i];
            col_number++;

            if(token_complete)
            {
                t.line_number = line_number;
                t.col_number = token_col_number;
                _tokens.push_back(t);
                token_complete = false;

                t.token_string = "";
                token_col_number = col_number;
            }


            if(is_comment)
            {
                if(c == '\n')
                {
                    is_comment = false;
                    line_number ++;

                    t.line_number = line_number;
                    t.col_number = token_col_number;
                    _tokens.push_back(t);
                    token_complete = false;

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
                token_complete = false;

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
                    token_complete = false;

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
    }

    void Assembler::parse()
    {
        // TODO: Implement
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
}
