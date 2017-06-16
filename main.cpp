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

int main(int argc, char **argv)
{
    parse::trace_t trace;
    if (argc == 2 && strcmp(argv[1], "fuzzmode") == 0)
    {
        istringstream input_stream(program_input);
        if (!parse::lex(trace, input_stream))
            return 1;
    } else {
        if (!parse::lex(trace, cin, true)) // Using the old version for now.
            return 1;
    }

    BrokenHeap<MallocHeap> brokenHeap;
    if (!parse::execute<BrokenHeap<MallocHeap>>(brokenHeap, trace))
    { }
}
