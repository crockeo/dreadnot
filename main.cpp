#include <iostream>
#include <cstring>

using namespace std;

void trigger_error()
{
    int *ptr = nullptr;
    *ptr = 5;
}

int main(int argc, char **argv)
{
    const char *target_str = "Space... The final frontier. These are the \
voyages of the starship Enterprise. It's continuing mission, to explore \
strange new worlds. To seek out new life and new civilizations. To boldly go \
where no one has gone before.";
    const int target_str_len = strlen(target_str);

    int i = 0;
    char c;
    while (cin.get(c) && i < target_str_len)
    {
        if (c == target_str[i])
            i++;
        else
            i = 0;
    }

    if (i == target_str_len)
        trigger_error();

    return 0;
}
