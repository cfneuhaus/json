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

TEST_CASE("JSON patch")
{
    SECTION("examples from RFC 6902")
    {
        SECTION("4.2 remove")
        {
            // If removing an element from an array, any elements above the
            // specified index are shifted one position to the left.
            json doc = {1, 2, 3, 4};
            json patch = {{{"op", "remove"}, {"path", "/1"}}};
            CHECK(doc.patch(patch) == json({1, 3, 4}));
        }




    
      
    
        // A.13. Invalid JSON Patch Document
        // not applicable

        SECTION("replace")
        {
            json j = "string";
            json patch = {{{"op", "replace"}, {"path", ""}, {"value", 1}}};
            CHECK(j.patch(patch) == json(1));
        }
    }

    SECTION("errors")
    {
        SECTION("unknown operation")
        {
            SECTION("not an array")
            {
                json j;
                json patch = {{"op", "add"}, {"path", ""}, {"value", 1}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "JSON patch must be an array of objects");
            }

            SECTION("not an array of objects")
            {
                json j;
                json patch = {"op", "add", "path", "", "value", 1};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "JSON patch must be an array of objects");
            }

            SECTION("missing 'op'")
            {
                json j;
                json patch = {{{"foo", "bar"}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation must have member 'op'");
            }

            SECTION("non-string 'op'")
            {
                json j;
                json patch = {{{"op", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation must have string member 'op'");
            }

            SECTION("invalid operation")
            {
                json j;
                json patch = {{{"op", "foo"}, {"path", ""}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation value 'foo' is invalid");
            }
        }

        SECTION("add")
        {
            SECTION("missing 'path'")
            {
                json j;
                json patch = {{{"op", "add"}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'add' must have member 'path'");
            }

            SECTION("non-string 'path'")
            {
                json j;
                json patch = {{{"op", "add"}, {"path", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'add' must have string member 'path'");
            }

            SECTION("missing 'value'")
            {
                json j;
                json patch = {{{"op", "add"}, {"path", ""}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'add' must have member 'value'");
            }

            SECTION("invalid array index")
            {
                json j = {1, 2};
                json patch = {{{"op", "add"}, {"path", "/4"}, {"value", 4}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "array index 4 is out of range");
            }
        }

        SECTION("remove")
        {
            SECTION("missing 'path'")
            {
                json j;
                json patch = {{{"op", "remove"}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'remove' must have member 'path'");
            }

            SECTION("non-string 'path'")
            {
                json j;
                json patch = {{{"op", "remove"}, {"path", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'remove' must have string member 'path'");
            }

            SECTION("nonexisting target location (array)")
            {
                json j = {1, 2, 3};
                json patch = {{{"op", "remove"}, {"path", "/17"}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "array index 17 is out of range");
            }

            SECTION("nonexisting target location (object)")
            {
                json j = {{"foo", 1}, {"bar", 2}};
                json patch = {{{"op", "remove"}, {"path", "/baz"}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "key 'baz' not found");
            }

            SECTION("root element as target location")
            {
                json j = "string";
                json patch = {{{"op", "remove"}, {"path", ""}}};
                CHECK_THROWS_AS(j.patch(patch), std::domain_error);
                CHECK_THROWS_WITH(j.patch(patch), "JSON pointer has no parent");
            }
        }

        SECTION("replace")
        {
            SECTION("missing 'path'")
            {
                json j;
                json patch = {{{"op", "replace"}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'replace' must have member 'path'");
            }

            SECTION("non-string 'path'")
            {
                json j;
                json patch = {{{"op", "replace"}, {"path", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'replace' must have string member 'path'");
            }

            SECTION("missing 'value'")
            {
                json j;
                json patch = {{{"op", "replace"}, {"path", ""}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'replace' must have member 'value'");
            }

            SECTION("nonexisting target location (array)")
            {
                json j = {1, 2, 3};
                json patch = {{{"op", "replace"}, {"path", "/17"}, {"value", 19}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "array index 17 is out of range");
            }

            SECTION("nonexisting target location (object)")
            {
                json j = {{"foo", 1}, {"bar", 2}};
                json patch = {{{"op", "replace"}, {"path", "/baz"}, {"value", 3}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "key 'baz' not found");
            }
        }

        SECTION("move")
        {
            SECTION("missing 'path'")
            {
                json j;
                json patch = {{{"op", "move"}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'move' must have member 'path'");
            }

            SECTION("non-string 'path'")
            {
                json j;
                json patch = {{{"op", "move"}, {"path", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'move' must have string member 'path'");
            }

            SECTION("missing 'from'")
            {
                json j;
                json patch = {{{"op", "move"}, {"path", ""}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'move' must have member 'from'");
            }

            SECTION("non-string 'from'")
            {
                json j;
                json patch = {{{"op", "move"}, {"path", ""}, {"from", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'move' must have string member 'from'");
            }

            SECTION("nonexisting from location (array)")
            {
                json j = {1, 2, 3};
                json patch = {{{"op", "move"}, {"path", "/0"}, {"from", "/5"}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "array index 5 is out of range");
            }

            SECTION("nonexisting from location (object)")
            {
                json j = {{"foo", 1}, {"bar", 2}};
                json patch = {{{"op", "move"}, {"path", "/baz"}, {"from", "/baz"}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "key 'baz' not found");
            }
        }

        SECTION("copy")
        {
            SECTION("missing 'path'")
            {
                json j;
                json patch = {{{"op", "copy"}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'copy' must have member 'path'");
            }

            SECTION("non-string 'path'")
            {
                json j;
                json patch = {{{"op", "copy"}, {"path", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'copy' must have string member 'path'");
            }

            SECTION("missing 'from'")
            {
                json j;
                json patch = {{{"op", "copy"}, {"path", ""}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'copy' must have member 'from'");
            }

            SECTION("non-string 'from'")
            {
                json j;
                json patch = {{{"op", "copy"}, {"path", ""}, {"from", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'copy' must have string member 'from'");
            }

            SECTION("nonexisting from location (array)")
            {
                json j = {1, 2, 3};
                json patch = {{{"op", "copy"}, {"path", "/0"}, {"from", "/5"}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "array index 5 is out of range");
            }

            SECTION("nonexisting from location (object)")
            {
                json j = {{"foo", 1}, {"bar", 2}};
                json patch = {{{"op", "copy"}, {"path", "/fob"}, {"from", "/baz"}}};
                CHECK_THROWS_AS(j.patch(patch), std::out_of_range);
                CHECK_THROWS_WITH(j.patch(patch), "key 'baz' not found");
            }
        }

        SECTION("test")
        {
            SECTION("missing 'path'")
            {
                json j;
                json patch = {{{"op", "test"}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'test' must have member 'path'");
            }

            SECTION("non-string 'path'")
            {
                json j;
                json patch = {{{"op", "test"}, {"path", 1}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'test' must have string member 'path'");
            }

            SECTION("missing 'value'")
            {
                json j;
                json patch = {{{"op", "test"}, {"path", ""}}};
                CHECK_THROWS_AS(j.patch(patch), std::invalid_argument);
                CHECK_THROWS_WITH(j.patch(patch), "operation 'test' must have member 'value'");
            }
        }
    }

}
