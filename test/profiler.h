#pragma once

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
