#ifndef _DREADNOT_TOKEN_HPP_
#define _DREADNOT_TOKEN_HPP_

namespace parse
{
    enum opt_t { MALLOC, FREE };

    struct token_t
    {
        opt_t operation;
        uint32_t name;
        uint32_t size;

        token_t(opt_t operation, uint32_t name, uint32_t size);
        token_t();
    };
}

#endif
