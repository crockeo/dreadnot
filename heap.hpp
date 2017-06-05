#ifndef _DREADNOT_HEAP_H_
#define _DREADNOT_HEAP_H_

#include "heap-layers/heaplayers.h"
#include "parse.hpp"

namespace heap
{
    using namespace parse;
    using namespace HL;

    template <typename SuperHeap>
    class BrokenHeap : public SuperHeap
    {
    private:
        constexpr static int actions_len = 6;
        const parse::opt_t actions[actions_len] {
            MALLOC,
            MALLOC,
            FREE,
            MALLOC,
            FREE,
            FREE
        };

        int pos = 0;

        void check_case(parse::opt_t action)
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

            check_case(parse::opt_t::MALLOC);

            return ptr;
        }

        inline void free(void *ptr)
        {
            SuperHeap::free(ptr);
            check_case(parse::opt_t::FREE);
        }
    };
}

#endif
