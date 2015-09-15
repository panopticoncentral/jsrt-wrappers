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
    TEST_CLASS(array)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty array handle.")
        {
            jsrt::array<> handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            jsrt::array<> array;
            TEST_NO_CONTEXT_CALL(jsrt::array<>::create(0));
            TEST_NO_CONTEXT_CALL(array.size());
            TEST_NO_CONTEXT_CALL(array[0]);
            TEST_NO_CONTEXT_CALL(array[0] = jsrt::value());
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::array<> array;
                TEST_NULL_ARG_CALL(array.size());
                TEST_NULL_ARG_CALL((jsrt::value)array[0]);
                TEST_NULL_ARG_CALL(array[0] = jsrt::value());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(create, "Test ::create method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::array<>::create(0);
                jsrt::array<> array = (jsrt::array<>)value;

                jsrt::array<double> array2 = jsrt::array<double>::create({ 1, 2, 3, 4 });
                Assert::AreEqual((double)array2[0], 1.0);
                Assert::AreEqual((double)array2[3], 4.0);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(indexing, "Test indexing.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::array<double> darray = jsrt::array<double>::create(1);
                darray[0] = 10;
                darray[1] = 20;
                Assert::AreEqual((double)darray[0], 10.0);
                Assert::AreEqual((double)darray[1], 20.0);

                jsrt::array<bool> barray = jsrt::array<bool>::create(1);
                barray[0] = true;
                barray[1] = true;
                Assert::IsTrue((bool) barray[0]);
                Assert::IsTrue((bool) barray[1]);

                jsrt::array<std::wstring> sarray = jsrt::array<std::wstring>::create(1);
                sarray[0] = L"foo";
                sarray[1] = L"bar";
                Assert::AreEqual((std::wstring) sarray[0], (std::wstring)L"foo");
                Assert::AreEqual((std::wstring) sarray[1], (std::wstring)L"bar");

                Assert::AreEqual(sarray.size(), 2);
            }
            runtime.dispose();
        }
    };
}