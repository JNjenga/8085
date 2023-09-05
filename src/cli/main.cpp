#include "../assembler.h"

#include <iostream>
#include <fstream>

void write_file(const char* path, char* data, size_t len)
{
    std::ofstream file(path);

    if(file.good() && file.is_open())
    {
        file.write(data, len);
        file.close();
    }
    else
    {
        std::cerr << "Error writing file \'" << path << "\'\n";
    }
}

std::vector<uint8_t> read_file(const char* path)
{
    std::ifstream file(path);

    file.seekg(0, std::ios_base::end);
    std::vector<uint8_t> data(file.tellg());
    file.seekg(0, std::ios_base::beg);

    std::cout << "File size: " << data.size() << "\n";
    if(file.good() && file.is_open())
    {
        file.read(reinterpret_cast<char*>(&data[0]), data.size());
        file.close();
    }
    else
    {
        std::cerr << "Error reading file \'" << path << "\'\n";
    }

    return data;
}

std::vector<uint8_t> assemble(std::string& code)
{
    lib8085::Assembler assembler(code);
    assembler.assemble();

    return assembler._program_instructions;
}

std::map<uint64_t, std::string> disassemble(std::vector<uint8_t> instructions)
{
    lib8085::Assembler assembler(std::string(""));
    assembler._program_instructions = instructions;

    std::cout << "Program size: " << assembler._program_instructions.size() << "\n";;

    assembler.disassemble();
    return assembler._disassembly;
}

void print_help()
{
    std::cout << "-a - Assemble source code\n";
    std::cout << "-d - Dissassemble program\n";
    std::cout << "-r - Run program\n";
}

int main(int argc, char* argv[])
{
    char* command = nullptr;
    while((command = *(++argv)))
    {
        if(std::string(command) == "-a")
        {
            while((command = *(++argv)))
            {
                std::cout << "Assembling file:\'" << command << "\'...";

                std::vector<uint8_t> file_bin = read_file(command);
                std::string code(file_bin.begin(), file_bin.end());
                std::vector<uint8_t> machine_code = assemble(code);

                std::cout << "Done\n";

                std::cout << "Writing to file:\'" << command << ".retro85\'...";
                std::string output_path = std::string(command) + ".retro85";
                write_file(output_path.c_str(), reinterpret_cast<char*>(machine_code.data()), machine_code.size() * sizeof(uint8_t));
                std::cout << "Done\n";
            }
            break;
        }
        else if(std::string(command) == "-d")
        {
            while((command = *(++argv)))
            {
                std::cout << "Disassembling file:\'" << command << "\'...";

                std::vector<uint8_t> file_bin = read_file(command);
                std::map<uint64_t, std::string> disasm = disassemble(file_bin);

                std::cout << "Done\n";

                for(std::map<uint64_t, std::string>::iterator it = disasm.begin(); it != disasm.end(); it++)
                {
                    std::cout << it->first << ": " << it->second << "\n";
                }
            }
        }
        else
        {
            std::cout << "Unknown command \'" << command << (command == "-a" ? "yes" : "no") << "\'\n";
            print_help();
            return -1;
        }
    }
    return 0;
}
