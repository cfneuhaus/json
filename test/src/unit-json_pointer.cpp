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

#define private public
#include "json.hpp"
using nlohmann::json;

TEST_CASE("JSON pointers")
{
    SECTION("errors")
    {
        CHECK_THROWS_AS(json::json_pointer("foo"), std::domain_error);
        CHECK_THROWS_WITH(json::json_pointer("foo"), "JSON pointer must be empty or begin with '/'");

        CHECK_THROWS_AS(json::json_pointer("/~~"), std::domain_error);
        CHECK_THROWS_WITH(json::json_pointer("/~~"), "escape error: '~' must be followed with '0' or '1'");

        CHECK_THROWS_AS(json::json_pointer("/~"), std::domain_error);
        CHECK_THROWS_WITH(json::json_pointer("/~"), "escape error: '~' must be followed with '0' or '1'");

        json::json_pointer p;
        CHECK_THROWS_AS(p.top(), std::domain_error);
        CHECK_THROWS_WITH(p.top(), "JSON pointer has no parent");
        CHECK_THROWS_AS(p.pop_back(), std::domain_error);
        CHECK_THROWS_WITH(p.pop_back(), "JSON pointer has no parent");
    }


    SECTION("flatten")
    {
        json j =
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
                    {"value", 42.99},
                    {"", "empty string"},
                    {"/", "slash"},
                    {"~", "tilde"},
                    {"~1", "tilde1"}
                }
            }
        };

        json j_flatten =
        {
            {"/pi", 3.141},
            {"/happy", true},
            {"/name", "Niels"},
            {"/nothing", nullptr},
            {"/answer/everything", 42},
            {"/list/0", 1},
            {"/list/1", 0},
            {"/list/2", 2},
            {"/object/currency", "USD"},
            {"/object/value", 42.99},
            {"/object/", "empty string"},
            {"/object/~1", "slash"},
            {"/object/~0", "tilde"},
            {"/object/~01", "tilde1"}
        };

        // check if flattened result is as expected
        CHECK(j.flatten() == j_flatten);

        // check if unflattened result is as expected
        CHECK(j_flatten.unflatten() == j);

        // error for nonobjects
        CHECK_THROWS_AS(json(1).unflatten(), std::domain_error);
        CHECK_THROWS_WITH(json(1).unflatten(), "only objects can be unflattened");

        // error for nonprimitve values
        CHECK_THROWS_AS(json({{"/1", {1, 2, 3}}}).unflatten(), std::domain_error);
        CHECK_THROWS_WITH(json({{"/1", {1, 2, 3}}}).unflatten(), "values in object must be primitive");

        // error for conflicting values
        json j_error = {{"", 42}, {"/foo", 17}};
        CHECK_THROWS_AS(j_error.unflatten(), std::domain_error);
        CHECK_THROWS_WITH(j_error.unflatten(), "invalid value to unflatten");

        // explicit roundtrip check
        CHECK(j.flatten().unflatten() == j);

        // roundtrip for primitive values
        json j_null;
        CHECK(j_null.flatten().unflatten() == j_null);
        json j_number = 42;
        CHECK(j_number.flatten().unflatten() == j_number);
        json j_boolean = false;
        CHECK(j_boolean.flatten().unflatten() == j_boolean);
        json j_string = "foo";
        CHECK(j_string.flatten().unflatten() == j_string);

        // roundtrip for empty structured values (will be unflattened to null)
        json j_array(json::value_t::array);
        CHECK(j_array.flatten().unflatten() == json());
        json j_object(json::value_t::object);
        CHECK(j_object.flatten().unflatten() == json());
    }

    SECTION("string representation")
    {
        for (auto ptr :
                {"", "/foo", "/foo/0", "/", "/a~1b", "/c%d", "/e^f", "/g|h", "/i\\j", "/k\"l", "/ ", "/m~0n"
                })
        {
            CHECK(json::json_pointer(ptr).to_string() == ptr);
        }
    }
}
