// Copyright 2013 Paul Vick
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