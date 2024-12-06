#include <iostream>
#include <pda/context.h>
#include <pda/parser.h>

int main(int argc, char* argv[]){
    std::cout << "Correct configured cmake!" << std::endl;

    PDAContext context;
    context = PDAParser::parse("/Users/jinwenze/Coding/2024/Automata/lab/fla-emulator/pda/test.pda");
}