#include <iostream>
#include <vector>

#include "heap-layers/heaplayers.h"

using namespace std;
using namespace HL;

template <typename SuperHeap>
class BrokenHeap : public SuperHeap
{
private:
    enum mem_action { MALLOC, FREE };
    constexpr static int actions_len = 20;
    const mem_action actions[actions_len] {
        MALLOC,
        MALLOC,
        FREE,
        MALLOC,
        FREE,
        MALLOC,
        MALLOC,
        FREE,
        FREE,
        FREE,
        MALLOC,
        FREE,
        FREE,
        MALLOC,
        MALLOC,
        MALLOC,
        MALLOC,
        FREE,
        MALLOC,
        MALLOC,
    };

    int pos = 0;

    void check_case(mem_action action)
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

        check_case(mem_action::MALLOC);

        return ptr;
    }

    inline void free(void *ptr)
    {
        SuperHeap::free(ptr);
        check_case(mem_action::FREE);
    }
};

int main()
{
    BrokenHeap<MallocHeap> brokenHeap;

    vector<void *> chunks;
    const size_t chunk_size = 32;
    string command;
    while (cin >> command)
    {
        if (command == "malloc")
        {
            chunks.push_back(brokenHeap.malloc(chunk_size));
        }

        if (command == "free")
        {
            brokenHeap.free(chunks.back());
            chunks.pop_back();
        }
    }

    return 0;
}
