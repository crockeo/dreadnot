#include "parse.hpp"
#include "token.hpp"

#include <iostream>

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

    token_t::token_t(opt_t operation, uint32_t name, uint32_t size) :
            operation(operation), name(name), size(size) { }
    token_t::token_t() { }

    // OLD VERSION of the lex function, new version below follows the
    // documentation now-present above and in parse.hpp.
    bool lex_old(trace_t& trace, istream& in)
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

    // Filling a uint32_t from an istream.
    static int fill(istream& in, uint32_t& block)
    {
        memset(&block, 0, sizeof(uint32_t));

        int op;
        for (int i = 0; i < 4; i++)
        {
            op = in.get();
            if (op < 0)
                return -1;

            block <<= 8;
            block |= op;
        }

        return 0;
    }

    bool lex(trace_t& trace, istream& in)
    {
        int op;
        while ((op = in.get()) > 0)
        {
            token_t tok;
            memset(&tok, 0, sizeof(token_t));

            if (op == 'M')
                tok.operation = MALLOC;
            else if (op == 'F')
                tok.operation = FREE;
            else
                return false;

            if (fill(in, tok.name))
                return false;

            if (tok.operation == MALLOC)
                if (fill(in, tok.size))
                    return false;
        }

        return true;
    }

    bool lex(trace_t& trace, istream& in, bool old)
    {
        if (old)
            return lex_old(trace, in);
        return lex(trace, in);
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
