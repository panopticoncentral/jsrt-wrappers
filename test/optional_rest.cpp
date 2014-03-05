#include "stdafx.h"
#include <utility>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(optional_rest)
    {
    public:
        MY_TEST_METHOD(optional, "Test optional.")
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

            jsrt::rest<int> o2;
            Assert::IsFalse(o2.has_value());
            Assert::AreEqual(o2.value(), 0);
            o2.clear();
            o2 = 10;
            Assert::IsTrue(o2.has_value());
            Assert::AreEqual(o2.value(), 10);
            o2.clear();
            Assert::IsFalse(o2.has_value());
            Assert::AreEqual(o2.value(), 0);
        }
    };
}