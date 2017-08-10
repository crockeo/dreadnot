#include "grammar.hpp"

namespace grammar
{
    namespace util
    {
        // Segmenting a string into a series of strings separated by a delimiter.
        vector<string> split(string s, char delim)
        {
            stringstream sstream(s);
            string segment;

            vector<string> strings;
            while (getline(sstream, segment, delim))
                strings.push_back(segment);

            return strings;
        }

        // Replacing all occurrences of 'from' in 'str' with 'to'.
        int replace(string& str, const string& from, const string& to) {
            int replaced = 0;
            size_t start;
            do {
                start = str.find(from);
                if (start == string::npos)
                    continue;

                str.replace(start, from.length(), to);
                replaced++;
            } while (start != string::npos);

            return replaced;
        }
    }

    // Expanding a product into a random, gramatically correct sentence.
    // Uses a util::seen_t<string> to maintain the count of seen products
    // to implement a converging generator.
    //
    // ID is used to parameterize mallocs and frees with a specific name.
    void generator_t::generate(stringstream& accumulator, string lhs, util::seen_t<string> seens, int id)
    {
        vector<string>& transforms = products[lhs];
        string transform = util::random_choice<string>(
            transforms,
            seens
        );

        vector<string> symbols = util::split(transform, ';');
        for (auto it = symbols.begin(); it != symbols.end(); it++) {
            if (products.find(*it) == products.end()) {
                util::replace(*it, "@RAND", to_string(rand() % 16000));
                util::replace(*it, "@ID", to_string(id));
                accumulator << (*it);
            } else {
                seens.inc_else(transform);
                generate(accumulator, *it, seens, id + 1);
            }
        }
    }

    // Explicitly adding a product.
    void generator_t::add_product(string lhs, vector<string> rhs)
    {
        products.insert(make_pair(lhs, rhs));
    }

    // Implicitly adding a product. Parses the right hand side for the
    // explicit expansion.
    void generator_t::add_product(string lhs, string rhs)
    {
        products.insert(make_pair(lhs, util::split(rhs, '|')));
    }

    // Expanding a product into a random, gramatically correct sentence.
    // Allows the user to provide their own accumulator.
    void generator_t::generate(stringstream& accumulator, string lhs)
    {
        generate(
            accumulator,
            lhs,
            util::seen_t<string>(),
            0
        );
    }

    // Expanding a product into a random, gramatically correct sentence.
    string generator_t::generate(string lhs)
    {
        stringstream accumulator;
        generate(accumulator, lhs);
        return accumulator.str();
    }
}
