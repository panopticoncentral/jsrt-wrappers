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

        MY_TEST_METHOD(temp, "Test messages.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
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
            jsrt::runtime runtime(param);
            TEST_FAILED_CALL(runtime.collect_garbage(), wrong_thread_exception);
            return 0;
        }

        MY_TEST_METHOD(wrong_thread, "Test wrong_thread_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                HANDLE thread = CreateThread(nullptr, 0, thread_proc, runtime.handle(), 0, nullptr);
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
    };
}