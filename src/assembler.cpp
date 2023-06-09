#include "assembler.h"
#include <iostream>

namespace lib8085
{
    Assembler::Assembler(std::string& code) : _code(code)
    {
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
                    t.tt = TokenType::COMMENT;
                    _tokens.push_back(t);

                    t.token_string = "";
                    line_number ++;
                    token_col_number = 0;
                    col_number = 0;
                }
                else
                {
                    t.token_string += c;
                }

                continue;
            }
            else if(c == ':')
            {
                t.line_number = line_number;
                t.col_number = token_col_number;
                t.tt = TokenType::LABEL;
                _tokens.push_back(t);

                t.token_string = "";
                token_col_number = col_number;
            }
            else if(c == ' ' || c == ',' || c == '\n' ||  c == '\t' || c == '\r' || c == ';')
            {

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

            if(c == ';')
            {
                is_comment = true;
                token_col_number = col_number;
            }

            if(c == '\n')
            {
                line_number ++;
                col_number = 0;
            }
        }
    }
}
