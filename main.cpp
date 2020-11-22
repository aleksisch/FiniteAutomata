#include <iostream>
#include <fstream>
#include "Automata.h"

int main(int argc, char** argv) {
    const char* help_msg = "help: -h\n"
                           "build addition: -a\n"
                           "./a.out filename [-a]\n"
                           "Format in file:\n"
                           "S\n"
                           "F\n"
                           "f1 f2 f3 ...\n"
                           "A\n"
                           "s1 s2 s3 ...\n"
                           "N M\n"
                           "s t c\n"
                           "...\n"
                           "S - start vertex.\n"
                           "F - number of finish states\n"
                           "f1 f2 f3 ... - finish states\n"
                           "A - number of letter in alphabet\n"
                           "s1 s2 s3 ... - letter, separated by space\n"
                           "N M - number of vertex and edges\n"
                           "s t c - start vertex, end vertex, letter\n";

    bool is_addition = false;
    if (argv[1][0] == '-' && argv[1][1] == 'h') {
        std::cout << help_msg;
        return 0;
    }
    if (argc == 3 && argv[2][0] == '-' && argv[3][1] == 'a') {
        is_addition = true;
    }

    std::ifstream inAutomataFile(argv[1]);

    Automata NKA;
    inAutomataFile >> NKA;

    if (is_addition) {
        NKA.Inverse();
    }
    NKA.CreateDKA();
    NKA.MakeFull();
    NKA.Minimize();
    std::cout << NKA;
    return 0;
}
