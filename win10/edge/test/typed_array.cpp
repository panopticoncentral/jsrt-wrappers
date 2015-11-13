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
    TEST_CLASS(typed_array)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty typed_array handle.")
        {
            jsrt::typed_array<int> handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            jsrt::typed_array<short> array;
            TEST_NO_CONTEXT_CALL(jsrt::typed_array<short>::create(0));
            {
				jsrt::context::scope scope(context);
				array = jsrt::typed_array<short>::create(0);
            }
            array.data();
            array.data_size();
            array.element_size();
            array.type();
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::typed_array<int> array;
                TEST_INVALID_ARG_CALL(array.data_size());
                TEST_INVALID_ARG_CALL(array.element_size());
                TEST_INVALID_ARG_CALL(array.data());
                TEST_INVALID_ARG_CALL(array.type());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(create, "Test ::create method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::typed_array<short>::create(5);
                jsrt::typed_array<short> array = (jsrt::typed_array<short>)value;

                jsrt::array_buffer buffer = jsrt::array_buffer::create(8);
                array = jsrt::typed_array<short>::create(buffer, 2, 3);

                jsrt::typed_array<int> int_array = jsrt::typed_array<int>::create(array);

                jsrt::array<int> base_array = jsrt::array<int>::create(5);
                int_array = jsrt::typed_array<int>::create(base_array);

                int_array = jsrt::typed_array<int>::create({ 1, 2, 3, 4 });
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(types, "Test types.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);

                auto int8 = jsrt::typed_array<char>::create(1);
                auto uint8 = jsrt::typed_array<unsigned char>::create(1);
                auto uint8clamped = jsrt::typed_array<unsigned char, true>::create(1);
                auto int16 = jsrt::typed_array<short>::create(1);
                auto uint16 = jsrt::typed_array<unsigned short>::create(1);
                auto int32 = jsrt::typed_array<int>::create(1);
                auto uint32 = jsrt::typed_array<unsigned int>::create(1);
                auto single = jsrt::typed_array<float>::create(1);
                auto dbl = jsrt::typed_array<double>::create(1);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(data, "Test data and size.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::array_buffer buffer = jsrt::array_buffer::create(8);
                jsrt::typed_array<short> array = jsrt::typed_array<short>::create(buffer, 2, 3);
                Assert::AreEqual(array.type(), JsArrayTypeInt16);
                Assert::AreEqual(array.data_size(), 6u);
                Assert::AreEqual(array.element_size(), 2);
                Assert::IsNotNull(array.data());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(indexing, "Test indexing.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::array_buffer buffer = jsrt::array_buffer::create(64);
                jsrt::typed_array<double> darray = jsrt::typed_array<double>::create(buffer, 0, 2);
                darray[0] = 10;
                darray[1] = 20;
                Assert::AreEqual((double)darray[0], 10.0);
                Assert::AreEqual((double)darray[1], 20.0);

                jsrt::typed_array<int> iarray = jsrt::typed_array<int>::create(buffer, 0, 2);
                iarray[0] = 30;
                iarray[1] = 40;
                Assert::AreEqual((int)iarray[0], 30);
                Assert::AreEqual((int)iarray[1], 40);
            }
            runtime.dispose();
        }
    };
}