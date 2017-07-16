#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

#include <vector>
#include <regex>
#include <set>

using namespace std;

namespace dreadnot_generator
{
namespace grammar
{
    // Struct to combine amount consumed and correctness of consumption.
    struct match_t
    {
        int consumed;
        bool correct;
        
        match_t(int consumed, bool correct)
        {
            this->consumed = consumed;
            this->correct = correct;
        }

        match_t()
        {
            consumed = 0;
            correct = false;
        }
    };

    // Supertype of symbols.
    struct symbol_t
    {
        enum kind_t
        {
            SINGLE,
            OPTION,
            SEQUENCE
        };

        // Matching this symbol against a string.
        virtual match_t match(string s);
    };

    // A single, terminal symbol. Can be modeled by a regular expression to
    // provide more options.
    struct single_t : symbol_t
    {
        regex expr;

        single_t(regex expr) :
                expr(expr) { }

        virtual match_t match(string s)
        {
            bool b = regex_match(s, expr);
            return match_t(b ? s.size() : 0, b);
        }
    };

    // An option symbol, or a set of other symbols derived from it.
    struct option_t : symbol_t
    {
        set<symbol_t *> options;

        option_t(set<symbol_t *> options) :
                options(options) { }

        virtual match_t match(string s)
        {
            match_t curr;
            for (auto it = options.begin(); it != options.end(); it++)
            {
                curr = (*it)->match(s);
                if (curr.correct)
                    return curr;
            }

            return match_t(0, false);
        }
    };

    // A sequence symbol, or a set of required, sequential symbols.
    struct sequence_t : symbol_t
    {
        vector<symbol_t *> sequence;

        virtual match_t match(string s)
        {
            int accum = 0;
            match_t curr;
            for (auto it = sequence.begin(); it != sequence.end(); it++)
            {
                curr = (*it)->match(s);
                accum += curr.consumed;
                if (!curr.correct)
                    break;
            }
            
            return match_t(accum, curr.correct);
        }
    };
}
}

#endif
