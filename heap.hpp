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
        const vector<parse::opt_t> operations {
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
            MALLOC,
            FREE,
            FREE
        };

        int position = 0;

        void check_position(parse::opt_t curr)
        {
            if (operations[position] == curr)
                position++;
            else
                position = 0;

            if (position > operations.size())
                exit(1);
        }

    public:
        inline void *malloc(size_t size)
        {
            void *ptr = SuperHeap::malloc(size);
            if (ptr == nullptr)
                return nullptr;

            check_position(parse::opt_t::MALLOC);

            return ptr;
        }

        inline void free(void *ptr)
        {
            check_position(parse::opt_t::FREE);
            SuperHeap::free(ptr);
        }
    };
}

#endif
