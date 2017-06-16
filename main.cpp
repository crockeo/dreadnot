#include <iostream>
#include <cstring>
#include <sstream>

#include "heap-layers/heaplayers.h"

#include "parse.hpp"
#include "heap.hpp"

using namespace parse;
using namespace heap;
using namespace std;
using namespace HL;

char program_input[4096];

void print_correct_usage()
{
    cout << "Correct usage:" << endl
         << "  dreadnot [fuzzmode]" << endl
         << "    dreadnot - Reads input from stdin, parses out input into alloc\
 and free calls to test a heap." << endl
         << "    fuzzmode - Used for FuzzBALL; provide a symblic block to the\
 location of program_input in main.cpp." << endl;
}

int main(int argc, char **argv)
{
    parse::trace_t trace;
    if (argc == 2)
    {
        if (strcmp(argv[1], "fuzzmode") == 0)
        {
            istringstream input_stream(program_input);
            if (!parse::lex(trace, input_stream))
                return 1;
        } else
            print_correct_usage();
    } else {
        if (!parse::lex(trace, cin, true)) // Using the old version for now.
            return 1;
    }

    BrokenHeap<MallocHeap> brokenHeap;
    if (!parse::execute<BrokenHeap<MallocHeap>>(brokenHeap, trace))
    { }
}
