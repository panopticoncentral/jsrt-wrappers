#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(runtime)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty handle.")
        {
            jsrt::runtime runtime;
            Assert::AreEqual(runtime.handle(), JS_INVALID_RUNTIME_HANDLE);
            Assert::IsFalse(runtime.is_valid());
        }

        MY_TEST_METHOD(create_dispose, "Test the ::create and ::dispose methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            Assert::AreNotEqual(runtime.handle(), JS_INVALID_RUNTIME_HANDLE);
            Assert::IsTrue(runtime.is_valid());
            runtime.dispose();
        }

#define TEST_INVALID_CALL(call) \
    try \
    { \
        call; \
        Assert::Fail(); \
    } \
    catch (const jsrt::invalid_argument_exception &) \
    { \
    }

        MY_TEST_METHOD(invalid_handle, "Test APIs on an invalid handle.")
        {
            jsrt::runtime runtime;
            TEST_INVALID_CALL(runtime.dispose())
            TEST_INVALID_CALL(runtime.memory_usage())
            TEST_INVALID_CALL(runtime.memory_limit())
            TEST_INVALID_CALL(runtime.set_memory_limit(-1))
            TEST_INVALID_CALL(runtime.set_memory_allocation_callback(nullptr, nullptr))
            TEST_INVALID_CALL(runtime.set_before_collect_callback(nullptr, nullptr))
            TEST_INVALID_CALL(runtime.collect_garbage())
            TEST_INVALID_CALL(runtime.disable_execution())
            TEST_INVALID_CALL(runtime.enable_execution())
            TEST_INVALID_CALL(runtime.is_execution_disabled())
            TEST_INVALID_CALL(runtime.create_context())
        }

        MY_TEST_METHOD(memory_usage, "Test ::memory_usage method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            size_t usage = runtime.memory_usage();
            Assert::AreNotEqual(usage, (size_t)0);
            runtime.dispose();
        }

        MY_TEST_METHOD(memory_limit, "Test ::memory_limit and ::set_memory_limit methods.")
        {
            const size_t onegb = 1024 * 1024 * 1024;

            jsrt::runtime runtime = jsrt::runtime::create();
            size_t limit = runtime.memory_limit();
            Assert::AreEqual(limit, (size_t) -1);
            runtime.set_memory_limit(onegb);
            limit = runtime.memory_limit();
            Assert::AreEqual(limit, onegb);
            runtime.set_memory_limit((size_t) -1);
            limit = runtime.memory_limit();
            Assert::AreEqual(limit, (size_t) -1);
            runtime.dispose();
        }

        static int callback_count;

        static bool CALLBACK allocation_callback(void *callbackState, JsMemoryEventType allocationEvent, size_t allocationSize)
        {
            Assert::AreEqual(callbackState, (void *) 0xdeadbeef);
            callback_count++;
            return true;
        }

        MY_TEST_METHOD(memory_allocation_callback, "Test ::set_memory_allocation_callback method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            callback_count = 0;
            runtime.set_memory_allocation_callback((void *) 0xdeadbeef, allocation_callback);
            {
                jsrt::context context = runtime.create_context();
                jsrt::context::scope scope(context);
            }
            runtime.set_memory_allocation_callback(nullptr, nullptr);
            Assert::AreNotEqual(runtime::callback_count, 0);
            runtime.dispose();
        }

        static void CALLBACK collect_callback(void *callbackState)
        {
            Assert::AreEqual(callbackState, (void *) 0xdeadbeef);
            callback_count++;
        }

        MY_TEST_METHOD(collection_and_callbacks, "Test ::set_before_collect_callback and ::collect_garbage.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            callback_count = 0;
            runtime.set_before_collect_callback((void *) 0xdeadbeef, collect_callback);
            {
                jsrt::context context = runtime.create_context();
                jsrt::context::scope scope(context);
                runtime.collect_garbage();
            }
            runtime.set_before_collect_callback(nullptr, nullptr);
            Assert::AreNotEqual(runtime::callback_count, 0);
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_disable, "Test ::enable_execution and ::disable_execution in a runtime that doesn't allow it.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            try
            {
                runtime.disable_execution();
                Assert::Fail();
            }
            catch (const jsrt::cannot_disable_execution_exception &)
            {
                // Expected
            }

            runtime.enable_execution();
            Assert::IsFalse(runtime.is_execution_disabled());
            runtime.dispose();
        }

        MY_TEST_METHOD(disable, "Test ::enable_execution and ::disable_execution methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeAllowScriptInterrupt);
            {
                jsrt::context context = runtime.create_context();
                jsrt::context::scope scope(context);
                runtime.disable_execution();
                Assert::IsTrue(runtime.is_execution_disabled());
                try
                {
                    context.has_exception();
                    Assert::Fail();
                }
                catch (const jsrt::in_disabled_state_exception &)
                {
                    // Expected
                }
                runtime.enable_execution();
                Assert::IsFalse(runtime.is_execution_disabled());
                Assert::IsFalse(context.has_exception());
            }
            runtime.dispose();
        }

        IDebugApplication *get_debug_application()
        {
            HRESULT hr = S_OK;
            IClassFactory *classFactory = nullptr;
            IProcessDebugManager *pdm = nullptr;
            IDebugApplication *debugApplication = nullptr;

            IfComFailError(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));
            IfComFailError(CoGetClassObject(__uuidof(ProcessDebugManager), CLSCTX_INPROC_SERVER, NULL, __uuidof(IClassFactory), (LPVOID *) &classFactory));
            IfComFailError(classFactory->CreateInstance(0, _uuidof(IProcessDebugManager), (LPVOID *) &pdm));
            Assert::IsTrue(SUCCEEDED(pdm->GetDefaultApplication(&debugApplication)));

        error:
            if (pdm)
            {
                pdm->Release();
            }

            if (classFactory)
            {
                classFactory->Release();
            }

            return debugApplication;
        }

        MY_TEST_METHOD(create_debug_context, "Test creating a context with debugging.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            {
                jsrt::context context = runtime.create_context(get_debug_application());
                jsrt::context::scope scope(context);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(disable_eval, "Test disabling eval in a runtime.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeDisableEval);
            {
                jsrt::context context = runtime.create_context();
                jsrt::context::scope scope(context);

                try
                {
                    context.run(L"eval(\"1 + 2\");");
                    Assert::Fail();
                }
                catch (const jsrt::script_eval_disabled_exception &)
                {
                    // Expected
                }
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(runtime_settings, "Test runtime settings.")
        {
            // JsRuntimeAttributeAllowScriptInterrupt is tested above in invalid_disable/disable
            // JsRuntimeAttributeEnableIdleProcessing is tested with contexts
            // JsRuntimeAttributeDisableEval is tested in disable_eval
            jsrt::runtime runtime = jsrt::runtime::create(
                (JsRuntimeAttributes) (
                JsRuntimeAttributeDisableBackgroundWork |
                JsRuntimeAttributeDisableNativeCodeGeneration));
            runtime.dispose();

            runtime = jsrt::runtime::create(JsRuntimeAttributeNone, JsRuntimeVersion10);
            runtime.dispose();

            runtime = jsrt::runtime::create(JsRuntimeAttributeNone, JsRuntimeVersion11);
            runtime.dispose();

            runtime = jsrt::runtime::create(JsRuntimeAttributeNone, JsRuntimeVersionEdge);
            runtime.dispose();
        }

        static bool CALLBACK service_callback(JsBackgroundWorkItemCallback callback, void *callbackState)
        {
            callback(callbackState);
            callback_count++;
            return true;
        }

        MY_TEST_METHOD(background_callback, "Test background work item callbacks.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeNone, JsRuntimeVersion11, service_callback);
            callback_count = 0;
            {
                jsrt::context context = runtime.create_context();
                jsrt::context::scope scope(context);
                runtime.collect_garbage();
            }
            Assert::AreNotEqual(runtime::callback_count, 0);
            runtime.dispose();
        }
    };

    int runtime::callback_count = 0;
}