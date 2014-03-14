#include "stdafx.h"
#include <utility>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(rest)
    {
    public:
        MY_TEST_METHOD(rest_values, "Test rest.")
        {
            jsrt::rest<int> o2;
            Assert::IsFalse(o2.has_value());
            Assert::IsFalse(o2.value().is_valid());
            o2.clear();
            jsrt::array<int> x = jsrt::array<int>::create(1);
            o2 = x;
            Assert::IsTrue(o2.has_value());
            Assert::AreEqual(o2.value().handle(), x.handle());
            o2.clear();
            Assert::IsFalse(o2.has_value());
            Assert::IsFalse(o2.value().is_valid());
        }
    };
}
