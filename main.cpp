#include <iostream>
#include <cstring>
#include <set>

using namespace std;

set<char> contents;

template <typename T>
bool con(const set<T>& set, const T v)
{
    return set.find(v) != set.end();
}

int main(int argc, char **argv)
{
    const char *target_chars = "atlfpqormn";
    const int target_chars_len = strlen(target_chars);
    const int max_count = 50;

    bool b;
    char c;
    for (int j = 0; j < max_count && cin.get(c); j++)
    {
        b = true;
        contents.insert(c);
        for (int i = 0; i < target_chars_len; i++)
        {
            if (!con(contents, target_chars[i]))
            {
                b = false;
                break;
            }
        }

        if (b)
            abort();
    }

    return 0;
}
