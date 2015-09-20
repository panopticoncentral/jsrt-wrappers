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
    TEST_CLASS(value)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty value handle.")
        {
            jsrt::value handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());

            jsrt::boolean boolean_handle;
            Assert::AreEqual(boolean_handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(boolean_handle.is_valid());

            jsrt::number number_handle;
            Assert::AreEqual(number_handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(number_handle.is_valid());

            jsrt::string string_handle;
            Assert::AreEqual(string_handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(string_handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            jsrt::value value;
            VARIANT v;
            TEST_NO_CONTEXT_CALL(value.type());
            TEST_NO_CONTEXT_CALL(value.to_variant(&v));
            TEST_NO_CONTEXT_CALL(jsrt::value::from_variant(&v));
            TEST_NO_CONTEXT_CALL(jsrt::boolean::create(true));
            TEST_NO_CONTEXT_CALL(jsrt::boolean::true_value());
            TEST_NO_CONTEXT_CALL(jsrt::boolean::false_value());
            TEST_NO_CONTEXT_CALL(((jsrt::boolean)value).data());
            TEST_NO_CONTEXT_CALL(jsrt::number::create(1.0));
            TEST_NO_CONTEXT_CALL(((jsrt::number)value).as_double());
            TEST_NO_CONTEXT_CALL(((jsrt::number)value).as_int());
            TEST_NO_CONTEXT_CALL(jsrt::string::create(L"foo"));
            TEST_NO_CONTEXT_CALL(((jsrt::string)value).data());
            TEST_NO_CONTEXT_CALL(((jsrt::string)value).length());
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value;
                VARIANT v;
                TEST_NULL_ARG_CALL(value.type());
                TEST_NULL_ARG_CALL(value.to_variant(&v));
                jsrt::boolean boolean;
                TEST_NULL_ARG_CALL(boolean.data());
                jsrt::number number;
                TEST_NULL_ARG_CALL(number.as_int());
                TEST_NULL_ARG_CALL(number.as_double());
                jsrt::string string;
                TEST_NULL_ARG_CALL(string.data());
                TEST_NULL_ARG_CALL(string.length());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(type, "Test the ::type method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value;
                value = jsrt::context::undefined();
                Assert::AreEqual(value.type(), JsUndefined);
                value = jsrt::context::null();
                Assert::AreEqual(value.type(), JsNull);
                value = jsrt::boolean::true_value();
                Assert::AreEqual(value.type(), JsBoolean);
                value = jsrt::number::create(10);
                Assert::AreEqual(value.type(), JsNumber);
                value = jsrt::string::create(L"foo");
                Assert::AreEqual(value.type(), JsString);
                value = jsrt::object::create();
                Assert::AreEqual(value.type(), JsObject);
                value = jsrt::context::parse(L"1 + 2;");
                Assert::AreEqual(value.type(), JsFunction);
                value = jsrt::error::create_uri_error(L"foo");
                Assert::AreEqual(value.type(), JsError);
                value = jsrt::array<jsrt::boolean>::create(10);
                Assert::AreEqual(value.type(), JsArray);
                value = jsrt::symbol::create(L"foo");
                Assert::AreEqual(value.type(), JsSymbol);
                value = jsrt::array_buffer::create(0);
                Assert::AreEqual(value.type(), JsArrayBuffer);
                value = jsrt::data_view<>::create(jsrt::array_buffer::create(0), 0, 0);
                Assert::AreEqual(value.type(), JsDataView);
                value = jsrt::typed_array<int>::create(5);
                Assert::AreEqual(value.type(), JsTypedArray);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(variant, "Test the ::to_variant and ::from_variant methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                VARIANT v;
                v.vt = VT_I4;
                v.intVal = 42;
                jsrt::value value = jsrt::value::from_variant(&v);
                Assert::AreEqual((int) value.type(), (int) JsNumber);
                Assert::AreEqual(((jsrt::number) value).as_int(), 42);
                value = jsrt::number::create(32);
                value.to_variant(&v);
                Assert::AreEqual((int) v.vt, (int) VT_I4);
                Assert::AreEqual(v.intVal, 32);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(equals, "Test the ::equals and ::strict_equals methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value v1 = jsrt::number::create(1);
                jsrt::value v2 = jsrt::string::create(L"1");
                jsrt::value v3 = jsrt::number::create(1);
                Assert::IsTrue(v1.equals(v2));
                Assert::IsTrue(v1.equals(v3));
                Assert::IsFalse(v1.strict_equals(v2));
                Assert::IsTrue(v1.strict_equals(v3));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(boolean, "Test boolean methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::boolean::create(true);
                jsrt::boolean boolean = jsrt::boolean::convert(value);
                Assert::AreEqual(value.type(), JsBoolean);
                Assert::IsTrue(boolean.data());
                Assert::IsTrue(jsrt::boolean::true_value().data());
                Assert::IsFalse(jsrt::boolean::false_value().data());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(number, "Test number methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::number::create(1.0);
                jsrt::number number = jsrt::number::convert(value);
                Assert::AreEqual(value.type(), JsNumber);
                Assert::AreEqual(number.as_double(), 1.0);
                number = jsrt::number::create(42);
                Assert::AreEqual(number.as_int(), 42);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(string, "Test string methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::string::create(L"foo");
                jsrt::string string = jsrt::string::convert(value);
                Assert::AreEqual(value.type(), JsString);
                Assert::AreEqual(string.data(), (std::wstring) L"foo");
                Assert::AreEqual(string.length(), 3);
            }
            runtime.dispose();
        }
    };
}