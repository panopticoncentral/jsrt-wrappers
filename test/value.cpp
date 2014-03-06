#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(value)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty context handle.")
        {
            jsrt::value handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
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
                value = jsrt::object::null_value();
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
                value = jsrt::array<boolean>::create(10);
                Assert::AreEqual(value.type(), JsArray);
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
                Assert::AreEqual(((jsrt::number) value).data(), 42.0);
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
    };
}