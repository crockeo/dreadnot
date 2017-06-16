#ifndef _DREADNOT_PARSE_H_
#define _DREADNOT_PARSE_H_

#include <exception>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>

namespace parse
{
    using namespace std;

    enum opt_t { MALLOC, FREE };

    struct token_t
    {
        opt_t operation;
        uint32_t name;
        uint32_t size;
    };

    typedef vector<token_t> trace_t;
    typedef map<int, void *> state_t;

    bool lex(trace_t &trace, istream& in);

    void print_trace(trace_t trace);

    bool validate_state(const state_t& state);

    template <typename allocator>
    bool execute(allocator& alloc, trace_t trace)
    {
        state_t chunks;

        for (auto it = trace.begin(); it != trace.end(); it++)
        {
            if (it->operation == MALLOC)
            {
                void *data = alloc.malloc(it->size);
                memset(data, it->name, it->size);
                chunks.insert(pair<int, void *>(it->name, data));

                if (!validate_state(chunks))
                    abort(); // TODO: Expand this.
            } else if (it->operation == FREE)
            {
                // TODO: Ensure fuzzer only generates matching frees.
                auto cit = chunks.find(it->name);
                if (cit == chunks.end())
                    return false;

                void *data = chunks[it->name];
                alloc.free(data);
                chunks.erase(cit);
            }
        }

        return true;
    }
}

#endif
