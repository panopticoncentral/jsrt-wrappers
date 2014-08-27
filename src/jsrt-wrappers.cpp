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
#include "jsrt-wrappers.h"
#include "jsrt-proxy_object.h"
namespace jsrt
{
    void runtime::dispose()
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }
        runtime::translate_error_code(JsDisposeRuntime(_handle));
        _handle = JS_INVALID_RUNTIME_HANDLE;
    }

    size_t runtime::memory_usage()
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        size_t memoryUsage;
        runtime::translate_error_code(JsGetRuntimeMemoryUsage(_handle, &memoryUsage));
        return memoryUsage;
    }

    size_t runtime::memory_limit()
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        size_t memoryLimit;
        runtime::translate_error_code(JsGetRuntimeMemoryLimit(_handle, &memoryLimit));
        return memoryLimit;
    }

    void runtime::set_memory_limit(size_t memory_limit)
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        runtime::translate_error_code(JsSetRuntimeMemoryLimit(_handle, memory_limit));
    }

    void runtime::collect_garbage()
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        runtime::translate_error_code(JsCollectGarbage(_handle));
    }

    void runtime::set_memory_allocation_callback(void *callbackState, JsMemoryAllocationCallback allocationCallback)
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        runtime::translate_error_code(JsSetRuntimeMemoryAllocationCallback(_handle, callbackState, allocationCallback));
    }

    void runtime::set_before_collect_callback(void *callbackState, JsBeforeCollectCallback beforeCollectCallback)
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        runtime::translate_error_code(JsSetRuntimeBeforeCollectCallback(_handle, callbackState, beforeCollectCallback));
    }

    void runtime::disable_execution()
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        runtime::translate_error_code(JsDisableRuntimeExecution(_handle));
    }

    void runtime::enable_execution()
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        runtime::translate_error_code(JsEnableRuntimeExecution(_handle));
    }

    bool runtime::is_execution_disabled()
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        bool value;
        runtime::translate_error_code(JsIsRuntimeExecutionDisabled(_handle, &value));
        return value;
    }

    void runtime::translate_error_code(JsErrorCode errorCode)
    {
        switch (errorCode)
        {
        case JsNoError:
            return;
        case JsErrorInvalidArgument:
            throw invalid_argument_exception();
        case JsErrorNullArgument:
            throw null_argument_exception();
        case JsErrorNoCurrentContext:
            throw no_current_context_exception();
        case JsErrorInExceptionState:
            throw in_exception_state_exception();
        case JsErrorNotImplemented:
            throw not_implemented_exception();
        case JsErrorWrongThread:
            throw wrong_thread_exception();
        case JsErrorRuntimeInUse:
            throw runtime_in_use_exception();
        case JsErrorBadSerializedScript:
            throw bad_serialized_script_exception();
        case JsErrorInDisabledState:
            throw in_disabled_state_exception();
        case JsErrorCannotDisableExecution:
            throw cannot_disable_execution_exception();
        case JsErrorHeapEnumInProgress:
            throw heap_enum_in_progress_exception();
        case JsErrorArgumentNotObject:
            throw argument_not_object_exception();
        case JsErrorInProfileCallback:
            throw in_profile_callback_exception();
        case JsErrorInThreadServiceCallback:
            throw in_thread_service_callback_exception();
        case JsErrorCannotSerializeDebugScript:
            throw cannot_serialize_debug_script_exception();
        case JsErrorAlreadyDebuggingContext:
            throw already_debugging_context_exception();
        case JsErrorAlreadyProfilingContext:
            throw already_profiling_context_exception();
        case JsErrorIdleNotEnabled:
            throw idle_not_enabled_exception();
        case JsErrorOutOfMemory:
            throw out_of_memory_exception();
        case JsErrorScriptException:
            {
                JsValueRef exception;

                if (JsGetAndClearException(&exception) != JsNoError)
                {
                    // Something has gone very wrong.
                    throw new fatal_exception();
                }

                throw script_exception(value(exception));
            }
        case JsErrorScriptCompile:
            {
                JsValueRef exception;

                if (JsGetAndClearException(&exception) != JsNoError)
                {
                    // Something has gone very wrong.
                    throw new fatal_exception();
                }

                throw script_compile_exception(compile_error(exception));
            }
        case JsErrorScriptTerminated:
            throw script_terminated_exception();
        case JsErrorScriptEvalDisabled:
            throw script_eval_disabled_exception();
        case JsErrorFatal:
            throw fatal_exception();
        default:
            throw fatal_exception();
        }
    }

    context runtime::create_context(IDebugApplication *debug_application)
    {
        // TODO: Throws an access violation in this case, which shouldn't happen
        if (!is_valid())
        {
            throw invalid_argument_exception();
        }

        JsContextRef newContext;

        runtime::translate_error_code(JsCreateContext(_handle, debug_application, &newContext));

        return context(newContext);
    }

    void context::start_debugging(IDebugApplication *debug_application)
    {
        // TODO: JsStartDebugging doesn't return an error in this case, which it should
        if (!debug_application)
        {
            throw null_argument_exception();
        }

        runtime::translate_error_code(JsStartDebugging(debug_application));
    }

    IActiveScriptProfilerHeapEnum *context::enumerate_heap()
    {
        IActiveScriptProfilerHeapEnum *enumerator;
        runtime::translate_error_code(JsEnumerateHeap(&enumerator));
        return enumerator;
    }

    bool context::is_enumerating_heap()
    {
        bool value;
        runtime::translate_error_code(JsIsEnumeratingHeap(&value));
        return value;
    }

    void context::set_exception(value object)
    {
        runtime::translate_error_code(JsSetException(object.handle()));
    }

    value context::get_and_clear_exception()
    {
        JsValueRef exception;
        runtime::translate_error_code(JsGetAndClearException(&exception));
        return value(exception);
    }

    function_base context::parse(std::wstring script, JsSourceContext sourceContext, std::wstring sourceUrl)
    {
        JsValueRef result = nullptr;

        runtime::translate_error_code(JsParseScript(script.c_str(), sourceContext, sourceUrl.c_str(), &result));

        return function_base(result);
    }

    value context::evaluate(std::wstring script, JsSourceContext sourceContext, std::wstring sourceUrl)
    {
        JsValueRef result = nullptr;

        runtime::translate_error_code(JsRunScript(script.c_str(), sourceContext, sourceUrl.c_str(), &result));

        return value(result);
    }

    function_base context::parse_serialized(std::wstring script, unsigned char *buffer, JsSourceContext sourceContext, std::wstring sourceUrl)
    {
        JsValueRef result = nullptr;

        runtime::translate_error_code(JsParseSerializedScript(script.c_str(), buffer, sourceContext, sourceUrl.c_str(), &result));

        return function_base(result);
    }

    value context::evaluate_serialized(std::wstring script, unsigned char *buffer, JsSourceContext sourceContext, std::wstring sourceUrl)
    {
        JsValueRef result = nullptr;

        runtime::translate_error_code(JsRunSerializedScript(script.c_str(), buffer, sourceContext, sourceUrl.c_str(), &result));

        return value(result);
    }

    value context::undefined()
    {
        JsValueRef undefinedValue;
        runtime::translate_error_code(JsGetUndefinedValue(&undefinedValue));
        return value(undefinedValue);
    }

    value context::null()
    {
        JsValueRef nullValue;
        runtime::translate_error_code(JsGetNullValue(&nullValue));
        return object(nullValue);
    }

    object context::global()
    {
        JsValueRef globalObject;
        runtime::translate_error_code(JsGetGlobalObject(&globalObject));
        return object(globalObject);
    }

    boolean boolean::convert(value value)
    {
        JsValueRef booleanValue;
        runtime::translate_error_code(JsConvertValueToBoolean(value.handle(), &booleanValue));
        return boolean(booleanValue);
    }

    number number::convert(value value)
    {
        JsValueRef numberValue;
        runtime::translate_error_code(JsConvertValueToNumber(value.handle(), &numberValue));
        return number(numberValue);
    }

    string string::convert(value value)
    {
        JsValueRef stringValue;
        runtime::translate_error_code(JsConvertValueToString(value.handle(), &stringValue));
        return string(stringValue);
    }

    std::vector<std::wstring> object::get_own_property_names()
    {
        std::vector<std::wstring> namesVector;

        JsValueRef names;
        runtime::translate_error_code(JsGetOwnPropertyNames(handle(), &names));

        auto namesArray = array<std::wstring>(names);

        for (int index = 0; index < namesArray.length(); index++)
        {
            namesVector.push_back(namesArray[index]);
        }

        return namesVector;
    }
}