#pragma once
#include <string>
#include <vector>

namespace lib8085
{
    enum TokenType
    {
        OPCODE,
        LABEL,
        NAME,
        COMMENT,
        UNKNOWN
    };

    struct Token
    {
        int line_number;
        int col_number;
        
        std::string token_string;
        TokenType tt;
    };

    /*
    struct OpcodeRule
    {
        // addressing mode
        // operand count
    };
    */

    class Assembler
    {
        public:
            Assembler(std::string& code);
            ~Assembler();

            void tokenize();
            void print_tokens();
        private:
            std::string& _code;
            std::vector<Token> _tokens;
    };
}
