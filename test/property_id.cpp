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
    };
}