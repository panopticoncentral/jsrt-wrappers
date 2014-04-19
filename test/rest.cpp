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
    TEST_CLASS(rest)
    {
    public:
        MY_TEST_METHOD(rest_values, "Test rest.")
        {
            jsrt::rest<int> o2;
            Assert::IsFalse(o2.has_value());
            Assert::IsFalse(o2.value().is_valid());
            o2.clear();
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::array<int> x = jsrt::array<int>::create(1);
                o2 = x;
                Assert::IsTrue(o2.has_value());
                Assert::AreEqual(o2.value().handle(), x.handle());
                o2.clear();
                Assert::IsFalse(o2.has_value());
                Assert::IsFalse(o2.value().is_valid());
                o2 = jsrt::missing();
                Assert::IsFalse(o2.has_value());
                Assert::IsFalse(o2.value().is_valid());
            }
            runtime.dispose();
        }
    };
}
