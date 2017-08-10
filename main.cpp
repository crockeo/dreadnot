#include <iostream>
#include <cstring>
#include <sstream>

#include <getopt.h>

#include "heap-layers/heaplayers.h"

#include "failure_list.hpp"
#include "grammar.hpp"
#include "parse.hpp"
#include "token.hpp"

using namespace parse;
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
            if (strcmp(optarg, "AFL") == 0)
                args.mode = arguments_t::mode_t::AFL;
            else if (strcmp(optarg, "FUZZBALL") == 0)
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
         << "  --help (-h) -- Displays this page." << endl;
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

    grammar::generator_t generator;

    // TODO: Implement generator thing.
    //         GOTTA LOOK ONLINE

    stringstream sentence;
    generator.generate(sentence, "");

    parse::trace_t trace;
    if (!parse::lex(trace, sentence, true))
    {
        cout << "Lex failure" << endl;
        return 1;
    }

    MallocHeap mallocHeap;

    vector<parse::token_t> tokens
    {
        parse::token_t(parse::MALLOC, 0, 16),
        parse::token_t(parse::FREE, 0, 0)
    };

    less_complex_failure_list_t *complex_list = new less_complex_failure_list_t(tokens);
    bool b = parse::execute<MallocHeap>(mallocHeap, complex_list, trace);
    delete complex_list;
    if (!b)
    {
        cout << "Parse execution failure." << endl;
        return 1;
    }

    return 0;
}
