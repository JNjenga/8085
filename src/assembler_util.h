#pragma once
#include "instruction_set.h"

#include <string>
#include <unordered_map>

namespace lib8085
{

    struct OpcodeData
    {
        std::string str;
        InstructionSet opcode;
        int operand_count;

        int operand_size;
    };

    class AssemblerUtil
    {
        public:
            /*
             * Returns a map that maps the opcode(enum) with what it expects as input
             * This map is to be used for disassembley
             *
             */
            static std::unordered_map<InstructionSet, OpcodeData> get_instraction_data_map();

            /*
             * Returns a map that maps the opcode(string) with what it expects as input
             * This map is to be used for assembley
             *
             */
            static std::unordered_map<std::string, OpcodeData> get_instraction_str_data_map();
    };
}

