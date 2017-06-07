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
        int name;
    };

    struct malloc_t : public token_t
    {
        size_t size;
    };

    struct free_t : public token_t { };

    typedef vector<token_t *> trace_t;
    typedef map<int, void *> state_t;

    trace_t lex(istream& in) throw(runtime_error);

    void print_trace(trace_t trace);

    bool validate_state(const state_t& state);

    template <typename allocator>
    void execute(allocator& alloc, trace_t trace) throw(runtime_error)
    {
        state_t chunks;

        for (auto it = trace.begin(); it != trace.end(); it++)
        {
            token_t *curr = *it;

            if (curr->operation == MALLOC)
            {
                void *data = alloc.malloc(((malloc_t *)curr)->size);
                memset(data, curr->name, ((malloc_t *)curr)->size);
                chunks.insert(pair<int, void *>(curr->name, data));

                if (!validate_state(chunks))
                    abort(); // TODO: Expand this.
            } else if (curr->operation == FREE)
            {
                // TODO: Ensure AFL only creates matching frees.
                auto cit = chunks.find(curr->name);
                if (cit == chunks.end())
                    throw runtime_error("Attempting to free a non-existant chunk.");

                void *data = chunks[curr->name];
                alloc.free(data);
                chunks.erase(cit);
            }
        }
    }
}

#endif
