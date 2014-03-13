#include "stdafx.h"
#include <utility>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(optional)
    {
    public:
        MY_TEST_METHOD(optional_values, "Test optional.")
        {
            jsrt::optional<int> o1;
            Assert::IsFalse(o1.has_value());
            Assert::AreEqual(o1.value(), 0);
            o1.clear();
            o1 = 10;
            Assert::IsTrue(o1.has_value());
            Assert::AreEqual(o1.value(), 10);
            o1.clear();
            Assert::IsFalse(o1.has_value());
            Assert::AreEqual(o1.value(), 0);
        }
    };
}