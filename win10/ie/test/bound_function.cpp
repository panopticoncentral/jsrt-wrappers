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
    TEST_CLASS(bound_function)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty function handle.")
        {
            jsrt::bound_function<jsrt::object, void> handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();

            jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8;
            jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7;
            jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool> f6;
            jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double> f5;
            jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring> f4;
            jsrt::bound_function<jsrt::object, double, std::wstring, double, bool> f3;
            jsrt::bound_function<jsrt::object, double, std::wstring, double> f2;
            jsrt::bound_function<jsrt::object, double, std::wstring> f1;
            jsrt::bound_function<jsrt::object, double> f0;
            jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8v;
            jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7v;
            jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool> f6v;
            jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double> f5v;
            jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring> f4v;
            jsrt::bound_function<jsrt::object, void, std::wstring, double, bool> f3v;
            jsrt::bound_function<jsrt::object, void, std::wstring, double> f2v;
            jsrt::bound_function<jsrt::object, void, std::wstring> f1v;
            jsrt::bound_function<jsrt::object, void> f0v;
            jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8c;
            jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7c;
            jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool> f6c;
            jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double> f5c;
            jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring> f4c;
            jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool> f3c;
            jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double> f2c;
            jsrt::bound_function<jsrt::object, jsrt::object, std::wstring> f1c;
            jsrt::bound_function<jsrt::object, jsrt::object> f0c;
            jsrt::object this_value;

            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f8(L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f7(L"foo", 2, true, L"bar", 5, false, L"baz")));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f6(L"foo", 2, true, L"bar", 5, false)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f5(L"foo", 2, true, L"bar", 5)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f4(L"foo", 2, true, L"bar")));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double, std::wstring, double, bool>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f3(L"foo", 2, true)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double, std::wstring, double>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f2(L"foo", 2)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double, std::wstring>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f1(L"foo")));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, double>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f0()));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f8v(L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f7v(L"foo", 2, true, L"bar", 5, false, L"baz")));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f6v(L"foo", 2, true, L"bar", 5, false)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f5v(L"foo", 2, true, L"bar", 5)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f4v(L"foo", 2, true, L"bar")));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void, std::wstring, double, bool>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f3v(L"foo", 2, true)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void, std::wstring, double>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f2v(L"foo", 2)));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void, std::wstring>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f1v(L"foo")));
            TEST_NO_CONTEXT_CALL((jsrt::bound_function<jsrt::object, void>::create(jsrt::object(), nullptr)));
            TEST_NO_CONTEXT_CALL((f0v()));
            TEST_NO_CONTEXT_CALL((f8c.construct(L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
            TEST_NO_CONTEXT_CALL((f7c.construct(L"foo", 2, true, L"bar", 5, false, L"baz")));
            TEST_NO_CONTEXT_CALL((f6c.construct(L"foo", 2, true, L"bar", 5, false)));
            TEST_NO_CONTEXT_CALL((f5c.construct(L"foo", 2, true, L"bar", 5)));
            TEST_NO_CONTEXT_CALL((f4c.construct(L"foo", 2, true, L"bar")));
            TEST_NO_CONTEXT_CALL((f3c.construct(L"foo", 2, true)));
            TEST_NO_CONTEXT_CALL((f2c.construct(L"foo", 2)));
            TEST_NO_CONTEXT_CALL((f1c.construct(L"foo")));
            TEST_NO_CONTEXT_CALL((f0c.construct()));

            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8;
                jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7;
                jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool> f6;
                jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double> f5;
                jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring> f4;
                jsrt::bound_function<jsrt::object, double, std::wstring, double, bool> f3;
                jsrt::bound_function<jsrt::object, double, std::wstring, double> f2;
                jsrt::bound_function<jsrt::object, double, std::wstring> f1;
                jsrt::bound_function<jsrt::object, double> f0;
                jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8v;
                jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7v;
                jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool> f6v;
                jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double> f5v;
                jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring> f4v;
                jsrt::bound_function<jsrt::object, void, std::wstring, double, bool> f3v;
                jsrt::bound_function<jsrt::object, void, std::wstring, double> f2v;
                jsrt::bound_function<jsrt::object, void, std::wstring> f1v;
                jsrt::bound_function<jsrt::object, void> f0v;
                jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8c;
                jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7c;
                jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool> f6c;
                jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double> f5c;
                jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring> f4c;
                jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool> f3c;
                jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double> f2c;
                jsrt::bound_function<jsrt::object, jsrt::object, std::wstring> f1c;
                jsrt::bound_function<jsrt::object, jsrt::object> f0c;
                TEST_NULL_ARG_CALL((f8(L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
                TEST_NULL_ARG_CALL((f7(L"foo", 2, true, L"bar", 5, false, L"baz")));
                TEST_NULL_ARG_CALL((f6(L"foo", 2, true, L"bar", 5, false)));
                TEST_NULL_ARG_CALL((f5(L"foo", 2, true, L"bar", 5)));
                TEST_NULL_ARG_CALL((f4(L"foo", 2, true, L"bar")));
                TEST_NULL_ARG_CALL((f3(L"foo", 2, true)));
                TEST_NULL_ARG_CALL((f2(L"foo", 2)));
                TEST_NULL_ARG_CALL((f1(L"foo")));
                TEST_NULL_ARG_CALL((f0()));
                TEST_NULL_ARG_CALL((f8v(L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
                TEST_NULL_ARG_CALL((f7v(L"foo", 2, true, L"bar", 5, false, L"baz")));
                TEST_NULL_ARG_CALL((f6v(L"foo", 2, true, L"bar", 5, false)));
                TEST_NULL_ARG_CALL((f5v(L"foo", 2, true, L"bar", 5)));
                TEST_NULL_ARG_CALL((f4v(L"foo", 2, true, L"bar")));
                TEST_NULL_ARG_CALL((f3v(L"foo", 2, true)));
                TEST_NULL_ARG_CALL((f2v(L"foo", 2)));
                TEST_NULL_ARG_CALL((f1v(L"foo")));
                TEST_NULL_ARG_CALL((f0v()));
                TEST_NULL_ARG_CALL((f8c.construct(L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
                TEST_NULL_ARG_CALL((f7c.construct(L"foo", 2, true, L"bar", 5, false, L"baz")));
                TEST_NULL_ARG_CALL((f6c.construct(L"foo", 2, true, L"bar", 5, false)));
                TEST_NULL_ARG_CALL((f5c.construct(L"foo", 2, true, L"bar", 5)));
                TEST_NULL_ARG_CALL((f4c.construct(L"foo", 2, true, L"bar")));
                TEST_NULL_ARG_CALL((f3c.construct(L"foo", 2, true)));
                TEST_NULL_ARG_CALL((f2c.construct(L"foo", 2)));
                TEST_NULL_ARG_CALL((f1c.construct(L"foo")));
                TEST_NULL_ARG_CALL((f0c.construct()));
            }
            runtime.dispose();
        }

        static double callback8(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7, double p8)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, (std::wstring)L"baz");
            Assert::AreEqual(p8, 8.0);

            return 8;
        }

        static double callback7(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, (std::wstring)L"baz");

            return 7;
        }

        static double callback6(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);

            return 6;
        }

        static double callback5(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);

            return 5;
        }

        static double callback4(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");

            return 4;
        }

        static double callback3(const jsrt::call_info &info, std::wstring p1, double p2, bool p3)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);

            return 3;
        }

        static double callback2(const jsrt::call_info &info, std::wstring p1, double p2)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);

            return 2;
        }

        static double callback1(const jsrt::call_info &info, std::wstring p1)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");

            return 1;
        }

        static double callback0(const jsrt::call_info &info)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            return 0;
        }

        static void callback8v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7, double p8)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, (std::wstring)L"baz");
            Assert::AreEqual(p8, 8.0);
        }

        static void callback7v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, (std::wstring)L"baz");
        }

        static void callback6v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
        }

        static void callback5v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
        }

        static void callback4v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
        }

        static void callback3v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
        }

        static void callback2v(const jsrt::call_info &info, std::wstring p1, double p2)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
        }

        static void callback1v(const jsrt::call_info &info, std::wstring p1)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
        }

        static void callback0v(const jsrt::call_info &info)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());
        }

        MY_TEST_METHOD(strongly_typed, "Test strongly typed functions.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create((void *) 0xdeadbeef);

                auto f8 = jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(this_value, callback8);
                double result = f8(L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                Assert::AreEqual(result, 8.0);

                auto f7 = jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(this_value, callback7);
                result = f7(L"foo", 2, true, L"bar", 5, false, L"baz");
                Assert::AreEqual(result, 7.0);

                auto f6 = jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double, bool>::create(this_value, callback6);
                result = f6(L"foo", 2, true, L"bar", 5, false);
                Assert::AreEqual(result, 6.0);

                auto f5 = jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring, double>::create(this_value, callback5);
                result = f5(L"foo", 2, true, L"bar", 5);
                Assert::AreEqual(result, 5.0);

                auto f4 = jsrt::bound_function<jsrt::object, double, std::wstring, double, bool, std::wstring>::create(this_value, callback4);
                result = f4(L"foo", 2, true, L"bar");
                Assert::AreEqual(result, 4.0);

                auto f3 = jsrt::bound_function<jsrt::object, double, std::wstring, double, bool>::create(this_value, callback3);
                result = f3(L"foo", 2, true);
                Assert::AreEqual(result, 3.0);

                auto f2 = jsrt::bound_function<jsrt::object, double, std::wstring, double>::create(this_value, callback2);
                result = f2(L"foo", 2);
                Assert::AreEqual(result, 2.0);

                auto f1 = jsrt::bound_function<jsrt::object, double, std::wstring>::create(this_value, callback1);
                result = f1(L"foo");
                Assert::AreEqual(result, 1.0);

                auto f0 = jsrt::bound_function<jsrt::object, double>::create(this_value, callback0);
                result = f0();
                Assert::AreEqual(result, 0.0);

                auto f8v = jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(this_value, callback8v);
                f8v(L"foo", 2, true, L"bar", 5, false, L"baz", 8);

                auto f7v = jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(this_value, callback7v);
                f7v(L"foo", 2, true, L"bar", 5, false, L"baz");

                auto f6v = jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double, bool>::create(this_value, callback6v);
                f6v(L"foo", 2, true, L"bar", 5, false);

                auto f5v = jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring, double>::create(this_value, callback5v);
                f5v(L"foo", 2, true, L"bar", 5);

                auto f4v = jsrt::bound_function<jsrt::object, void, std::wstring, double, bool, std::wstring>::create(this_value, callback4v);
                f4v(L"foo", 2, true, L"bar");

                auto f3v = jsrt::bound_function<jsrt::object, void, std::wstring, double, bool>::create(this_value, callback3v);
                f3v(L"foo", 2, true);

                auto f2v = jsrt::bound_function<jsrt::object, void, std::wstring, double>::create(this_value, callback2v);
                f2v(L"foo", 2);

                auto f1v = jsrt::bound_function<jsrt::object, void, std::wstring>::create(this_value, callback1v);
                f1v(L"foo");

                auto f0v = jsrt::bound_function<jsrt::object, void>::create(this_value, callback0v);
                f0v();
            }
            runtime.dispose();
        }

        static jsrt::object callback8c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7, double p8)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, (std::wstring)L"baz");
            Assert::AreEqual(p8, 8.0);

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        static jsrt::object callback7c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, (std::wstring)L"baz");

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        static jsrt::object callback6c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        static jsrt::object callback5c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5.0);

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        static jsrt::object callback4c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        static jsrt::object callback3c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        static jsrt::object callback2c(const jsrt::call_info &info, std::wstring p1, double p2)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2.0);

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        static jsrt::object callback1c(const jsrt::call_info &info, std::wstring p1)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        static jsrt::object callback0c(const jsrt::call_info &info)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            return jsrt::external_object::create((void *) 0xdeadbeef);
        }

        MY_TEST_METHOD(strongly_typed_constructors, "Test strongly typed constructor functions.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create((void *) 0xdeadbeef);
                auto f8 = jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(this_value, callback8c);
                jsrt::object result = f8.construct(L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);

                auto f7 = jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(this_value, callback7c);
                result = f7.construct(L"foo", 2, true, L"bar", 5, false, L"baz");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);

                auto f6 = jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double, bool>::create(this_value, callback6c);
                result = f6.construct(L"foo", 2, true, L"bar", 5, false);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);

                auto f5 = jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring, double>::create(this_value, callback5c);
                result = f5.construct(L"foo", 2, true, L"bar", 5);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);

                auto f4 = jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool, std::wstring>::create(this_value, callback4c);
                result = f4.construct(L"foo", 2, true, L"bar");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);

                auto f3 = jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double, bool>::create(this_value, callback3c);
                result = f3.construct(L"foo", 2, true);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);

                auto f2 = jsrt::bound_function<jsrt::object, jsrt::object, std::wstring, double>::create(this_value, callback2c);
                result = f2.construct(L"foo", 2);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);

                auto f1 = jsrt::bound_function<jsrt::object, jsrt::object, std::wstring>::create(this_value, callback1c);
                result = f1.construct(L"foo");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);

                auto f0 = jsrt::bound_function<jsrt::object, jsrt::object>::create(this_value, callback0c);
                result = f0.construct();
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadbeef);
            }
            runtime.dispose();
        }

        static jsrt::object callback0cp(const jsrt::call_info &info)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            return jsrt::object();
        }

        MY_TEST_METHOD(constructor_prototype, "Test strongly typed constructor functions with prototypes.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create((void *) 0xdeadbeef);
                jsrt::object proto = jsrt::object::create();
                auto f0 = jsrt::bound_function<jsrt::object, jsrt::object>::create(this_value, callback0cp);
                f0.set_constructor_prototype(proto);
                jsrt::object result = f0.construct();
                Assert::IsTrue(result.is_valid());
                Assert::IsFalse(result.is_external());
                Assert::AreEqual(result.prototype().handle(), f0.constructor_prototype().handle());
            }
            runtime.dispose();
        }
    };
}