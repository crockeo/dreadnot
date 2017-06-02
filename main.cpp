#include <iostream>
#include <cstring>
#include <vector>

#include "heap-layers/heaplayers.h"

using namespace std;
using namespace HL;

enum opt_t { MALLOC, FREE };

template <typename SuperHeap>
class BrokenHeap : public SuperHeap
{
private:
    constexpr static int actions_len = 6;
    const opt_t actions[actions_len] {
        MALLOC,
        MALLOC,
        FREE,
        MALLOC,
        FREE,
        FREE
    };

    int pos = 0;

    void check_case(opt_t action)
    {
        if (actions[pos] == action)
        {
            if (++pos >= actions_len)
                abort();
        } else
            pos = 0;
    }

public:
    inline void *malloc(size_t size)
    {
        void *ptr = SuperHeap::malloc(size);
        if (ptr == nullptr)
            return nullptr;

        check_case(opt_t::MALLOC);

        return ptr;
    }

    inline void free(void *ptr)
    {
        SuperHeap::free(ptr);
        check_case(opt_t::FREE);
    }
};

namespace parse
{
    //struct token_t
    //{
        //opt_t operation;
        //int name;
        //size_t length;
    //};

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

    trace_t lex(istream& in) throw(runtime_error)
    {
        trace_t trace;

        const int line_len = 256;
        char line[line_len];

        char *tok;
        while (in.getline(line, line_len))
        {
            token_t *token;
            tok = strtok(line, " ");
            if (!tok)
                throw runtime_error("No operation token.");

            if (strcmp(tok, "malloc") == 0)
            {
                token = new malloc_t();
                token->operation = MALLOC;
            }
            else if (strcmp(tok, "free") == 0)
            {
                token = new free_t();
                token->operation = FREE;
            }
            else
                throw runtime_error("Improper operation token.");

            tok = strtok(nullptr, " ");
            if (!tok)
                throw runtime_error("No name token.");

            if (sscanf(tok, "%d", &token->name) == 0)
                throw runtime_error("Improper name token.");

            if (token->operation == MALLOC)
            {
                tok = strtok(nullptr, " ");
                if (!tok)
                    throw runtime_error("No size token.");

                if (sscanf(tok, "%lu", &((malloc_t *)token)->size) == 0)
                    throw runtime_error("Improper size token.");
            }

            trace.push_back(token);
        }

        return trace;
    }

    void print_trace(trace_t trace)
    {
        for (auto it = trace.begin(); it != trace.end(); it++)
        {
            token_t *curr = *it;
            switch (curr->operation)
            {
            case MALLOC:
                cout << "malloc ";
                break;
            case FREE:
                cout << "free ";
                break;
            }

            cout << curr->name;
            if (curr->operation == MALLOC)
                cout << " " << ((malloc_t *)curr)->size;
            cout << endl;
        }
    }

    bool validate_state(const state_t& state)
    {
        for (auto it = state.begin(); it != state.end(); it++)
        {
            // TODO: Perform validation.
        }

        return true;
    }

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
