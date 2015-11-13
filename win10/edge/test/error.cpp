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
    TEST_CLASS(error)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty error handle.")
        {
            jsrt::error handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            jsrt::error error;
            TEST_NO_CONTEXT_CALL(jsrt::error::create(L"foo"));
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::error error;
                TEST_INVALID_ARG_CALL(error.message());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(create, "Test ::create method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::error::create(L"foo");
                jsrt::error error = (jsrt::error)value;
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(message, "Test messages.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::error::create(L"foo");
                jsrt::error error = (jsrt::error)value;
                Assert::AreEqual(error.name(), (std::wstring)L"Error");
                Assert::AreEqual(error.message(), (std::wstring)L"foo");

                error = jsrt::error::create(L"%s %d", L"foo", 20);
                Assert::AreEqual(error.message(), (std::wstring)L"foo 20");
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(specialized, "Test specialize errors.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::error error;

                error = jsrt::error::create_range_error(L"");
                Assert::AreEqual(error.name(), (std::wstring)L"RangeError");

                error = jsrt::error::create_reference_error(L"");
                Assert::AreEqual(error.name(), (std::wstring)L"ReferenceError");

                error = jsrt::error::create_syntax_error(L"");
                Assert::AreEqual(error.name(), (std::wstring)L"SyntaxError");

                error = jsrt::error::create_type_error(L"");
                Assert::AreEqual(error.name(), (std::wstring)L"TypeError");

                error = jsrt::error::create_uri_error(L"");
                Assert::AreEqual(error.name(), (std::wstring)L"URIError");
            }
            runtime.dispose();
        }
    };
}