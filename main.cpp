#include <iostream>
#include <cstring>
#include <sstream>

#include <getopt.h>

#include "heap-layers/heaplayers.h"

#include "parse.hpp"
#include "heap.hpp"

using namespace parse;
using namespace heap;
using namespace std;
using namespace HL;

char program_input[4096];

struct arguments_t
{
    enum mode_t
    {
        AFL, FUZZBALL
    };

    bool help;
    mode_t mode;

    bool parse_error;
};

arguments_t parse_args(int argc, char **argv)
{
    arguments_t args;
    memset(&args, 0, sizeof(arguments_t));

    args.mode = arguments_t::mode_t::AFL;

    struct option options[] =
    {
        { "help", no_argument, (int *)&args.help, true },

        { "mode", required_argument, 0, 'm' },

        { 0, 0, 0, 0 }
    };

    int idx = 0;
    int c;
    while ((c = getopt_long(argc, argv, "hm:", options, &idx)) > 0)
    {
        switch (c)
        {
        case 'h':
            args.help = true;
            break;

        case 'm':
            cout << "HELP" << endl;
            if (strcmp(optarg, "afl") == 0)
                args.mode = arguments_t::mode_t::AFL;
            else if (strcmp(optarg, "fuzzball") == 0)
                args.mode = arguments_t::mode_t::FUZZBALL;
            else
                args.parse_error = true;
            break;
        }
    }

    return args;
}

void print_help(int argc, char **argv)
{
    cout << "Usage:" << endl
         << "  " << argv[0] << " [flags]" << endl
         << endl
         << "Flags:" << endl
         << "  --help (-h)            -- Displays this page." << endl
         << "  --mode (-m) = AFL      -- Run with AFL support." << endl
         << "                FUZZBALL -- Run with FuzzBALL support." << endl;
}

int main(int argc, char **argv)
{
    arguments_t args = parse_args(argc, argv);

    // Informing the user that the parsing was incorrect.
    if (args.parse_error)
    {
        cout << "Invalid arguments." << endl;
        print_help(argc, argv);
        return 1;
    }

    // Printing help.
    if (args.help)
    {
        print_help(argc, argv);
        return 0;
    }

    parse::trace_t trace;
    if (args.mode == arguments_t::mode_t::AFL)
    {
        istringstream input_stream(program_input);
        if (!parse::lex(trace, input_stream))
            return 1;
    } else if (args.mode == arguments_t::mode_t::FUZZBALL)
        if (!parse::lex(trace, cin, true))
            return 1;

    BrokenHeap<MallocHeap> brokenHeap;
    if (!parse::execute<BrokenHeap<MallocHeap>>(brokenHeap, trace))
    { }
}
