/*
    __ _____ _____ _____
 __|  |   __|     |   | |  JSON for Modern C++ (test suite)
|  |  |__   |  |  | | | |  version 2.0.2
|_____|_____|_____|_|___|  https://github.com/nlohmann/json

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
Copyright (c) 2013-2016 Niels Lohmann <http://nlohmann.me>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "catch.hpp"

#include "json.hpp"
using nlohmann::json;

#include <deque>
#include <forward_list>
#include <list>
#include <unordered_map>
#include <unordered_set>

TEST_CASE("README", "[hide]")
{
    {
        // redirect std::cout for the README file
        auto old_cout_buffer = std::cout.rdbuf();
        std::ostringstream new_stream;
        std::cout.rdbuf(new_stream.rdbuf());
        {
            // create an empty structure (null)
            json j;

            // add a number that is stored as double (note the implicit conversion of j to an object)
            j["pi"] = 3.141;

            // add a Boolean that is stored as bool
            j["happy"] = true;

            // add a string that is stored as std::string
            j["name"] = "Niels";

            // add another null object by passing nullptr
            j["nothing"] = nullptr;

            // add an object inside the object
            j["answer"]["everything"] = 42;

            // add an array that is stored as std::vector (using an initializer list)
            j["list"] = { 1, 0, 2 };

            // add another object (using an initializer list of pairs)
            j["object"] = { {"currency", "USD"}, {"value", 42.99} };

            // instead, you could also write (which looks very similar to the JSON above)
            json j2 =
            {
                {"pi", 3.141},
                {"happy", true},
                {"name", "Niels"},
                {"nothing", nullptr},
                {
                    "answer", {
                        {"everything", 42}
                    }
                },
                {"list", {1, 0, 2}},
                {
                    "object", {
                        {"currency", "USD"},
                        {"value", 42.99}
                    }
                }
            };
        }

        {
            // ways to express the empty array []
            json empty_array_implicit = {{}};
            json empty_array_explicit = json::array();

            // a way to express the empty object {}
            json empty_object_explicit = json::object();

            // a way to express an _array_ of key/value pairs [["currency", "USD"], ["value", 42.99]]
            json array_not_object = { json::array({"currency", "USD"}), json::array({"value", 42.99}) };
        }


        {
            std::vector<int> c_vector {1, 2, 3, 4};
            json j_vec(c_vector);
            // [1, 2, 3, 4]

            std::deque<float> c_deque {1.2f, 2.3f, 3.4f, 5.6f};
            json j_deque(c_deque);
            // [1.2, 2.3, 3.4, 5.6]

            std::list<bool> c_list {true, true, false, true};
            json j_list(c_list);
            // [true, true, false, true]

            std::forward_list<int64_t> c_flist {12345678909876, 23456789098765, 34567890987654, 45678909876543};
            json j_flist(c_flist);
            // [12345678909876, 23456789098765, 34567890987654, 45678909876543]

            std::array<unsigned long, 4> c_array {{1, 2, 3, 4}};
            json j_array(c_array);
            // [1, 2, 3, 4]

            std::set<std::string> c_set {"one", "two", "three", "four", "one"};
            json j_set(c_set); // only one entry for "one" is used
            // ["four", "one", "three", "two"]

            std::unordered_set<std::string> c_uset {"one", "two", "three", "four", "one"};
            json j_uset(c_uset); // only one entry for "one" is used
            // maybe ["two", "three", "four", "one"]

            std::multiset<std::string> c_mset {"one", "two", "one", "four"};
            json j_mset(c_mset); // only one entry for "one" is used
            // maybe ["one", "two", "four"]

            std::unordered_multiset<std::string> c_umset {"one", "two", "one", "four"};
            json j_umset(c_umset); // both entries for "one" are used
            // maybe ["one", "two", "one", "four"]
        }

        {
            std::map<std::string, int> c_map { {"one", 1}, {"two", 2}, {"three", 3} };
            json j_map(c_map);
            // {"one": 1, "two": 2, "three": 3}

            std::unordered_map<const char*, float> c_umap { {"one", 1.2f}, {"two", 2.3f}, {"three", 3.4f} };
            json j_umap(c_umap);
            // {"one": 1.2, "two": 2.3, "three": 3.4}

            std::multimap<std::string, bool> c_mmap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
            json j_mmap(c_mmap); // only one entry for key "three" is used
            // maybe {"one": true, "two": true, "three": true}

            std::unordered_multimap<std::string, bool> c_ummap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
            json j_ummap(c_ummap); // only one entry for key "three" is used
            // maybe {"one": true, "two": true, "three": true}
        }

        {
            // strings
            std::string s1 = "Hello, world!";
            json js = s1;
            std::string s2 = js;

            // Booleans
            bool b1 = true;
            json jb = b1;
            bool b2 = jb;

            // numbers
            int i = 42;
            json jn = i;
            double f = jn;

            // etc.

            std::string vs = js.get<std::string>();
            bool vb = jb.get<bool>();
            int vi = jn.get<int>();

            // etc.
        }

      

        // restore old std::cout
        std::cout.rdbuf(old_cout_buffer);
    }
}
