#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

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
        OPERAND_HEX,
        OPERAND_DEC,
        OPERAND_OCT,
        OPERAND_BIN,
        OPERAND_LOCATION_COUNTER,
        OPERAND_ASCII_STR,
        ARITHMETIC_OP,
        UNKNOWN,
        _EOF
    };

    struct Token
    {
        int line_number;
        int col_number;
        
        std::string token_string;
        TokenType tt;

        Token* next;
        Token* prev;
    };

    struct SymbolValue
    {
        uint16_t value = 0;
        std::vector<int> references;
    };

    class Assembler
    {
        public:
            Assembler();
            Assembler(std::string& code);
            ~Assembler();

            std::vector<uint8_t> assemble();
            void tokenize();
            bool parse();
            void print_tokens();
            bool disassemble();

            std::map<uint64_t, std::string> _disassembly;
            std::vector<uint8_t> _program_instructions;
        private:
            std::string& _code;
            std::vector<std::string> _opcode_strs;
            std::vector<std::string> _directive_strs;

            Token* m_tokens;

            std::unordered_map<std::string, SymbolValue> _symbol_table;

            bool is_reg(const std::string& str) const;
            bool is_opcode(const std::string& str) const;
            bool is_directive(const std::string& str) const;
            bool is_hex_operand(const std::string& str) const;
            bool is_dec_operand(const std::string& str) const;
            bool is_oct_operand(const std::string& str) const;
            bool is_bin_operand(const std::string& str) const;
            bool is_location_counter_operand(const std::string& str) const;
            bool parse_data_byte(const Token& t, uint8_t& operand_byte);
            bool parse_data_word(const Token& t, uint16_t& operand_word);
    };
}
