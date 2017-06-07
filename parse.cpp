#include "parse.hpp"

#include <iostream>

namespace parse
{
    using namespace std;

    bool lex(trace_t &trace, istream& in)
    {
        const int line_len = 256;
        char line[line_len];

        char *tok;
        while (in.getline(line, line_len))
        {
            token_t *token;
            tok = strtok(line, " ");
            if (!tok)
                return false;

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
                return false;

            tok = strtok(nullptr, " ");
            if (!tok)
                return false;

            if (sscanf(tok, "%d", &token->name) == 0)
                return false;

            if (token->operation == MALLOC)
            {
                tok = strtok(nullptr, " ");
                if (!tok)
                    return false;

                if (sscanf(tok, "%lu", &((malloc_t *)token)->size) == 0)
                    return false;
            }

            trace.push_back(token);
        }

        return true;
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
