#ifndef _DREADNOT_FAILURE_LIST_H_
#define _DREADNOT_FAILURE_LIST_H_

#include <vector>
#include <map>

#include "token.hpp"

using namespace std;

// A generic interface that includes the ability to check whether the list
// should fail given a token.
struct i_failure_list_t
{
    virtual ~i_failure_list_t() { }
    virtual bool check_opt(parse::token_t tok) { return false; }
    virtual void clear() { }
};

// An implementation of i_failure_list_t that requires a given number of mallocs
// and frees before it fails.
struct count_failure_list_t : public i_failure_list_t
{
private:
    int maxMallocCalls;
    int mallocCalls;

    int maxFreeCalls;
    int freeCalls;

public:
    count_failure_list_t(int maxMallocCalls, int maxFreeCalls);

    virtual bool check_opt(parse::token_t tok);
    virtual void clear();
};

// An implementation of i_failure_list_t that requires an ordered list of
// operations (i.e. MALLOC | FREE).
struct order_failure_list_t : public i_failure_list_t
{
private:
    vector<parse::opt_t> opts;
    int loc;

public:
    order_failure_list_t(vector<parse::opt_t> opts);

    virtual bool check_opt(parse::token_t tok);
    virtual void clear();
};

// An implementation of i_failure_list that requires an ordered list of
// operations including size and ID.
struct complex_failure_list_t : public i_failure_list_t
{
private:
    vector<parse::token_t> tokens;
    int loc;

public:
    complex_failure_list_t(vector<parse::token_t> tokens);

    virtual bool check_opt(parse::token_t tok);
    virtual void clear();
};

// An implementation of i_failure_list that requires an ordered list of
// operations including size and a fake ID. Rather than the actual, universal
// ID, it uses the IDs of the tokens for any matching operation (which then
// fills the ID until the corresponding free).
struct less_complex_failure_list_t : public i_failure_list_t
{
private:
    vector<parse::token_t> tokens;
    map<int, int> id_map;
    int loc;

public:
    less_complex_failure_list_t(vector<parse::token_t> tokens);

    virtual bool check_opt(parse::token_t tok);
    virtual void clear();
};

order_failure_list_t *default_order_failure_list();
less_complex_failure_list_t *default_less_complex_failure_list();

#endif
