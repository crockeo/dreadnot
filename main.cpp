#include <iostream>
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
    struct token_t
    {
        opt_t operation;
        int name;
        size_t length;
    };

    typedef vector<token_t> trace_t;

    trace_t lex(istream& in)
    {
        trace_t trace;

        const int line_len = 512;
        char line[line_len];

        char operation[16];
        int name;
        size_t length;
        while (in.getline(line, line_len))
        {
            if (sscanf(line, "%s %d %lu", operation, &name, &length) < 3)
            {
                break;
            }

            token_t token;
            if (strncmp(operation, "malloc", 16) == 0)
                token.operation = MALLOC;
            if (strncmp(operation, "free", 16) == 0)
                token.operation = FREE;

            token.name = name;
            token.length = length;
            
            trace.push_back(token);
        }

        return trace;
    }

    void print_trace(trace_t trace)
    {
        for (auto it = trace.begin(); it != trace.end(); it++)
        {
            switch (it->operation)
            {
            case MALLOC:
                cout << "malloc ";
                break;
            case FREE:
                cout << "free ";
                break;
            }

            cout << it->name << " " << it->length << endl;
        }
    }
}

int main()
{
    parse::trace_t trace = parse::lex(cin);

    parse::print_trace(trace);
}
