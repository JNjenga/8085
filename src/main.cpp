#include "lib8085.h"
#include "assembler.h"
#include <iostream>

using namespace lib8085;

int main()
{
    std::string code = "   MOV A  ,  B ;This is a comment\nLabel:ROT C\n";
    Assembler asmb(code);
    asmb.tokenize();
    asmb.print_tokens();

	return 0;
}
