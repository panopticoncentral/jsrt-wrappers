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
    TEST_CLASS(function)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty function handle.")
        {
            jsrt::function_base handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

		static jsrt::value base_callback(const jsrt::call_info &info, const std::vector<jsrt::value> &arguments)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);

			Assert::AreEqual(arguments.size(), static_cast<size_t>(2));
			Assert::AreEqual(arguments[0].type(), JsNumber);
			Assert::AreEqual(static_cast<jsrt::number>(arguments[0]).as_int(), 1);
			Assert::AreEqual(arguments[1].type(), JsNumber);
			Assert::AreEqual(static_cast<jsrt::number>(arguments[1]).as_int(), 2);

			if (static_cast<jsrt::object>(info.this_value()).is_external())
			{
				void *data = static_cast<jsrt::external_object>(info.this_value()).data();
				Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
				Assert::IsFalse(info.is_construct_call());
				return jsrt::number::create(2);
			}
			else
			{
				Assert::IsTrue(info.is_construct_call());
				return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadc0de));
			}
		}

		static double callback8(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7, double p8)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
			Assert::AreEqual(p5, 5.0);
			Assert::AreEqual(p6, false);
			Assert::AreEqual(p7, static_cast<std::wstring>(L"baz"));
			Assert::AreEqual(p8, 8.0);

			return 8;
		}

		static double callback7(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
			Assert::AreEqual(p5, 5.0);
			Assert::AreEqual(p6, false);
			Assert::AreEqual(p7, static_cast<std::wstring>(L"baz"));

			return 7;
		}

		static double callback6(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
			Assert::AreEqual(p5, 5.0);
			Assert::AreEqual(p6, false);

			return 6;
		}

		static double callback5(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
			Assert::AreEqual(p5, 5.0);

			return 5;
		}

		static double callback4(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));

			return 4;
		}

		static double callback3(const jsrt::call_info &info, std::wstring p1, double p2, bool p3)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);

			return 3;
		}

		static double callback2(const jsrt::call_info &info, std::wstring p1, double p2)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);

			return 2;
		}

		static double callback1(const jsrt::call_info &info, std::wstring p1)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));

			return 1;
		}

		static double callback0(const jsrt::call_info &info)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			return 0;
		}

		static void callback8v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7, double p8)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
			Assert::AreEqual(p5, 5.0);
			Assert::AreEqual(p6, false);
			Assert::AreEqual(p7, static_cast<std::wstring>(L"baz"));
			Assert::AreEqual(p8, 8.0);
		}

		static void callback7v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
			Assert::AreEqual(p5, 5.0);
			Assert::AreEqual(p6, false);
			Assert::AreEqual(p7, static_cast<std::wstring>(L"baz"));
		}

		static void callback6v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
			Assert::AreEqual(p5, 5.0);
			Assert::AreEqual(p6, false);
		}

		static void callback5v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
			Assert::AreEqual(p5, 5.0);
		}

		static void callback4v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
			Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
		}

		static void callback3v(const jsrt::call_info &info, std::wstring p1, double p2, bool p3)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
			Assert::AreEqual(p3, true);
		}

		static void callback2v(const jsrt::call_info &info, std::wstring p1, double p2)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
			Assert::AreEqual(p2, 2.0);
		}

		static void callback1v(const jsrt::call_info &info, std::wstring p1)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());

			Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
		}

		static void callback0v(const jsrt::call_info &info)
		{
			Assert::AreEqual(info.callee().type(), JsFunction);
			Assert::AreEqual(info.this_value().type(), JsObject);
			Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
			void *data = static_cast<jsrt::external_object>(info.this_value()).data();
			Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
			Assert::IsFalse(info.is_construct_call());
		}
		
		MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();

            jsrt::function_base base_function;
            jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8;
            jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7;
            jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool> f6;
            jsrt::function<double, std::wstring, double, bool, std::wstring, double> f5;
            jsrt::function<double, std::wstring, double, bool, std::wstring> f4;
            jsrt::function<double, std::wstring, double, bool> f3;
            jsrt::function<double, std::wstring, double> f2;
            jsrt::function<double, std::wstring> f1;
            jsrt::function<double> f0;
            jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8v;
            jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7v;
            jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool> f6v;
            jsrt::function<void, std::wstring, double, bool, std::wstring, double> f5v;
            jsrt::function<void, std::wstring, double, bool, std::wstring> f4v;
            jsrt::function<void, std::wstring, double, bool> f3v;
            jsrt::function<void, std::wstring, double> f2v;
            jsrt::function<void, std::wstring> f1v;
            jsrt::function<void> f0v;
            jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8c;
            jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7c;
            jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool> f6c;
            jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double> f5c;
            jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring> f4c;
            jsrt::function<jsrt::object, std::wstring, double, bool> f3c;
            jsrt::function<jsrt::object, std::wstring, double> f2c;
            jsrt::function<jsrt::object, std::wstring> f1c;
            jsrt::function<jsrt::object> f0c;
            jsrt::object this_value;

            TEST_NO_CONTEXT_CALL(jsrt::function_base::create(base_callback));
            TEST_NO_CONTEXT_CALL(base_function(jsrt::value(), {}));
            TEST_NO_CONTEXT_CALL(base_function.construct({}));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(callback8)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback8)));
            TEST_NO_CONTEXT_CALL((f8(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(callback7)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(L"foo", callback7)));
            TEST_NO_CONTEXT_CALL((f7(this_value, L"foo", 2, true, L"bar", 5, false, L"baz")));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool>::create(callback6)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool>::create(L"foo", callback6)));
            TEST_NO_CONTEXT_CALL((f6(this_value, L"foo", 2, true, L"bar", 5, false)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring, double>::create(callback5)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback5)));
            TEST_NO_CONTEXT_CALL((f5(this_value, L"foo", 2, true, L"bar", 5)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring>::create(callback4)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool, std::wstring>::create(L"foo", callback4)));
            TEST_NO_CONTEXT_CALL((f4(this_value, L"foo", 2, true, L"bar")));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool>::create(callback3)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double, bool>::create(L"foo", callback3)));
            TEST_NO_CONTEXT_CALL((f3(this_value, L"foo", 2, true)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double>::create(callback2)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring, double>::create(L"foo", callback2)));
            TEST_NO_CONTEXT_CALL((f2(this_value, L"foo", 2)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring>::create(callback1)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double, std::wstring>::create(L"foo", callback1)));
            TEST_NO_CONTEXT_CALL((f1(this_value, L"foo")));
            TEST_NO_CONTEXT_CALL((jsrt::function<double>::create(callback0)));
            TEST_NO_CONTEXT_CALL((jsrt::function<double>::create(L"foo", callback0)));
            TEST_NO_CONTEXT_CALL((f0(this_value)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(callback8v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback8v)));
            TEST_NO_CONTEXT_CALL((f8v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(callback7v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(L"foo", callback7v)));
            TEST_NO_CONTEXT_CALL((f7v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz")));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool>::create(callback6v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool>::create(L"foo", callback6v)));
            TEST_NO_CONTEXT_CALL((f6v(this_value, L"foo", 2, true, L"bar", 5, false)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring, double>::create(callback5v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback5v)));
            TEST_NO_CONTEXT_CALL((f5v(this_value, L"foo", 2, true, L"bar", 5)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring>::create(callback4v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool, std::wstring>::create(L"foo", callback4v)));
            TEST_NO_CONTEXT_CALL((f4v(this_value, L"foo", 2, true, L"bar")));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool>::create(callback3v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double, bool>::create(L"foo", callback3v)));
            TEST_NO_CONTEXT_CALL((f3v(this_value, L"foo", 2, true)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double>::create(callback2v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring, double>::create(L"foo", callback2v)));
            TEST_NO_CONTEXT_CALL((f2v(this_value, L"foo", 2)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring>::create(callback1v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void, std::wstring>::create(L"foo", callback1v)));
            TEST_NO_CONTEXT_CALL((f1v(this_value, L"foo")));
            TEST_NO_CONTEXT_CALL((jsrt::function<void>::create(callback0v)));
            TEST_NO_CONTEXT_CALL((jsrt::function<void>::create(L"foo", callback0v)));
            TEST_NO_CONTEXT_CALL((f0v(this_value)));
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
                jsrt::function_base func;
                jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8;
                jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7;
                jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool> f6;
                jsrt::function<double, std::wstring, double, bool, std::wstring, double> f5;
                jsrt::function<double, std::wstring, double, bool, std::wstring> f4;
                jsrt::function<double, std::wstring, double, bool> f3;
                jsrt::function<double, std::wstring, double> f2;
                jsrt::function<double, std::wstring> f1;
                jsrt::function<double> f0;
                jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8v;
                jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7v;
                jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool> f6v;
                jsrt::function<void, std::wstring, double, bool, std::wstring, double> f5v;
                jsrt::function<void, std::wstring, double, bool, std::wstring> f4v;
                jsrt::function<void, std::wstring, double, bool> f3v;
                jsrt::function<void, std::wstring, double> f2v;
                jsrt::function<void, std::wstring> f1v;
                jsrt::function<void> f0v;
                jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double> f8c;
                jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring> f7c;
                jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool> f6c;
                jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double> f5c;
                jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring> f4c;
                jsrt::function<jsrt::object, std::wstring, double, bool> f3c;
                jsrt::function<jsrt::object, std::wstring, double> f2c;
                jsrt::function<jsrt::object, std::wstring> f1c;
                jsrt::function<jsrt::object> f0c;
                TEST_INVALID_ARG_CALL(func(jsrt::value(), {}));
                TEST_INVALID_ARG_CALL(func.construct({}));
                TEST_INVALID_ARG_CALL((f8(jsrt::value(), L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
                TEST_INVALID_ARG_CALL((f7(jsrt::value(), L"foo", 2, true, L"bar", 5, false, L"baz")));
                TEST_INVALID_ARG_CALL((f6(jsrt::value(), L"foo", 2, true, L"bar", 5, false)));
                TEST_INVALID_ARG_CALL((f5(jsrt::value(), L"foo", 2, true, L"bar", 5)));
                TEST_INVALID_ARG_CALL((f4(jsrt::value(), L"foo", 2, true, L"bar")));
                TEST_INVALID_ARG_CALL((f3(jsrt::value(), L"foo", 2, true)));
                TEST_INVALID_ARG_CALL((f2(jsrt::value(), L"foo", 2)));
                TEST_INVALID_ARG_CALL((f1(jsrt::value(), L"foo")));
                TEST_INVALID_ARG_CALL((f0(jsrt::value())));
                TEST_INVALID_ARG_CALL((f8v(jsrt::value(), L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
                TEST_INVALID_ARG_CALL((f7v(jsrt::value(), L"foo", 2, true, L"bar", 5, false, L"baz")));
                TEST_INVALID_ARG_CALL((f6v(jsrt::value(), L"foo", 2, true, L"bar", 5, false)));
                TEST_INVALID_ARG_CALL((f5v(jsrt::value(), L"foo", 2, true, L"bar", 5)));
                TEST_INVALID_ARG_CALL((f4v(jsrt::value(), L"foo", 2, true, L"bar")));
                TEST_INVALID_ARG_CALL((f3v(jsrt::value(), L"foo", 2, true)));
                TEST_INVALID_ARG_CALL((f2v(jsrt::value(), L"foo", 2)));
                TEST_INVALID_ARG_CALL((f1v(jsrt::value(), L"foo")));
                TEST_INVALID_ARG_CALL((f0v(jsrt::value())));
                TEST_INVALID_ARG_CALL((f8c.construct(L"foo", 2, true, L"bar", 5, false, L"baz", 8)));
                TEST_INVALID_ARG_CALL((f7c.construct(L"foo", 2, true, L"bar", 5, false, L"baz")));
                TEST_INVALID_ARG_CALL((f6c.construct(L"foo", 2, true, L"bar", 5, false)));
                TEST_INVALID_ARG_CALL((f5c.construct(L"foo", 2, true, L"bar", 5)));
                TEST_INVALID_ARG_CALL((f4c.construct(L"foo", 2, true, L"bar")));
                TEST_INVALID_ARG_CALL((f3c.construct(L"foo", 2, true)));
                TEST_INVALID_ARG_CALL((f2c.construct(L"foo", 2)));
                TEST_INVALID_ARG_CALL((f1c.construct(L"foo")));
                TEST_INVALID_ARG_CALL((f0c.construct()));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(call_info, "Test call_info class.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::call_info info;
                Assert::IsFalse(info.callee().is_valid());
                Assert::IsFalse(info.this_value().is_valid());
                Assert::IsFalse(info.is_construct_call());
                info = jsrt::call_info(jsrt::number::create(1), jsrt::number::create(2), true);
                Assert::IsTrue(info.callee().is_valid());
                Assert::IsTrue(info.this_value().is_valid());
                Assert::IsTrue(info.is_construct_call());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(base, "Test function_base.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::function_base func = jsrt::function_base::create(base_callback);
                jsrt::value result = func(jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef)), { jsrt::number::create(1), jsrt::number::create(2) });
                Assert::AreEqual(result.type(), JsNumber);
                Assert::AreEqual(static_cast<jsrt::number>(result).as_int(), 2);

                result = func.construct({ jsrt::number::create(1), jsrt::number::create(2) });
                Assert::AreEqual(result.type(), JsObject);
                Assert::IsTrue(static_cast<jsrt::object>(result).is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadc0de));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(base_named, "Test named function_base.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::function_base func = jsrt::function_base::create(L"foo", base_callback);
                jsrt::value result = func(jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef)), { jsrt::number::create(1), jsrt::number::create(2) });
                Assert::AreEqual(result.type(), JsNumber);
                Assert::AreEqual(static_cast<jsrt::number>(result).as_int(), 2);

                result = func.construct({ jsrt::number::create(1), jsrt::number::create(2) });
                Assert::AreEqual(result.type(), JsObject);
                Assert::IsTrue(static_cast<jsrt::object>(result).is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadc0de));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(strongly_typed, "Test strongly typed functions.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));

                auto f8 = jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(callback8);
                double result = f8(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                Assert::AreEqual(result, 8.0);

                auto f7 = jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(callback7);
                result = f7(this_value, L"foo", 2, true, L"bar", 5, false, L"baz");
                Assert::AreEqual(result, 7.0);

                auto f6 = jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool>::create(callback6);
                result = f6(this_value, L"foo", 2, true, L"bar", 5, false);
                Assert::AreEqual(result, 6.0);

                auto f5 = jsrt::function<double, std::wstring, double, bool, std::wstring, double>::create(callback5);
                result = f5(this_value, L"foo", 2, true, L"bar", 5);
                Assert::AreEqual(result, 5.0);

                auto f4 = jsrt::function<double, std::wstring, double, bool, std::wstring>::create(callback4);
                result = f4(this_value, L"foo", 2, true, L"bar");
                Assert::AreEqual(result, 4.0);

                auto f3 = jsrt::function<double, std::wstring, double, bool>::create(callback3);
                result = f3(this_value, L"foo", 2, true);
                Assert::AreEqual(result, 3.0);

                auto f2 = jsrt::function<double, std::wstring, double>::create(callback2);
                result = f2(this_value, L"foo", 2);
                Assert::AreEqual(result, 2.0);

                auto f1 = jsrt::function<double, std::wstring>::create(callback1);
                result = f1(this_value, L"foo");
                Assert::AreEqual(result, 1.0);

                auto f0 = jsrt::function<double>::create(callback0);
                result = f0(this_value);
                Assert::AreEqual(result, 0.0);

                auto f8v = jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(callback8v);
                f8v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);

                auto f7v = jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(callback7v);
                f7v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz");

                auto f6v = jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool>::create(callback6v);
                f6v(this_value, L"foo", 2, true, L"bar", 5, false);

                auto f5v = jsrt::function<void, std::wstring, double, bool, std::wstring, double>::create(callback5v);
                f5v(this_value, L"foo", 2, true, L"bar", 5);

                auto f4v = jsrt::function<void, std::wstring, double, bool, std::wstring>::create(callback4v);
                f4v(this_value, L"foo", 2, true, L"bar");

                auto f3v = jsrt::function<void, std::wstring, double, bool>::create(callback3v);
                f3v(this_value, L"foo", 2, true);

                auto f2v = jsrt::function<void, std::wstring, double>::create(callback2v);
                f2v(this_value, L"foo", 2);

                auto f1v = jsrt::function<void, std::wstring>::create(callback1v);
                f1v(this_value, L"foo");

                auto f0v = jsrt::function<void>::create(callback0v);
                f0v(this_value);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(strongly_typed_named, "Test named strongly typed functions.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));

                auto f8 = jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback8);
                double result = f8(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                Assert::AreEqual(result, 8.0);

                auto f7 = jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(L"foo", callback7);
                result = f7(this_value, L"foo", 2, true, L"bar", 5, false, L"baz");
                Assert::AreEqual(result, 7.0);

                auto f6 = jsrt::function<double, std::wstring, double, bool, std::wstring, double, bool>::create(L"foo", callback6);
                result = f6(this_value, L"foo", 2, true, L"bar", 5, false);
                Assert::AreEqual(result, 6.0);

                auto f5 = jsrt::function<double, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback5);
                result = f5(this_value, L"foo", 2, true, L"bar", 5);
                Assert::AreEqual(result, 5.0);

                auto f4 = jsrt::function<double, std::wstring, double, bool, std::wstring>::create(L"foo", callback4);
                result = f4(this_value, L"foo", 2, true, L"bar");
                Assert::AreEqual(result, 4.0);

                auto f3 = jsrt::function<double, std::wstring, double, bool>::create(L"foo", callback3);
                result = f3(this_value, L"foo", 2, true);
                Assert::AreEqual(result, 3.0);

                auto f2 = jsrt::function<double, std::wstring, double>::create(L"foo", callback2);
                result = f2(this_value, L"foo", 2);
                Assert::AreEqual(result, 2.0);

                auto f1 = jsrt::function<double, std::wstring>::create(L"foo", callback1);
                result = f1(this_value, L"foo");
                Assert::AreEqual(result, 1.0);

                auto f0 = jsrt::function<double>::create(L"foo", callback0);
                result = f0(this_value);
                Assert::AreEqual(result, 0.0);

                auto f8v = jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback8v);
                f8v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);

                auto f7v = jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(L"foo", callback7v);
                f7v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz");

                auto f6v = jsrt::function<void, std::wstring, double, bool, std::wstring, double, bool>::create(L"foo", callback6v);
                f6v(this_value, L"foo", 2, true, L"bar", 5, false);

                auto f5v = jsrt::function<void, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback5v);
                f5v(this_value, L"foo", 2, true, L"bar", 5);

                auto f4v = jsrt::function<void, std::wstring, double, bool, std::wstring>::create(L"foo", callback4v);
                f4v(this_value, L"foo", 2, true, L"bar");

                auto f3v = jsrt::function<void, std::wstring, double, bool>::create(L"foo", callback3v);
                f3v(this_value, L"foo", 2, true);

                auto f2v = jsrt::function<void, std::wstring, double>::create(L"foo", callback2v);
                f2v(this_value, L"foo", 2);

                auto f1v = jsrt::function<void, std::wstring>::create(L"foo", callback1v);
                f1v(this_value, L"foo");

                auto f0v = jsrt::function<void>::create(L"foo", callback0v);
                f0v(this_value);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(strongly_typed_generic, "Test strongly typed functions with generic ::create.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));

                auto f8 = jsrt::function_base::create(callback8);
                double result = f8(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                Assert::AreEqual(result, 8.0);

                auto f7 = jsrt::function_base::create(callback7);
                result = f7(this_value, L"foo", 2, true, L"bar", 5, false, L"baz");
                Assert::AreEqual(result, 7.0);

                auto f6 = jsrt::function_base::create(callback6);
                result = f6(this_value, L"foo", 2, true, L"bar", 5, false);
                Assert::AreEqual(result, 6.0);

                auto f5 = jsrt::function_base::create(callback5);
                result = f5(this_value, L"foo", 2, true, L"bar", 5);
                Assert::AreEqual(result, 5.0);

                auto f4 = jsrt::function_base::create(callback4);
                result = f4(this_value, L"foo", 2, true, L"bar");
                Assert::AreEqual(result, 4.0);

                auto f3 = jsrt::function_base::create(callback3);
                result = f3(this_value, L"foo", 2, true);
                Assert::AreEqual(result, 3.0);

                auto f2 = jsrt::function_base::create(callback2);
                result = f2(this_value, L"foo", 2);
                Assert::AreEqual(result, 2.0);

                auto f1 = jsrt::function_base::create(callback1);
                result = f1(this_value, L"foo");
                Assert::AreEqual(result, 1.0);

                auto f0 = jsrt::function_base::create(callback0);
                result = f0(this_value);
                Assert::AreEqual(result, 0.0);

                auto f8v = jsrt::function_base::create(callback8v);
                f8v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);

                auto f7v = jsrt::function_base::create(callback7v);
                f7v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz");

                auto f6v = jsrt::function_base::create(callback6v);
                f6v(this_value, L"foo", 2, true, L"bar", 5, false);

                auto f5v = jsrt::function_base::create(callback5v);
                f5v(this_value, L"foo", 2, true, L"bar", 5);

                auto f4v = jsrt::function_base::create(callback4v);
                f4v(this_value, L"foo", 2, true, L"bar");

                auto f3v = jsrt::function_base::create(callback3v);
                f3v(this_value, L"foo", 2, true);

                auto f2v = jsrt::function_base::create(callback2v);
                f2v(this_value, L"foo", 2);

                auto f1v = jsrt::function_base::create(callback1v);
                f1v(this_value, L"foo");

                auto f0v = jsrt::function_base::create(callback0v);
                f0v(this_value);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(strongly_typed_generic_named, "Test named strongly typed functions with generic ::create.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));

                auto f8 = jsrt::function_base::create(L"foo", callback8);
                double result = f8(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                Assert::AreEqual(result, 8.0);

                auto f7 = jsrt::function_base::create(L"foo", callback7);
                result = f7(this_value, L"foo", 2, true, L"bar", 5, false, L"baz");
                Assert::AreEqual(result, 7.0);

                auto f6 = jsrt::function_base::create(L"foo", callback6);
                result = f6(this_value, L"foo", 2, true, L"bar", 5, false);
                Assert::AreEqual(result, 6.0);

                auto f5 = jsrt::function_base::create(L"foo", callback5);
                result = f5(this_value, L"foo", 2, true, L"bar", 5);
                Assert::AreEqual(result, 5.0);

                auto f4 = jsrt::function_base::create(L"foo", callback4);
                result = f4(this_value, L"foo", 2, true, L"bar");
                Assert::AreEqual(result, 4.0);

                auto f3 = jsrt::function_base::create(L"foo", callback3);
                result = f3(this_value, L"foo", 2, true);
                Assert::AreEqual(result, 3.0);

                auto f2 = jsrt::function_base::create(L"foo", callback2);
                result = f2(this_value, L"foo", 2);
                Assert::AreEqual(result, 2.0);

                auto f1 = jsrt::function_base::create(L"foo", callback1);
                result = f1(this_value, L"foo");
                Assert::AreEqual(result, 1.0);

                auto f0 = jsrt::function_base::create(L"foo", callback0);
                result = f0(this_value);
                Assert::AreEqual(result, 0.0);

                auto f8v = jsrt::function_base::create(L"foo", callback8v);
                f8v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);

                auto f7v = jsrt::function_base::create(L"foo", callback7v);
                f7v(this_value, L"foo", 2, true, L"bar", 5, false, L"baz");

                auto f6v = jsrt::function_base::create(L"foo", callback6v);
                f6v(this_value, L"foo", 2, true, L"bar", 5, false);

                auto f5v = jsrt::function_base::create(L"foo", callback5v);
                f5v(this_value, L"foo", 2, true, L"bar", 5);

                auto f4v = jsrt::function_base::create(L"foo", callback4v);
                f4v(this_value, L"foo", 2, true, L"bar");

                auto f3v = jsrt::function_base::create(L"foo", callback3v);
                f3v(this_value, L"foo", 2, true);

                auto f2v = jsrt::function_base::create(L"foo", callback2v);
                f2v(this_value, L"foo", 2);

                auto f1v = jsrt::function_base::create(L"foo", callback1v);
                f1v(this_value, L"foo");

                auto f0v = jsrt::function_base::create(L"foo", callback0v);
                f0v(this_value);
            }
            runtime.dispose();
        }

        static void callback8o(const jsrt::call_info &info, jsrt::optional<std::wstring> p1, jsrt::optional<double> p2, jsrt::optional<bool> p3, jsrt::optional<std::wstring> p4, jsrt::optional<double> p5, jsrt::optional<bool> p6, jsrt::optional<std::wstring> p7, std::vector<double> p8)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
            void *data = static_cast<jsrt::external_object>(info.this_value()).data();
            Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
            Assert::IsFalse(info.is_construct_call());

            if (p1.has_value())
            {
                Assert::AreEqual(p1.value(), static_cast<std::wstring>(L"foo"));
            }
            if (p2.has_value())
            {
                Assert::AreEqual(p2.value(), 2.0);
            }
            if (p3.has_value())
            {
                Assert::AreEqual(p3.value(), true);
            }
            if (p4.has_value())
            {
                Assert::AreEqual(p4.value(), static_cast<std::wstring>(L"bar"));
            }
            if (p5.has_value())
            {
                Assert::AreEqual(p5.value(), 5.0);
            }
            if (p6.has_value())
            {
                Assert::AreEqual(p6.value(), false);
            }
            if (p7.has_value())
            {
                Assert::AreEqual(p7.value(), static_cast<std::wstring>(L"baz"));
            }
            for (auto &v : p8)
            {
                Assert::AreEqual(v, 8.0);
            }
        }

        static void callback7o(const jsrt::call_info &info, jsrt::optional<std::wstring> p1, jsrt::optional<double> p2, jsrt::optional<bool> p3, jsrt::optional<std::wstring> p4, jsrt::optional<double> p5, jsrt::optional<bool> p6, std::vector<std::wstring> p7)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
            void *data = static_cast<jsrt::external_object>(info.this_value()).data();
            Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
            Assert::IsFalse(info.is_construct_call());

            if (p1.has_value())
            {
                Assert::AreEqual(p1.value(), static_cast<std::wstring>(L"foo"));
            }
            if (p2.has_value())
            {
                Assert::AreEqual(p2.value(), 2.0);
            }
            if (p3.has_value())
            {
                Assert::AreEqual(p3.value(), true);
            }
            if (p4.has_value())
            {
                Assert::AreEqual(p4.value(), static_cast<std::wstring>(L"bar"));
            }
            if (p5.has_value())
            {
                Assert::AreEqual(p5.value(), 5.0);
            }
            if (p6.has_value())
            {
                Assert::AreEqual(p6.value(), false);
            }
            for (auto &v : p7)
            {
                Assert::AreEqual(v, static_cast<std::wstring>(L"baz"));
            }
        }

        static void callback6o(const jsrt::call_info &info, jsrt::optional<std::wstring> p1, jsrt::optional<double> p2, jsrt::optional<bool> p3, jsrt::optional<std::wstring> p4, jsrt::optional<double> p5, std::vector<bool> p6)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
            void *data = static_cast<jsrt::external_object>(info.this_value()).data();
            Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
            Assert::IsFalse(info.is_construct_call());

            if (p1.has_value())
            {
                Assert::AreEqual(p1.value(), static_cast<std::wstring>(L"foo"));
            }
            if (p2.has_value())
            {
                Assert::AreEqual(p2.value(), 2.0);
            }
            if (p3.has_value())
            {
                Assert::AreEqual(p3.value(), true);
            }
            if (p4.has_value())
            {
                Assert::AreEqual(p4.value(), static_cast<std::wstring>(L"bar"));
            }
            if (p5.has_value())
            {
                Assert::AreEqual(p5.value(), 5.0);
            }
            for (auto &v : p6)
            {
                Assert::AreEqual(static_cast<bool>(v), false);
            }
        }

        static void callback5o(const jsrt::call_info &info, jsrt::optional<std::wstring> p1, jsrt::optional<double> p2, jsrt::optional<bool> p3, jsrt::optional<std::wstring> p4, std::vector<double> p5)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
            void *data = static_cast<jsrt::external_object>(info.this_value()).data();
            Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
            Assert::IsFalse(info.is_construct_call());

            if (p1.has_value())
            {
                Assert::AreEqual(p1.value(), static_cast<std::wstring>(L"foo"));
            }
            if (p2.has_value())
            {
                Assert::AreEqual(p2.value(), 2.0);
            }
            if (p3.has_value())
            {
                Assert::AreEqual(p3.value(), true);
            }
            if (p4.has_value())
            {
                Assert::AreEqual(p4.value(), static_cast<std::wstring>(L"bar"));
            }
            for (auto &v : p5)
            {
                Assert::AreEqual(v, 5.0);
            }
        }

        static void callback4o(const jsrt::call_info &info, jsrt::optional<std::wstring> p1, jsrt::optional<double> p2, jsrt::optional<bool> p3, std::vector<std::wstring> p4)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
            void *data = static_cast<jsrt::external_object>(info.this_value()).data();
            Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
            Assert::IsFalse(info.is_construct_call());

            if (p1.has_value())
            {
                Assert::AreEqual(p1.value(), static_cast<std::wstring>(L"foo"));
            }
            if (p2.has_value())
            {
                Assert::AreEqual(p2.value(), 2.0);
            }
            if (p3.has_value())
            {
                Assert::AreEqual(p3.value(), true);
            }
            for (auto &v : p4)
            {
                Assert::AreEqual(v, static_cast<std::wstring>(L"bar"));
            }
        }

        static void callback3o(const jsrt::call_info &info, jsrt::optional<std::wstring> p1, jsrt::optional<double> p2, std::vector<bool> p3)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
            void *data = static_cast<jsrt::external_object>(info.this_value()).data();
            Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
            Assert::IsFalse(info.is_construct_call());

            if (p1.has_value())
            {
                Assert::AreEqual(p1.value(), static_cast<std::wstring>(L"foo"));
            }
            if (p2.has_value())
            {
                Assert::AreEqual(p2.value(), 2.0);
            }
            for (auto &v : p3)
            {
                Assert::AreEqual(static_cast<bool>(v), true);
            }
        }

        static void callback2o(const jsrt::call_info &info, jsrt::optional<std::wstring> p1, std::vector<double> p2)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
            void *data = static_cast<jsrt::external_object>(info.this_value()).data();
            Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
            Assert::IsFalse(info.is_construct_call());

            if (p1.has_value())
            {
                Assert::AreEqual(p1.value(), static_cast<std::wstring>(L"foo"));
            }
            for (auto &v : p2)
            {
                Assert::AreEqual(v, 2.0);
            }
        }

        static void callback1o(const jsrt::call_info &info, std::vector<std::wstring> p1)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(static_cast<jsrt::object>(info.this_value()).is_external());
            void *data = static_cast<jsrt::external_object>(info.this_value()).data();
            Assert::AreEqual(data, reinterpret_cast<void *>(0xdeadbeef));
            Assert::IsFalse(info.is_construct_call());

            for (auto &v : p1)
            {
                Assert::AreEqual(v, static_cast<std::wstring>(L"foo"));
            }
        }

        MY_TEST_METHOD(strongly_typed_optional, "Test strongly typed functions with optional arguments.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));

                auto f8o = jsrt::function<void, jsrt::optional<std::wstring>, jsrt::optional<double>, jsrt::optional<bool>, jsrt::optional<std::wstring>, jsrt::optional<double>, jsrt::optional<bool>, jsrt::optional<std::wstring>, std::vector<double>>::create(callback8o);
                f8o(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", { 8 });
                f8o(this_value, jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), {});

                auto f7o = jsrt::function<void, jsrt::optional<std::wstring>, jsrt::optional<double>, jsrt::optional<bool>, jsrt::optional<std::wstring>, jsrt::optional<double>, jsrt::optional<bool>, std::vector<std::wstring>>::create(callback7o);
                f7o(this_value, L"foo", 2, true, L"bar", 5, false, { L"baz" });
                f7o(this_value, jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), {});

                auto f6o = jsrt::function<void, jsrt::optional<std::wstring>, jsrt::optional<double>, jsrt::optional<bool>, jsrt::optional<std::wstring>, jsrt::optional<double>, std::vector<bool>>::create(callback6o);
                f6o(this_value, L"foo", 2, true, L"bar", 5, { false });
                f6o(this_value, jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), {});

                auto f5o = jsrt::function<void, jsrt::optional<std::wstring>, jsrt::optional<double>, jsrt::optional<bool>, jsrt::optional<std::wstring>, std::vector<double>>::create(callback5o);
                f5o(this_value, L"foo", 2, true, L"bar", { 5 });
                f5o(this_value, jsrt::missing(), jsrt::missing(), jsrt::missing(), jsrt::missing(), {});

                auto f4o = jsrt::function<void, jsrt::optional<std::wstring>, jsrt::optional<double>, jsrt::optional<bool>, std::vector<std::wstring>>::create(callback4o);
                f4o(this_value, L"foo", 2, true, { L"bar" });
                f4o(this_value, jsrt::missing(), jsrt::missing(), jsrt::missing(), {});

                auto f3o = jsrt::function<void, jsrt::optional<std::wstring>, jsrt::optional<double>, std::vector<bool>>::create(callback3o);
                f3o(this_value, L"foo", 2, { true });
                f3o(this_value, jsrt::missing(), jsrt::missing(), {});

                auto f2o = jsrt::function<void, jsrt::optional<std::wstring>, std::vector<double>>::create(callback2o);
                f2o(this_value, L"foo", { 2 });
                f2o(this_value, jsrt::missing(), {});

                auto f1o = jsrt::function<void, std::vector<std::wstring>>::create(callback1o);
                f1o(this_value, { L"foo" });
                f1o(this_value, {});
            }
            runtime.dispose();
        }

        static jsrt::object callback8c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7, double p8)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, static_cast<std::wstring>(L"baz"));
            Assert::AreEqual(p8, 8.0);

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        static jsrt::object callback7c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6, std::wstring p7)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, static_cast<std::wstring>(L"baz"));

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        static jsrt::object callback6c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5, bool p6)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
            Assert::AreEqual(p5, 5.0);
            Assert::AreEqual(p6, false);

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        static jsrt::object callback5c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4, double p5)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));
            Assert::AreEqual(p5, 5.0);

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        static jsrt::object callback4c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3, std::wstring p4)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, static_cast<std::wstring>(L"bar"));

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        static jsrt::object callback3c(const jsrt::call_info &info, std::wstring p1, double p2, bool p3)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
            Assert::AreEqual(p2, 2.0);
            Assert::AreEqual(p3, true);

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        static jsrt::object callback2c(const jsrt::call_info &info, std::wstring p1, double p2)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));
            Assert::AreEqual(p2, 2.0);

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        static jsrt::object callback1c(const jsrt::call_info &info, std::wstring p1)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            Assert::AreEqual(p1, static_cast<std::wstring>(L"foo"));

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        static jsrt::object callback0c(const jsrt::call_info &info)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(info.is_construct_call());

            return jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));
        }

        MY_TEST_METHOD(strongly_typed_constructors, "Test strongly typed constructor functions.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                auto f8 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(callback8c);
                jsrt::object result = f8.construct(L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f7 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(callback7c);
                result = f7.construct(L"foo", 2, true, L"bar", 5, false, L"baz");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f6 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool>::create(callback6c);
                result = f6.construct(L"foo", 2, true, L"bar", 5, false);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f5 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double>::create(callback5c);
                result = f5.construct(L"foo", 2, true, L"bar", 5);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f4 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring>::create(callback4c);
                result = f4.construct(L"foo", 2, true, L"bar");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f3 = jsrt::function<jsrt::object, std::wstring, double, bool>::create(callback3c);
                result = f3.construct(L"foo", 2, true);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f2 = jsrt::function<jsrt::object, std::wstring, double>::create(callback2c);
                result = f2.construct(L"foo", 2);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f1 = jsrt::function<jsrt::object, std::wstring>::create(callback1c);
                result = f1.construct(L"foo");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f0 = jsrt::function<jsrt::object>::create(callback0c);
                result = f0.construct();
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(strongly_typed_constructors_named, "Test strongly typed constructor functions.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                auto f8 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback8c);
                jsrt::object result = f8.construct(L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f7 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool, std::wstring>::create(L"foo", callback7c);
                result = f7.construct(L"foo", 2, true, L"bar", 5, false, L"baz");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f6 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double, bool>::create(L"foo", callback6c);
                result = f6.construct(L"foo", 2, true, L"bar", 5, false);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f5 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring, double>::create(L"foo", callback5c);
                result = f5.construct(L"foo", 2, true, L"bar", 5);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f4 = jsrt::function<jsrt::object, std::wstring, double, bool, std::wstring>::create(L"foo", callback4c);
                result = f4.construct(L"foo", 2, true, L"bar");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f3 = jsrt::function<jsrt::object, std::wstring, double, bool>::create(L"foo", callback3c);
                result = f3.construct(L"foo", 2, true);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f2 = jsrt::function<jsrt::object, std::wstring, double>::create(L"foo", callback2c);
                result = f2.construct(L"foo", 2);
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f1 = jsrt::function<jsrt::object, std::wstring>::create(L"foo", callback1c);
                result = f1.construct(L"foo");
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));

                auto f0 = jsrt::function<jsrt::object>::create(L"foo", callback0c);
                result = f0.construct();
                Assert::IsTrue(result.is_valid());
                Assert::IsTrue(result.is_external());
                Assert::AreEqual(static_cast<jsrt::external_object>(result).data(), reinterpret_cast<void *>(0xdeadbeef));
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
                jsrt::object proto = jsrt::object::create();
                auto f0 = jsrt::function<jsrt::object>::create(callback0cp);
                f0.set_constructor_prototype(proto);
                jsrt::object result = f0.construct();
                Assert::IsTrue(result.is_valid());
                Assert::IsFalse(result.is_external());
                Assert::AreEqual(result.prototype().handle(), f0.constructor_prototype().handle());
            }
            runtime.dispose();
        }

        static void callback_throws(const jsrt::call_info &info)
        {
            throw 5;
        }

        MY_TEST_METHOD(throws, "Test function that throws and doesn't guard.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object this_value = jsrt::external_object::create(reinterpret_cast<void *>(0xdeadbeef));

                auto t = jsrt::function<void>::create(callback_throws);
                bool threw = false;
                try
                {
                    t(this_value);
                }
                catch (const jsrt::script_exception &)
                {
                    threw = true;
                }
                Assert::IsTrue(threw);
            }
            runtime.dispose();
        }
    };
}