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
            Assert::AreEqual(context.add_reference(), static_cast<unsigned int>(1));
            Assert::AreEqual(context.release(), static_cast<unsigned int>(0));
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test refcounting on an invalid handle.")
        {
            jsrt::context context;

            TEST_INVALID_ARG_CALL(context.add_reference());
            TEST_INVALID_ARG_CALL(context.release());
            TEST_INVALID_ARG_CALL(context.set_before_collect_callback(nullptr, nullptr))
        }

        static int callback_count;

        static void CALLBACK collect_callback(JsRef ref, void *callbackState)
        {
            Assert::AreEqual(callbackState, reinterpret_cast<void *>(0xdeadbeef));
            TEST_FAILED_CALL(jsrt::error::create(L"test"), in_object_before_collect_callback_exception);
            callback_count++;
        }

        MY_TEST_METHOD(collection_and_callbacks, "Test ::set_before_collect_callback.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            callback_count = 0;
            {
                jsrt::context context = runtime.create_context();
                jsrt::context::scope scope(context);
                jsrt::object object = jsrt::object::create();
                object.set_before_collect_callback(reinterpret_cast<void *>(0xdeadbeef), collect_callback);
                object = jsrt::object();
                runtime.collect_garbage();
            }
            Assert::AreNotEqual(callback_count, 0);
            runtime.dispose();
        }
    };

    int reference::callback_count = 0;
}