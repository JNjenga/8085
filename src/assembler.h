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
        _EOF
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
            std::vector<uint8_t> _program_instructions;

            bool is_reg(const std::string& str) const;
            bool is_opcode(const std::string& str) const;
            bool is_directive(const std::string& str) const;

            Token& next_token();
            Token& peek_token();
            bool parse_data_byte(const Token& t, uint8_t& operand_byte);
            bool parse_data_word(const Token& t, uint16_t& operand_word);
    };
}
