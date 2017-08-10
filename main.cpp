#include <iostream>
#include <cstring>
#include <sstream>

#include <getopt.h>

#include "heap-layers/heaplayers.h"

#include "failure_list.hpp"
#include "grammar.hpp"
#include "parse.hpp"
#include "token.hpp"
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

    // Seeding the random generator with the current time to create a
    // semi-random string at runtime.
    srand(
       chrono::duration_cast<chrono::microseconds>(
           chrono::high_resolution_clock::now().time_since_epoch()
       ).count()
    );

    // Preparing fields for the main loop.
    grammar::generator_t generator;
    generator.add_product("F", "free @ID");
    generator.add_product("M", "malloc @ID @RAND");
    generator.add_product("O", "M;\n;F|M;\n;O;\n;F|O;\n;O");

    stringstream sentence;
    string sentence_s;
    parse::trace_t trace;

    vector<parse::opt_t> opts
    {
        parse::MALLOC,
        parse::MALLOC,
        parse::MALLOC,
        parse::MALLOC,
        parse::MALLOC,
        parse::FREE,
        parse::FREE,
        parse::FREE,
        parse::FREE,
        parse::FREE
    };

    order_failure_list_t order_list(opts);

    clearable_heap_t<MallocHeap> clearable_heap;

    while (true)
    {
        sentence.clear();
        trace.clear();
        order_list.clear();
        clearable_heap.clear();

        generator.generate(sentence, "O");
        sentence_s = sentence.str();

        if (!parse::lex(trace, sentence, true))
        {
            cout << "Lex failure on: " << sentence_s << endl;
            return 1;
        }

        if (!parse::execute<clearable_heap_t<MallocHeap>>(clearable_heap, &order_list, trace))
        {
            cout << "Parse failure on: " << sentence_s << endl;
            return 1;
        }
    }

    return 0;
}
