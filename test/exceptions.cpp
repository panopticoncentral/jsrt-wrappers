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
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(exceptions)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty error handle.")
        {
            jsrt::compile_error handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::compile_error error;
                TEST_NULL_ARG_CALL(error.message());
                TEST_NULL_ARG_CALL(error.line());
                TEST_NULL_ARG_CALL(error.column());
                TEST_NULL_ARG_CALL(error.length());
                TEST_NULL_ARG_CALL(error.source());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_argument, "Test invalid_argument_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object valid_object = jsrt::object::create();
                TEST_INVALID_ARG_CALL(valid_object.has_property(jsrt::property_id()));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(out_of_memory, "Test out_of_memory_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                runtime.set_memory_limit(1024 * 1024);
                try
                {
                    jsrt::array<std::wstring> array = jsrt::array<std::wstring>::create(1024 * 1024);
                    for (int index = 0; index < 1024 * 1024; index++)
                    {
                        array[index] = L"foobar";
                    }
                    Assert::Fail();
                }
                catch (const jsrt::out_of_memory_exception &)
                {

                }
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(in_disabled_state, "Test in_disabled_state_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeAllowScriptInterrupt);
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                runtime.disable_execution();
                TEST_FAILED_CALL(jsrt::object::create(), in_disabled_state_exception);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(null_argument, "Test null_argument_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::compile_error error;
                TEST_NULL_ARG_CALL(error.message());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(no_current_context, "Test no_current_context_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            TEST_NO_CONTEXT_CALL(jsrt::object::create());
            runtime.dispose();
        }

        MY_TEST_METHOD(in_exception_state, "Test in_exception_state_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::context::set_exception(jsrt::error::create(L"foo"));
                TEST_FAILED_CALL(jsrt::object::create(), in_exception_state_exception);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(not_implemented, "Test not_implemented_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                // Nothing to test, no APIs return this error code.
            }
            runtime.dispose();
        }

        static DWORD WINAPI thread_proc(LPVOID param)
        {
            jsrt::runtime *runtime = (jsrt::runtime *)param;
            TEST_FAILED_CALL(runtime->collect_garbage(), wrong_thread_exception);
            return 0;
        }

        MY_TEST_METHOD(wrong_thread, "Test wrong_thread_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                HANDLE thread = CreateThread(nullptr, 0, thread_proc, &runtime, 0, nullptr);
                WaitForSingleObject(thread, INFINITE);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(runtime_in_use, "Test runtime_in_use_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                TEST_FAILED_CALL(runtime.dispose(), runtime_in_use_exception);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(cannot_disable_execution, "Test cannot_disable_execution_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                TEST_FAILED_CALL(runtime.disable_execution(), cannot_disable_execution_exception);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(heap_enum_in_progress, "Test heap_enum_in_progress_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                IActiveScriptProfilerHeapEnum *enumerator = jsrt::context::enumerate_heap();
                TEST_FAILED_CALL(jsrt::object::create(), heap_enum_in_progress_exception);
                enumerator->Release();
            }
            runtime.dispose();
        }

        static bool CALLBACK thread_service(JsBackgroundWorkItemCallback callback, void *callbackState)
        {
            jsrt::context current = jsrt::context::current();
            if (current.is_valid())
            {
                TEST_FAILED_CALL(current.parent().create_context(), in_thread_service_callback_exception);
            }
            return false;
        }

        MY_TEST_METHOD(in_thread_service_callback, "Test in_thread_service_callback_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeNone, JsRuntimeVersion11, thread_service);
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                runtime.collect_garbage();
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(cannot_serialize_debug_script, "Test cannot_serialize_debug_script_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::context::start_debugging(get_debug_application());
                TEST_FAILED_CALL(jsrt::context::serialize(L"1 + 2", nullptr, 0), cannot_serialize_debug_script_exception);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(already_debugging_context, "Test already_debugging_context_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::context::start_debugging(get_debug_application());
                TEST_FAILED_CALL(jsrt::context::start_debugging(get_debug_application()), already_debugging_context_exception);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(already_profiling_context, "Test already_profiling_context_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                Profiler profiler;
                jsrt::context::start_profiling(&profiler);
                TEST_FAILED_CALL(jsrt::context::start_profiling(&profiler), already_profiling_context_exception);
                jsrt::context::stop_profiling(S_OK);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(idle_not_enabled, "Test idle_not_enabled_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                TEST_FAILED_CALL(jsrt::context::idle(), idle_not_enabled_exception);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(bad_serialized_script, "Test bad_serialized_script_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                //unsigned char buffer[256];
                //memset(buffer, 0, 256);
                //TEST_FAILED_CALL(jsrt::context::run_serialized(L"1+2", buffer), bad_serialized_script_exception);
                // TODO: Not currently testable due to a bug in JsRT APIs.
            }
            runtime.dispose();
        }

        static HANDLE running_event;
        static HANDLE ready_event;

        static void signal(const jsrt::call_info &info)
        {
            SetEvent(running_event);
        }

        static DWORD WINAPI thread_proc2(LPVOID param)
        {
            jsrt::runtime *runtime = (jsrt::runtime *)param;
            SetEvent(ready_event);
            WaitForSingleObject(running_event, INFINITE);
            runtime->disable_execution();
            return 0;
        }

        MY_TEST_METHOD(script_terminated, "Test script_terminated_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeAllowScriptInterrupt);
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                running_event = CreateEvent(nullptr, true, false, nullptr);
                ready_event = CreateEvent(nullptr, true, false, nullptr);
                jsrt::context::global().set_property(jsrt::property_id::create(L"signal"),
                    jsrt::function<void>::create(signal));
                HANDLE thread = CreateThread(nullptr, 0, thread_proc2, &runtime, 0, nullptr);
                WaitForSingleObject(ready_event, INFINITE);
                TEST_FAILED_CALL(jsrt::context::run(L"while (true) { signal(); }"), script_terminated_exception);
                CloseHandle(ready_event);
                CloseHandle(running_event);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(script_eval_disabled, "Test script_eval_disabled_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeDisableEval);
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                TEST_FAILED_CALL(jsrt::context::run(L"eval(\"1 + 2\")"), script_eval_disabled_exception);
            }
            runtime.dispose();
        }

        // Can't test fatal_exception since there's no scenario where we can generate it & it's not
        // a bug...

        MY_TEST_METHOD(script, "Test script_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object object = jsrt::object::create();
                object.prevent_extension();
                try
                {
                    object.set_property(jsrt::property_id::create(L"bar"), 20);
                    Assert::Fail();
                }
                catch (const jsrt::script_exception &e)
                {
                    Assert::IsTrue(e.error().is_valid());
                }
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(script_compile, "Test script_compile_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                try
                {
                    jsrt::context::run(L"1+");
                    Assert::Fail();
                }
                catch (const jsrt::script_compile_exception &e)
                {
                    jsrt::compile_error error = e.error();
                    Assert::IsTrue(error.is_valid());
                    Assert::AreEqual(error.message(), (std::wstring)L"Syntax error");
                    Assert::AreEqual(error.line(), 0.0);
                    Assert::AreEqual(error.column(), 2.0);
                    Assert::AreEqual(error.length(), 0.0);
                    Assert::AreEqual(error.source(), (std::wstring)L"1+");
                }
            }
            runtime.dispose();
        }
    };

    HANDLE exceptions::running_event;
    HANDLE exceptions::ready_event;
}