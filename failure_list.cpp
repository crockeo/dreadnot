#include "failure_list.hpp"

using namespace parse;

////
// count_failure_list_t
count_failure_list_t::count_failure_list_t(int maxMallocCalls, int maxFreeCalls)
{
    this->maxMallocCalls = maxMallocCalls;
    mallocCalls = 0;

    this->maxFreeCalls = maxFreeCalls;
    freeCalls = 0;
}

bool count_failure_list_t::check_opt(parse::token_t tok)
{
    if (tok.operation == parse::MALLOC)
        mallocCalls++;
    if (tok.operation == parse::FREE)
        freeCalls++;

    if (mallocCalls >= maxMallocCalls && freeCalls >= maxFreeCalls)
        return true;
    return false;
}

void count_failure_list_t::clear()
{
    mallocCalls = 0;
    freeCalls = 0;
}

////
// order_failure_list_t
order_failure_list_t::order_failure_list_t(std::vector<parse::opt_t> opts) :
        opts(opts) { loc = 0; }

bool order_failure_list_t::check_opt(parse::token_t tok)
{
    if (tok.operation == opts[loc])
        loc++;
    else
        loc = 0;

    if (loc >= opts.size())
        return true;
    return false;
}

void order_failure_list_t::clear()
{
    loc = 0;
}

////
// complex_failure_list_t
complex_failure_list_t::complex_failure_list_t(std::vector<parse::token_t> tokens) :
        tokens(tokens) { loc = 0; }

bool complex_failure_list_t::check_opt(parse::token_t tok)
{
    parse::token_t curr = tokens[loc];
    if (curr.operation == tok.operation && curr.name == tok.name && 
        (tok.operation == parse::FREE ||
         (tok.operation == parse::MALLOC && curr.size == tok.size)))
            loc += 1;
    else
        loc = 0;

    if (loc >= tokens.size())
        return true;
    return false;
}

void complex_failure_list_t::clear()
{
    loc = 0;
}

////
// less_complex_failure_list_t
less_complex_failure_list_t::less_complex_failure_list_t(std::vector<parse::token_t> tokens) :
        tokens(tokens), id_map() { loc = 0; }

bool less_complex_failure_list_t::check_opt(parse::token_t tok)
{
    parse::token_t curr = tokens[loc];
    if (tok.operation == parse::MALLOC && tok.size == curr.size)
    {
        id_map.insert(make_pair<int, int>(curr.name, tok.name));
        loc++;
    } else
    {
        loc = 0;
        id_map.clear();
    }

    if (tok.operation == parse::FREE)
    {
        auto it = id_map.find(tok.name);
        if (it != id_map.end() && tok.name == it->second)
        {
            loc++;
            id_map.erase(it);
        } else
        {
            loc = 0;
            id_map.clear();
        }
    }

    if (loc >= tokens.size())
        return true;
    return false;
}

void less_complex_failure_list_t::clear()
{
    loc = 0;
}

order_failure_list_t *default_order_failure_list()
{
    vector<opt_t> opts
    {
        MALLOC,
        MALLOC,
        FREE,
        MALLOC,
        FREE,
        FREE,
        MALLOC,
        FREE,
        MALLOC,
        MALLOC,
        FREE,
        FREE
    };

    return new order_failure_list_t(opts);
}

less_complex_failure_list_t *default_less_complex_failure_list()
{
    vector<parse::token_t> toks
    {
        parse::token_t(MALLOC, 0, 16),
        parse::token_t(FREE, 0, 0)
    };

    return new less_complex_failure_list_t(toks);
}
