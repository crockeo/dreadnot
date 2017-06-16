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
            token_t token;
            memset(&token, 0, sizeof(token_t));

            tok = strtok(line, " ");
            if (!tok)
                return false;

            if (strcmp(tok, "malloc") == 0)
                token.operation = MALLOC;
            else if (strcmp(tok, "free") == 0)
                token.operation = FREE;
            else
                return false;

            tok = strtok(nullptr, " ");
            if (!tok)
                return false;

            if (sscanf(tok, "%d", &token.name) == 0)
                return false;

            if (token.operation == MALLOC)
            {
                tok = strtok(nullptr, " ");
                if (!tok)
                    return false;

                if (sscanf(tok, "%d", &token.size) == 0)
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
            switch (it->operation)
            {
            case MALLOC:
                cout << "malloc ";
                break;
            case FREE:
                cout << "free ";
                break;
            }

            cout << it->name;
            if (it->operation == MALLOC)
                cout << " " << it->size;
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
