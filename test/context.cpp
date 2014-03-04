#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class Profiler sealed : public IActiveScriptProfilerCallback2
{
private:
    long m_refCount;

public:
    Profiler(void)
    {
        m_refCount = 1;
    }

    ~Profiler(void)
    {
    }

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObj)
    {
        if (riid == IID_IUnknown)
        {
            *ppvObj = (IUnknown *) this;
        }
        else if (riid == IID_IActiveScriptProfilerCallback)
        {
            *ppvObj = (IActiveScriptProfilerCallback *) this;
        }
        else if (riid == IID_IActiveScriptProfilerCallback2)
        {
            *ppvObj = (IActiveScriptProfilerCallback2 *) this;
        }
        else
        {
            *ppvObj = NULL;
            return E_NOINTERFACE;
        }


        AddRef();
        return NOERROR;
    }

    ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return InterlockedIncrement(&m_refCount);
    }

    ULONG STDMETHODCALLTYPE Release(void)
    {
        long lw;


        if (0 == (lw = InterlockedDecrement(&m_refCount)))
        {
            delete this;
        }
        return lw;
    }

    // IActiveScriptProfilerCallback
    HRESULT STDMETHODCALLTYPE Initialize(DWORD dwContext) { return S_OK; }
    HRESULT STDMETHODCALLTYPE Shutdown(HRESULT hrReason) { return S_OK; }
    HRESULT STDMETHODCALLTYPE ScriptCompiled(PROFILER_TOKEN scriptId, PROFILER_SCRIPT_TYPE type, IUnknown *pIDebugDocumentContext) { return S_OK; }
    HRESULT STDMETHODCALLTYPE FunctionCompiled(PROFILER_TOKEN functionId, PROFILER_TOKEN scriptId, const wchar_t *pwszFunctionName, const wchar_t *pwszFunctionNameHint, IUnknown *pIDebugDocumentContext) { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnFunctionEnter(PROFILER_TOKEN scriptId, PROFILER_TOKEN functionId) { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnFunctionExit(PROFILER_TOKEN scriptId, PROFILER_TOKEN functionId) { return S_OK; }

    // IActiveScriptProfilerCallback2
    HRESULT STDMETHODCALLTYPE OnFunctionEnterByName(const wchar_t *pwszFunctionName, PROFILER_SCRIPT_TYPE type) { return S_OK; }
    HRESULT STDMETHODCALLTYPE OnFunctionExitByName(const wchar_t *pwszFunctionName, PROFILER_SCRIPT_TYPE type) { return S_OK; }
};

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

        MY_TEST_METHOD(profiler, "Test ::start_profiling and ::stop_profiling.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                Profiler profiler;
                context.start_profiling(&profiler);
                context.stop_profiling(S_OK);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(debugging, "Test ::start_debugging.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                context.start_debugging(get_debug_application());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(idle, "Test ::idle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create(JsRuntimeAttributeEnableIdleProcessing);
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                Assert::AreEqual(context.idle(), 0xffffffff);
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
                Assert::IsFalse(context.has_exception());
                context.set_exception(error);
                Assert::IsTrue(context.has_exception());
                jsrt::value value = context.get_and_clear_exception();
                Assert::IsFalse(context.has_exception());
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
                Assert::AreEqual(func1.call(jsrt::value::undefined()), 3.0);
                jsrt::context::run(L"function foo() { return 1 + 2; }");
                Assert::AreEqual(((jsrt::number)jsrt::context::evaluate(L"foo()")).data(), 3.0);
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
                jsrt::function<double> func1 = (jsrt::function<double>)jsrt::context::parse_serialized(script1, buffer1);
                Assert::AreEqual(func1.call(jsrt::value::undefined()), 3.0);

                const std::wstring script2 = L"function foo() { return 1 + 2; }";
                unsigned int func2_size = jsrt::context::serialize(script2, nullptr, 0) + 16;
                byte *buffer2 = new byte[func2_size];
                jsrt::context::serialize(script2, buffer2, func2_size);
                jsrt::context::run_serialized(script2, buffer2);

                const std::wstring script3 = L"foo()";
                unsigned int func3_size = jsrt::context::serialize(script3, nullptr, 0) + 16;
                byte *buffer3 = new byte[func3_size];
                jsrt::context::serialize(script3, buffer3, func3_size);
                Assert::AreEqual(((jsrt::number)jsrt::context::evaluate_serialized(script3, buffer3)).data(), 3.0);

                delete[] buffer1;
                delete[] buffer2;
                delete[] buffer3;
            }
            runtime.dispose();
        }
    };
}