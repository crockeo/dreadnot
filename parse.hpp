#ifndef _DREADNOT_PARSE_H_
#define _DREADNOT_PARSE_H_

#include <exception>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>

#include "failure_list.hpp"
#include "token.hpp"

namespace parse
{
    using namespace std;

    /* Grammar Documentation
     *
     * OLD:
     *   The old grammar is built off of relatively complex human-readable
     *   string parsing; we use two tokens, 'malloc' and 'free' to denote an
     *   operation, and on those we parameterize it with a name and, for malloc,
     *   a size.
     *
     * NEW:
     *   In the new scheme, we instead opt to rebalance readability and space,
     *   favoring the latter over the former. We use single-character indicators
     *   (rather than bits, for ease of parsing), 'M' and 'F', and follow with
     *   either 8 bytes (name and size) for malloc, or 4 bytes (name) for free.
     */

    typedef vector<token_t> trace_t;
    typedef map<int, void *> state_t;

    bool lex_old(trace_t& trace, istream& in);
    bool lex(trace_t& trace, istream& in);
    bool lex(trace_t& trace, istream& in, bool old);

    void print_trace(trace_t trace);

    bool validate_state(const state_t& state);

    template <typename allocator>
    bool execute(allocator& alloc, i_failure_list_t *failure_list, trace_t trace)
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
                auto cit = chunks.find(it->name);
                if (cit == chunks.end())
                    return false;

                alloc.free(cit->second);
                chunks.erase(cit);
            }

            if (failure_list->check_opt(*it))
                abort();
        }

        return true;
    }
}

#endif
