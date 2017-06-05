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
    parse::trace_t trace;
    try
    {
        parse::trace_t trace = parse::lex(cin);
    } catch (runtime_error& e)
    {
        cout << "Lex error: " << e.what() << endl;
        return 1;
    }

    BrokenHeap<MallocHeap> brokenHeap;
    try
    {
        parse::execute<BrokenHeap<MallocHeap> >(brokenHeap, trace);
    } catch (runtime_error& e)
    {
        cout << "Execute error: " << e.what() << endl;
    }

    for (auto it = trace.begin(); it != trace.end(); it++)
        free(*it);
}
