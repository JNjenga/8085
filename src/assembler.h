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
        REG,
        OPERAND,
        UNKNOWN,
        EOF
    };

    struct Token
    {
        int line_number;
        int col_number;
        
        std::string token_string;
        TokenType tt;
    };

    struct OpcodeRule
    {
        // addressing mode
        // operand count
        int operand_count;
        std::string opcode;
    };

    class Assembler
    {
        public:
            Assembler(std::string& code);
            ~Assembler();

            void tokenize();
            void parse();
            void print_tokens();
        private:
            std::string& _code;
            std::vector<Token> _tokens;
            std::vector<std::string> _opcode_strs;
            std::vector<std::string> _directive_strs;
            size_t _current_token;

            bool is_reg(const std::string& str) const;
            bool is_opcode(const std::string& str) const;
            bool is_directive(const std::string& str) const;

            Token& next_token();
            Token& peek_token();
    };
}