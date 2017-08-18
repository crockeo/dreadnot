#ifndef _DREADNOT_GRAMMAR_HPP_
#define _DREADNOT_GRAMMAR_HPP_

#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace grammar
{
    namespace util
    {
        template <typename T>
        class seen_t : public map<T, int> {
        public:
            void inc(T t) {
                (*this)[t] = 1 + (this->find(t) == this->end() ? 0 : (*this)[t]);
            }

            void inc_else(T t) {
                for (auto it = this->begin(); it != this->end(); it++) {
                    if (it->first != t)
                        inc(it->first);
                }
            }

            int val(T t) {
                if (this->find(t) == this->end())
                    return 0;
                return (*this)[t];
            }
        };

        // Taking a random choice from a vector affected by the number of times each
        // element has been seen.
        template <typename T>
        T random_choice(vector<T> choices, seen_t<T> seens) {
            int max = 0;
            for (int i = 0; i < choices.size(); i++)
                max += seens.val(choices[i]) + 1;

            int n = rand() % max;
            for (int i = 0; i < choices.size(); i++) {
                n -= seens.val(choices[i]) + 1;
                if (n < 0)
                    return choices[i];
            }

            return choices[0];
        }

        // Taking a random choice from a vector.
        template <typename T>
        T random_choice(vector<T> choices) {
            int i = rand() % choices.size();
            return choices[i];
        }
    }

    class generator_t
    {
    private:
        map<string, vector<string>> products;

    public:
        // Explicitly adding a product.
        void add_product(string lhs, vector<string> rhs);

        // Implicitly adding a product. Parses the right hand side for the
        // explicit expansion.
        void add_product(string lhs, string rhs);

        // Expanding a product into a random, gramatically correct sentence.
        // Allows the user to provide their own accumulator.
        void generate(stringstream& accumulator, string lhs);

        // Expanding a product into a random, gramatically correct sentence.
        string generate(string lhs);
    };
}

#endif
