#ifndef _DREADNOT_HEAP_H_
#define _DREADNOT_HEAP_H_

#include <set>

using namespace std;

namespace heap
{
    template <typename super_heap_t>
    class clearable_heap_t : public super_heap_t
    {
    private:
        set<void *> blocks;

    public:
        inline void *malloc(size_t size)
        {
            void *ptr = super_heap_t::malloc(size);
            blocks.insert(ptr);
            return ptr;
        }

        inline void free(void *ptr)
        {
            auto it = blocks.find(ptr);
            if (it == blocks.end())
                return;
            super_heap_t::free(ptr);
            blocks.erase(it);

        }

        void clear()
        {
            for (auto it = blocks.begin(); it != blocks.end(); it++)
                free(*it);
        }
    };
}

#endif
