#include "app.h"
#include <algorithm>

bool retro85::App::assemble(std::string& code)
{
    std::cout << "Code: " << code << "\n";
    lib8085::Assembler assembler{code};

    assembler.assemble();
    std::copy(assembler._program_instructions.begin(), 
            assembler._program_instructions.end(), _cpu.mem);
    assembler.disassemble();
    _assembler._disassembly = assembler._disassembly;

    return true;
}

bool retro85::App::step()
{
    _cpu.exec(1);
    return true;
}

bool retro85::App::reset()
{
    _assembler._disassembly.clear();
    _cpu.reset();
    return true;
}

bool retro85::App::pause()
{
    return true;
}

const std::map<uint64_t, std::string>& retro85::App::get_disassembly()
{
    return _assembler._disassembly;
}

lib8085::Processor* retro85::App::get_cpu()
{
    return &_cpu;
}

retro85::App::App() : _assembler(lib8085::Assembler(std::string("")))
{

}

retro85::App::~App()
{

}
