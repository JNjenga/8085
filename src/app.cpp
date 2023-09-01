#include "app.h"
#include <algorithm>

bool appname8085::AppName::assemble(std::string& code)
{
    lib8085::Assembler assembler{code};

    assembler.assemble();
    std::copy(assembler._program_instructions.begin(), 
            assembler._program_instructions.end(), _cpu.mem);
    assembler.disassemble();
    _assembler._disassembly = assembler._disassembly;

    return true;
}

bool appname8085::AppName::step()
{
    _cpu.exec(1);
    return true;
}

bool appname8085::AppName::reset()
{
    _cpu.reset();
    return true;
}

bool appname8085::AppName::pause()
{
    return true;
}

const std::map<uint64_t, std::string>& appname8085::AppName::get_disassembly()
{
    return _assembler._disassembly;
}

lib8085::Processor* appname8085::AppName::get_cpu()
{
    return &_cpu;
}

appname8085::AppName::AppName() : _assembler(lib8085::Assembler(std::string("")))
{

}

appname8085::AppName::~AppName()
{

}
