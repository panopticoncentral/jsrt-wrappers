#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(reference)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty handle.")
        {
            jsrt::reference handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }
        
        MY_TEST_METHOD(context_handle, "Test a context handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            Assert::IsTrue(context.is_valid());
            runtime.dispose();
        }

        MY_TEST_METHOD(handle_refcounting, "Test refcounting on a handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            Assert::AreEqual(context.add_reference(), (unsigned int) 1);
            Assert::AreEqual(context.release(), (unsigned int) 0);
            runtime.dispose();
        }
    };
}