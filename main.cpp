#include <iostream>
#include <cstring>

#include "heap-layers/heaplayers.h"

#include "parse.hpp"
#include "heap.hpp"

using namespace parse;
using namespace heap;
using namespace std;
using namespace HL;

int main()
{
#ifdef __AFL_LOOP
    while (__AFL_LOOP(1000))
    {
#endif
        parse::trace_t trace;
        if (!parse::lex(trace, cin))
            return 1;

        BrokenHeap<MallocHeap> brokenHeap;
        if (!parse::execute<BrokenHeap<MallocHeap>>(brokenHeap, trace))
        { }
#ifdef __AFL_LOOP
    }
#endif
}
