// Copyright 2015 Paul Vick
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(readme)
    {
    public:
        static double add(const jsrt::call_info &info, double a, double b) 
        {
            return a + b; 
        }

        MY_TEST_METHOD(samples, "Test readme samples.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::pinned<jsrt::object> pinned_obj = jsrt::object::create();

                jsrt::object obj = jsrt::object::create();
                obj.set_property(jsrt::property_id::create(L"boolProperty"), true);
                bool bool_value = obj.get_property<bool>(jsrt::property_id::create(L"boolProperty"));
                obj.set_property(jsrt::property_id::create(L"stringProperty"), L"foo");

                jsrt::array<double> darray = jsrt::array<double>::create(1);
                darray[0] = 10;
                darray[1] = 20;

                auto f = (jsrt::function<double, double, double>)jsrt::context::evaluate(L"function f(a, b) { return a + b; }; f;");
                double a = f(jsrt::context::undefined(), 1, 2);

                auto nf = jsrt::function<double, double, double>::create(add);
                jsrt::context::global().set_property(jsrt::property_id::create(L"add"), nf);
                jsrt::context::run(L"add(1, 2)");

                auto bf = jsrt::bound_function<jsrt::value, double, double, double>(
                    jsrt::context::undefined(),
                    (jsrt::function<double, double, double>)jsrt::context::evaluate(L"function f(a, b) { return a + b; }; f;"));
                double ba = bf(1, 2);
            }
            runtime.dispose();
        }
    };
}