#include "grammar.hpp"

#include <iostream>
#include <queue>
#include <stack>

namespace grammar
{
    namespace util
    {
        struct str_depth_t
        {
            string str;
            int id;

            str_depth_t(string str, int id) :
                    str(str), id(id) { }
        };

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
        util::seen_t<string> seens;

        stack<util::str_depth_t> remaining_products;
        remaining_products.push(util::str_depth_t(lhs, 0));

        while (remaining_products.size() > 0)
        {
            util::str_depth_t current_product = remaining_products.top();
            remaining_products.pop();

            if (products.find(current_product.str) == products.end())
            {
                util::replace(current_product.str, "@RAND", to_string(rand() % 16000));
                util::replace(current_product.str, "@ID", to_string(current_product.id));
                accumulator << current_product.str;
            } else
            {
                seens.inc_else(current_product.str);
                string transform = util::random_choice<string>(
                    products[current_product.str],
                    seens
                );

                vector<string> symbols = util::split(transform, ';');
                for (auto it = symbols.rbegin(); it != symbols.rend(); it++)
                    remaining_products.push(util::str_depth_t(*it, current_product.id + 1));
            }
        }
    }

    // Expanding a product into a random, gramatically correct sentence.
    string generator_t::generate(string lhs)
    {
        stringstream accumulator;
        generate(accumulator, lhs);
        return accumulator.str();
    }
}
