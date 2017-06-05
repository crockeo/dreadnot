#include "parse.hpp"

#include <iostream>

namespace parse
{
    using namespace std;

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
}
