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
#include <vector>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(property_descriptor)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty property_descriptor handle.")
        {
            jsrt::property_descriptor<bool> handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            jsrt::property_descriptor<bool> property_descriptor;
            TEST_NO_CONTEXT_CALL(jsrt::property_descriptor<bool>::create());
            TEST_NO_CONTEXT_CALL(jsrt::property_descriptor<bool>::create(jsrt::function<bool>(), jsrt::function<void, bool>()));
            TEST_NO_CONTEXT_CALL(property_descriptor.writable());
            TEST_NO_CONTEXT_CALL(property_descriptor.set_writable(true));
            TEST_NO_CONTEXT_CALL(property_descriptor.enumerable());
            TEST_NO_CONTEXT_CALL(property_descriptor.set_enumerable(true));
            TEST_NO_CONTEXT_CALL(property_descriptor.configurable());
            TEST_NO_CONTEXT_CALL(property_descriptor.set_configurable(true));
            TEST_NO_CONTEXT_CALL(property_descriptor.value());
            TEST_NO_CONTEXT_CALL(property_descriptor.set_value(true));
            TEST_NO_CONTEXT_CALL(property_descriptor.getter());
            TEST_NO_CONTEXT_CALL(property_descriptor.set_getter(jsrt::function<bool>()));
            TEST_NO_CONTEXT_CALL(property_descriptor.setter());
            TEST_NO_CONTEXT_CALL(property_descriptor.set_setter(jsrt::function<void, bool>()));
            runtime.dispose();
        }

        static bool b(const jsrt::call_info &info)
        {
            return true;
        }

        static void set_b(const jsrt::call_info &info, bool v)
        {
            Assert::IsTrue(v);
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::property_descriptor<bool> property_descriptor;
                TEST_INVALID_ARG_CALL(property_descriptor.writable());
                TEST_INVALID_ARG_CALL(property_descriptor.set_writable(true));
                TEST_INVALID_ARG_CALL(property_descriptor.enumerable());
                TEST_INVALID_ARG_CALL(property_descriptor.set_enumerable(true));
                TEST_INVALID_ARG_CALL(property_descriptor.configurable());
                TEST_INVALID_ARG_CALL(property_descriptor.set_configurable(true));
                TEST_INVALID_ARG_CALL(property_descriptor.value());
                TEST_INVALID_ARG_CALL(property_descriptor.set_value(true));
                TEST_INVALID_ARG_CALL(property_descriptor.getter());
                TEST_INVALID_ARG_CALL(property_descriptor.set_getter(jsrt::function<bool>()));
                TEST_INVALID_ARG_CALL(property_descriptor.setter());
                TEST_INVALID_ARG_CALL(property_descriptor.set_setter(jsrt::function<void, bool>()));

                jsrt::property_descriptor<bool> valid_descriptor = jsrt::property_descriptor<bool>::create();
                TEST_INVALID_ARG_CALL(jsrt::property_descriptor<bool>::create(jsrt::function<bool>(), jsrt::function<void, bool>()));
                TEST_INVALID_ARG_CALL(jsrt::property_descriptor<bool>::create(jsrt::function<bool>::create(b), jsrt::function<void, bool>()));
                TEST_INVALID_ARG_CALL(property_descriptor.set_getter(jsrt::function<bool>()));
                TEST_INVALID_ARG_CALL(property_descriptor.set_setter(jsrt::function<void, bool>()));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(create, "Test ::create method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::property_descriptor<bool>::create();
                jsrt::property_descriptor<bool> property_descriptor = (jsrt::property_descriptor<bool>)value;
                Assert::IsTrue(property_descriptor.is_valid());
                property_descriptor = jsrt::property_descriptor<bool>::create(jsrt::function<bool>::create(b), jsrt::function<void, bool>::create(set_b));
                Assert::IsTrue(property_descriptor.is_valid());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(descriptors, "Test descriptor methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object object = jsrt::object::create();

                jsrt::property_descriptor<double> desc = jsrt::property_descriptor<double>::create();
                desc.set_configurable(false);
                desc.set_enumerable(false);
                desc.set_writable(false);
                desc.set_value(10);
                object.define_property(jsrt::property_id::create(L"a"), desc);
                Assert::AreEqual(object.get_property<double>(jsrt::property_id::create(L"a")), 10.0);
                desc = object.get_own_property_descriptor<double>(jsrt::property_id::create(L"a"));
                Assert::IsFalse(desc.configurable());
                Assert::IsFalse(desc.enumerable());
                Assert::IsFalse(desc.writable());
                Assert::AreEqual(desc.value(), 10.0);
                jsrt::property_descriptor<bool> bdesc = jsrt::property_descriptor<bool>::create(jsrt::function<bool>::create(b), jsrt::function<void, bool>::create(set_b));
                object.define_property(jsrt::property_id::create(L"b"), bdesc);
                Assert::IsTrue(object.get_property<bool>(jsrt::property_id::create(L"b")));
                object.set_property(jsrt::property_id::create(L"b"), true);
            }
            runtime.dispose();
        }
    };
}