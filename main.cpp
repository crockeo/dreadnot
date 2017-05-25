#include <iostream>
#include <vector>

#include "heap-layers/heaplayers.h"

using namespace std;
using namespace HL;

template <typename SuperHeap>
class BrokenHeap : public SuperHeap
{
private:
    int malloc_count = 0;

public:
    inline void *malloc(size_t size)
    {
        void *ptr = SuperHeap::malloc(size);
        if (ptr == nullptr)
            return nullptr;

        if (++malloc_count > 5)
            abort();

        return ptr;
    }

    inline void free(void *ptr)
    {
        SuperHeap::free(ptr);
        malloc_count--;
    }
};

struct Chunk
{
    char info[32];
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
