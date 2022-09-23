#include "lib8085.h"
#include <iostream>

using namespace lib8085;

int main()
{
	Processor cpu = Processor();

    cpu.reg_a = 10;

    // ADD M, 5520h
    cpu.mem[0] = ADD_M;
    cpu.mem[0x5520] = 20;
    cpu.reg_h = 0x55;
    cpu.reg_l = 0x20;

    cpu.exec(1);

	std::cout << cpu;

	return 0;
}
