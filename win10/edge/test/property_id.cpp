// Copyright 2013 Paul Vick
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
#include <utility>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(property_id)
    {
    public:
        MY_TEST_METHOD(empty_id, "Test an empty property ID.")
        {
            jsrt::property_id id;
            Assert::AreEqual(id.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(id.is_valid());
        }

        MY_TEST_METHOD(name, "Test a property ID.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::property_id id = jsrt::property_id::create(L"foo");
                Assert::AreEqual(id.name(), (std::wstring) L"foo");
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid, "Test property ID methods with invalid ID.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                try
                {
                    jsrt::property_id id;
                    id.name();
                    Assert::Fail();
                }
                catch (jsrt::invalid_argument_exception)
                {
                }
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(no_context, "Test property ID methods with no context.")
        {
            try
            {
                jsrt::property_id::create(L"foo");
                Assert::Fail();
            }
            catch (jsrt::no_current_context_exception)
            {
            }

            try
            {
                jsrt::property_id foo;
                foo.name();
                Assert::Fail();
            }
            catch (jsrt::no_current_context_exception)
            {
            }
        }
    };
}