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
    TEST_CLASS(context)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty context handle.")
        {
            jsrt::context handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(scopes, "Test scope object and ::current.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            {
                jsrt::context context = runtime.create_context();
                Assert::IsFalse(jsrt::context::current().is_valid());
                jsrt::context::scope scope(context);
                Assert::AreEqual(jsrt::context::current().handle(), context.handle());
            }
            Assert::IsFalse(jsrt::context::current().is_valid());
            runtime.dispose();
        }

        MY_TEST_METHOD(parent, "Test ::parent.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            Assert::AreEqual(runtime.handle(), context.parent().handle());
            runtime.dispose();
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            Profiler profiler;
            TEST_NO_CONTEXT_CALL(jsrt::context::start_profiling(&profiler));
            TEST_NO_CONTEXT_CALL(jsrt::context::stop_profiling(S_OK));
            TEST_NO_CONTEXT_CALL(jsrt::context::start_debugging());
            TEST_NO_CONTEXT_CALL(jsrt::context::idle());
            TEST_NO_CONTEXT_CALL(jsrt::context::has_exception());
            TEST_NO_CONTEXT_CALL(jsrt::context::get_and_clear_exception());
            TEST_NO_CONTEXT_CALL(jsrt::context::set_exception(jsrt::value()));
            TEST_NO_CONTEXT_CALL(jsrt::context::parse(L"1 + 2"));
            TEST_NO_CONTEXT_CALL(jsrt::context::run(L"1 + 2"));
            TEST_NO_CONTEXT_CALL(jsrt::context::evaluate(L"1 + 2"));
            TEST_NO_CONTEXT_CALL(jsrt::context::serialize(L"1 + 2", nullptr, 0));
            TEST_NO_CONTEXT_CALL(jsrt::context::run_serialized(L"1 + 2", nullptr));
            TEST_NO_CONTEXT_CALL(jsrt::context::parse_serialized(L"1 + 2", nullptr));
            TEST_NO_CONTEXT_CALL(jsrt::context::evaluate_serialized(L"1 + 2", nullptr));
            TEST_NO_CONTEXT_CALL(jsrt::context::set_promise_continuation_callback(nullptr));
            TEST_NO_CONTEXT_CALL(jsrt::context::project_uwp_namespace(std::wstring()));
            TEST_NO_CONTEXT_CALL(jsrt::context::set_uwp_completion_callback(nullptr));
            TEST_NO_CONTEXT_CALL(jsrt::context::undefined());
            TEST_NO_CONTEXT_CALL(jsrt::context::null());
            TEST_NO_CONTEXT_CALL(jsrt::context::global());
            runtime.dispose();
        }

        MY_TEST_METHOD(profiler, "Test ::start_profiling and ::stop_profiling.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                Profiler profiler;
                jsrt::context::start_profiling(&profiler);
                jsrt::context::stop_profiling(S_OK);
                TEST_NULL_ARG_CALL(jsrt::context::start_profiling(nullptr));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(debugging, "Test ::start_debugging.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::context::start_debugging();
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(heap_enum, "Test ::enumerate_heap and ::is_enumerating_heap.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                IActiveScriptProfilerHeapEnum *enumerator = jsrt::context::enumerate_heap();
                Assert::IsTrue(jsrt::context::is_enumerating_heap());
                TEST_FAILED_CALL(jsrt::object::create(), heap_enum_in_progress_exception);
                enumerator->Release();
                Assert::IsFalse(jsrt::context::is_enumerating_heap());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(idle, "Test ::idle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeEnableIdleProcessing);
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                Assert::AreEqual(jsrt::context::idle(), 0xffffffff);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(exceptions, "Test ::set_exception, ::has_exception, and ::get_and_clear_exception.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::error error = jsrt::error::create_syntax_error(L"");
                Assert::IsFalse(jsrt::context::has_exception());
                TEST_INVALID_ARG_CALL(jsrt::context::set_exception(jsrt::value()));
                jsrt::context::set_exception(error);
                Assert::IsTrue(jsrt::context::has_exception());
                jsrt::value value = jsrt::context::get_and_clear_exception();
                Assert::IsFalse(jsrt::context::has_exception());
                Assert::AreEqual(value.handle(), error.handle());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(scripts, "Test ::run, ::parse, and ::evaluate.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::function<double> func1 = (jsrt::function<double>)jsrt::context::parse(L"1 + 2");
                Assert::AreEqual(func1(jsrt::context::undefined()), 3.0);
                jsrt::context::run(L"function foo() { return 1 + 2; }");
                Assert::AreEqual(((jsrt::number)jsrt::context::evaluate(L"foo()")).as_int(), 3);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(serialized_scripts, "Test ::serialize, ::run_serialized, ::parse_serialized, and ::evaluate_serialized.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);

                const std::wstring script1 = L"1 + 2";
                unsigned int func1_size = jsrt::context::serialize(script1, nullptr, 0) + 16;
                byte *buffer1 = new byte[func1_size];
                jsrt::context::serialize(script1, buffer1, func1_size);
                TEST_NULL_ARG_CALL(jsrt::context::parse_serialized(script1, nullptr));
                jsrt::function<double> func1 = (jsrt::function<double>)jsrt::context::parse_serialized(script1, buffer1);
                Assert::AreEqual(func1(jsrt::context::undefined()), 3.0);

                const std::wstring script2 = L"function foo() { return 1 + 2; }";
                unsigned int func2_size = jsrt::context::serialize(script2, nullptr, 0) + 16;
                byte *buffer2 = new byte[func2_size];
                jsrt::context::serialize(script2, buffer2, func2_size);
                TEST_NULL_ARG_CALL(jsrt::context::run_serialized(script2, nullptr));
                jsrt::context::run_serialized(script2, buffer2);

                const std::wstring script3 = L"foo()";
                unsigned int func3_size = jsrt::context::serialize(script3, nullptr, 0) + 16;
                byte *buffer3 = new byte[func3_size];
                jsrt::context::serialize(script3, buffer3, func3_size);
                TEST_NULL_ARG_CALL(jsrt::context::evaluate_serialized(script3, nullptr));
                Assert::AreEqual(((jsrt::number)jsrt::context::evaluate_serialized(script3, buffer3)).as_int(), 3);

                delete[] buffer1;
                delete[] buffer2;
                delete[] buffer3;
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(values, "Test ::undefined, ::null, and ::global.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::context::undefined();
                Assert::AreEqual(value.type(), JsUndefined);
                value = jsrt::context::null();
                Assert::AreEqual(value.type(), JsNull);
                value = jsrt::context::global();
                Assert::AreEqual(value.type(), JsObject);
            }
            runtime.dispose();
        }

        static void promise_callback(std::shared_ptr<std::queue<jsrt::function<void>>> callbacks, jsrt::function<void> task)
        {
            callbacks->push(task);
        }

        MY_TEST_METHOD(promises, "Test promises.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                auto callbacks = std::make_shared<std::queue<jsrt::function<void>>>();
                auto cb = std::make_shared<std::function<void(jsrt::function<void>)>>(std::bind(promise_callback, callbacks, std::placeholders::_1));
                jsrt::context::set_promise_continuation_callback(cb);

                jsrt::context::run(L"new Promise(function(resolve, reject) {resolve();}).then(function() {}).then(function () {});");

                while (callbacks->size() > 0)
                {
                    callbacks->front()(jsrt::context::undefined());
                    callbacks->pop();
                }
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(uwp, "Test UWP projection.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::context::project_uwp_namespace(L"Windows");

                jsrt::context::run(L"var x = new Windows.Foundation.Uri('http://microsoft.com');");
            }
            runtime.dispose();
        }

        static void uwp_callback(HANDLE event, std::shared_ptr<std::queue<std::function<void()>>> callbacks, std::function<void()> callback)
        {
            callbacks->push(callback);
            SetEvent(event);
        }

        static DWORD WINAPI uwp_thread(LPVOID data)
        {
            Assert::IsTrue(CoInitializeEx(nullptr, COINIT_MULTITHREADED) == S_OK);
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);

                HANDLE event = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
                Assert::IsNotNull(event);

                auto callbacks = std::make_shared<std::queue<std::function<void()>>>();
                auto bound = std::make_shared<std::function<void(std::function<void()>)>>(std::bind(uwp_callback, event, callbacks, std::placeholders::_1));
                jsrt::context::set_uwp_completion_callback(bound);

                jsrt::context::project_uwp_namespace(L"Windows");

                jsrt::context::run(L"Windows.Storage.KnownFolders.documentsLibrary.tryGetItemAsync('filedoesnotexist.txt').done(function (file) {});");

                DWORD wait_result = WaitForSingleObjectEx(event, 10000, FALSE);
                Assert::AreEqual(wait_result, WAIT_OBJECT_0);

                while (callbacks->size() > 0)
                {
                    callbacks->front()();
                    callbacks->pop();
                }
            }
            runtime.dispose();
            CoUninitialize();
            return 0;
        }
        
        MY_TEST_METHOD(uwp_callbacks, "Test UWP projection callbacks.")
        {
            DWORD threadId;
            HANDLE thread = CreateThread(NULL, 0, &uwp_thread, NULL, 0, &threadId);
            WaitForSingleObject(thread, INFINITE);
            CloseHandle(thread);
        }
    };
}