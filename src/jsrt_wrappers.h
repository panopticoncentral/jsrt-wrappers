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

/// \mainpage Chakra Hosting API C++ Wrappers

/// \file
/// \brief Wrappers to make working with Chakra's hosting API simpler in C++.

#include <string>
#include <vector>

#pragma once

namespace jsrt
{
    class value;
    template<class T>
    class property_descriptor;
    class context;
    class function_base;
    class boolean;
    class number;
    class string;

    /// <summary>
    ///     A class that wraps a handle to a Chakra runtime.
    /// </summary>
    class runtime
    {
        friend class context;

    private:
        JsRuntimeHandle _handle;

        runtime(JsRuntimeHandle runtime) :
            _handle(runtime)
        {
        }

    public:
        /// <summary>
        ///     Constructs an invalid runtime handle.
        /// </summary>
        runtime() :
            _handle(JS_INVALID_RUNTIME_HANDLE)
        {
        }

        /// <summary>
        ///     The underlying runtime handle this class wraps.
        /// </summary>
        JsRuntimeHandle handle()
        {
            return _handle;
        }

        /// <summary>
        ///     Whether the handle is valid.
        /// </summary>
        bool is_valid()
        {
            return _handle != JS_INVALID_RUNTIME_HANDLE;
        }

        /// <summary>
        ///     Disposes the runtime.
        /// </summary>
        /// <remarks>
        ///     Once a runtime has been disposed, all resources owned by it are invalid and cannot be used.
        ///     If the runtime is active (i.e. it is set to be current on a particular thread), it cannot 
        ///     be disposed.
        /// </remarks>
        void dispose();

        /// <summary>
        ///     Gets the current memory usage for a runtime.
        /// </summary>
        /// <remarks>
        ///     Memory usage can be always be retrieved, regardless of whether or not the runtime is active
        ///     on another thread.
        /// </remarks>
        /// <returns>The runtime's current memory usage, in bytes.</returns>
        size_t memory_usage();

        /// <summary>
        ///     Gets the current memory limit for a runtime.
        /// </summary>
        /// <remarks>
        ///     The memory limit of a runtime can be always be retrieved, regardless of whether or not the 
        ///     runtime is active on another thread.
        /// </remarks>
        /// <returns>
        ///     The runtime's current memory limit, in bytes, or -1 if no limit has been set.
        /// </returns>
        size_t memory_limit();

        /// <summary>
        ///     Sets the current memory limit for a runtime.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     A memory limit will cause any operation which exceeds the limit to fail with an "out of 
        ///     memory" error. Setting a runtime's memory limit to -1 means that the runtime has no memory 
        ///     limit. New runtimes  default to having no memory limit. If the new memory limit exceeds
        ///     current usage, the call will succeed and any future allocations in this runtime will fail
        ///     until the runtime's memory usage drops below the limit.
        ///     </para>
        ///     <para>
        ///     A runtime's memory limit can be always be set, regardless of whether or not the runtime is 
        ///     active on another thread.
        ///     </para>
        /// </remarks>
        /// <param name="memory_limit">
        ///     The new runtime memory limit, in bytes, or -1 for no memory limit.
        /// </param>
        void set_memory_limit(size_t memory_limit);

        /// <summary>
        ///     Performs a full garbage collection.
        /// </summary>
        void collect_garbage();

        /// <summary>
        ///     Sets a memory allocation callback for specified runtime
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Registering a memory allocation callback will cause the runtime to call back to the host 
        ///     whenever it acquires memory from, or releases memory to, the OS. The callback routine is
        ///     called before the runtime memory manager allocates a block of memory. The allocation will
        ///     be rejected if the callback returns false. The runtime memory manager will also invoke the
        ///     callback routine after freeing a block of memory, as well as after allocation failures. 
        ///     </para>
        ///     <para>
        ///     The callback is invoked on the current runtime execution thread, therefore execution is 
        ///     blocked until the callback completes.
        ///     </para>
        ///     <para>
        ///     The return value of the callback is not stored; previously rejected allocations will not
        ///     prevent the runtime from invoking the callback again later for new memory allocations.
        ///     </para>
        /// </remarks>
        /// <param name="callbackState">
        ///     User provided state that will be passed back to the callback.
        /// </param>
        /// <param name="allocationCallback">
        ///     Memory allocation callback to be called for memory allocation events.
        /// </param>
        void set_memory_allocation_callback(void *callbackState, JsMemoryAllocationCallback allocationCallback);

        /// <summary>
        ///     Sets a callback function that is called by the runtime before garbage collection.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     The callback is invoked on the current runtime execution thread, therefore execution is 
        ///     blocked until the callback completes.
        ///     </para>
        ///     <para>
        ///     The callback can be used by hosts to prepare for garbage collection. For example, by 
        ///     releasing unnecessary references on Chakra objects.
        ///     </para>
        /// </remarks>
        /// <param name="callbackState">
        ///     User provided state that will be passed back to the callback.
        /// </param>
        /// <param name="beforeCollectCallback">The callback function being set.</param>
        void set_before_collect_callback(void *callbackState, JsBeforeCollectCallback beforeCollectCallback);

        /// <summary>
        ///     Suspends script execution and terminates any running scripts in a runtime.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Calls to a suspended runtime will fail until <c>enable_execution</c> is called.
        ///     </para>
        ///     <para>
        ///     This API does not have to be called on the thread the runtime is active on. Although the
        ///     runtime will be set into a suspended state, an executing script may not be suspended 
        ///     immediately; a running script will be terminated with an uncatchable exception as soon as 
        ///     possible.
        ///     </para>
        ///     <para>
        ///     Suspending execution in a runtime that is already suspended is a no-op.
        ///     </para>
        /// </remarks>
        void disable_execution();

        /// <summary>
        ///     Enables script execution in a runtime.
        /// </summary>
        /// <remarks>
        ///     Enabling script execution in a runtime that already has script execution enabled is a 
        ///     no-op. 
        /// </remarks>
        void enable_execution();

        /// <summary>
        ///     Returns a value that indicates whether script execution is disabled in the runtime.
        /// </summary>
        /// <returns>If execution is disabled, <c>true</c>, <c>false</c> otherwise.</returns>
        bool is_execution_disabled();

        /// <summary>
        ///     Creates a script context for running scripts.
        /// </summary>
        /// <remarks>
        ///     Each script context has its own global object that is isolated from all other script 
        ///     contexts.
        /// </remarks>
        /// <param name="debug_application">
        ///     The debug application to use for debugging. This parameter can be null, in which case 
        ///     debugging is not enabled for the context.
        /// </param>
        /// <returns>The created script context.</returns>
        context create_context(IDebugApplication *debug_application = nullptr);

        /// <summary>
        ///     Translates a Chakra error code into a wrapper exception.
        /// </summary>
        /// <remarks>
        ///     If the error code is not <c>JsNoError</c>, this will thrown the corresponding
        ///     exception.
        /// </remarks>
        static void translate_error_code(JsErrorCode errorCode);

        /// <summary>
        ///     Creates a new runtime.
        /// </summary>
        /// <param name="attributes">The attributes of the runtime to be created.</param>
        /// <param name="version">The version of the runtime to be created.</param>
        /// <param name="callback">The thread service for the runtime. Can be null.</param>
        /// <returns>The runtime created.</returns>
        static runtime create(JsRuntimeAttributes attributes = JsRuntimeAttributeNone, JsRuntimeVersion version = JsRuntimeVersion11, JsThreadServiceCallback callback = nullptr)
        {
            JsRuntimeHandle newRuntime;
            runtime::translate_error_code(JsCreateRuntime(attributes, version, callback, &newRuntime));
            return runtime(newRuntime);
        }
    };

    /// <summary>
    ///     A class that wraps a Chakra handle.
    /// </summary>
    class reference
    {
    protected:
        JsRef _ref;

        reference(JsRef ref) :
            _ref(ref)
        {
        }

    public:
        /// <summary>
        ///     Constructs an invalid handle.
        /// </summary>
        reference() :
            _ref(JS_INVALID_REFERENCE)
        {
        }

        /// <summary>
        ///     The handle being wrapped.
        /// </summary>
        JsRef handle() const
        {
            return _ref;
        }

        /// <summary>
        ///     Whether the handle is valid.
        /// </summary>
        bool is_valid()
        {
            return _ref != JS_INVALID_REFERENCE;
        }

        /// <summary>
        ///     Adds a reference to the handle.
        /// </summary>
        /// <remarks>
        ///     This only needs to be called on handles that are not going to be stored somewhere
        ///     on the stack. Calling <c>add_reference</c> ensures that the object the handle
        ///     refers to will not be freed until <c>release</c> is called.
        /// </remarks>
        /// <returns>The object's new reference count.</returns>
        unsigned int add_reference()
        {
            unsigned int count;
            runtime::translate_error_code(JsAddRef(_ref, &count));
            return count;
        }

        /// <summary>
        ///     Releases a reference to the handle.
        /// </summary>
        /// <remarks>
        ///     Removes a reference to a handle that was created by <c>add_reference</c>.
        /// </remarks>
        /// <returns>The object's new reference count (can pass in null).</returns>
        unsigned int release()
        {
            unsigned int count;
            runtime::translate_error_code(JsRelease(_ref, &count));
            return count;
        }
    };

    template<class T>
    class pinned_reference
    {
    private:
        T _reference;

    public:
        pinned_reference() :
            _reference()
        {
        }

        pinned_reference(T collectedReference) :
            _reference(collectedReference)
        {
            if (_reference.is_valid())
            {
                _reference.add_reference();
            }
        }

        pinned_reference(const pinned_reference<T>& right) :
            _reference(right._reference)
        {
            if (_reference.is_valid())
            {
                _reference.add_reference();
            }
        }

        ~pinned_reference()
        {
            if (_reference.is_valid())
            {
                _reference.release();
                _reference = T();
            }
        }

        T* operator->() const
        {
            return (T *) &_reference;
        }

        T operator*() const
        {
            return _reference;
        }

        pinned_reference<T>& operator=(const pinned_reference<T>& right)
        {
            _reference = right._reference;
            if (_reference.is_valid())
            {
                _reference.add_reference();
            }
            return *this;
        }
    };

    class context : public reference
    {
        friend class runtime;

    private:
        context(JsContextRef context) :
            reference(context)
        {
        }

    public:
        context() :
            reference()
        {
        }

        class scope
        {
        private:
            JsContextRef previousContext;

            // Disallow some operators to keep the scope on the stack, where it belongs.
            scope(const scope&);
            void operator=(const scope&) ;
            void* operator new(size_t size);
            void operator delete(void*, size_t);

        public:
            scope(context context)
            {
                runtime::translate_error_code(JsGetCurrentContext(&(this->previousContext)));
                runtime::translate_error_code(JsSetCurrentContext(context._ref));
            }

            ~scope()
            {
                runtime::translate_error_code(JsSetCurrentContext(this->previousContext));
            }
        };

        runtime parent()
        {
            JsRuntimeHandle runtimeHandle;
            runtime::translate_error_code(JsGetRuntime(_ref, &runtimeHandle));
            return runtime(runtimeHandle);
        }

        static void start_profiling(IActiveScriptProfilerCallback *callback, PROFILER_EVENT_MASK eventMask = PROFILER_EVENT_MASK_TRACE_ALL, unsigned long contextValue = 0)
        {
            runtime::translate_error_code(JsStartProfiling(callback, eventMask, contextValue));
        }

        static void stop_profiling(HRESULT reason)
        {
            runtime::translate_error_code(JsStopProfiling(reason));
        }

        static void start_debugging(IDebugApplication *debugApplication)
        {
            runtime::translate_error_code(JsStartDebugging(debugApplication));
        }

        static context current()
        {
            JsContextRef contextHandle;
            runtime::translate_error_code(JsGetCurrentContext(&contextHandle));

            return context(contextHandle);
        }

        static unsigned int idle()
        {
            unsigned int nextIdleTick;
            runtime::translate_error_code(JsIdle(&nextIdleTick));
            return nextIdleTick;
        }

        static bool has_exception()
        {
            bool hasException;
            runtime::translate_error_code(JsHasException(&hasException));
            return hasException;
        }

        static void set_exception(value object);

        static value get_and_clear_exception();

        static function_base parse(std::wstring script, JsSourceContext sourceContext = JS_SOURCE_CONTEXT_NONE, std::wstring sourceUrl = std::wstring());

        static void run(std::wstring script, JsSourceContext sourceContext = JS_SOURCE_CONTEXT_NONE, std::wstring sourceUrl = std::wstring())
        {
            runtime::translate_error_code(JsRunScript(script.c_str(), sourceContext, sourceUrl.c_str(), nullptr));
        }

        static value evaluate(std::wstring script, JsSourceContext sourceContext = JS_SOURCE_CONTEXT_NONE, std::wstring sourceUrl = std::wstring());

        static unsigned long serialize(std::wstring script, unsigned char *buffer, unsigned long bufferSize)
        {
            runtime::translate_error_code(JsSerializeScript(script.c_str(), buffer, &bufferSize));
            return bufferSize;
        }

        static function_base parse_serialized(std::wstring script, unsigned char *buffer, JsSourceContext sourceContext = JS_SOURCE_CONTEXT_NONE, std::wstring sourceUrl = std::wstring());

        static void run_serialized(std::wstring script, unsigned char *buffer, JsSourceContext sourceContext, std::wstring sourceUrl)
        {
            runtime::translate_error_code(JsRunSerializedScript(script.c_str(), buffer, sourceContext, sourceUrl.c_str(), nullptr));
        }

        static value evaluate_serialized(std::wstring script, unsigned char *buffer, JsSourceContext sourceContext = JS_SOURCE_CONTEXT_NONE, std::wstring sourceUrl = std::wstring());
    };

    class property_id
    {
    private:
        JsPropertyIdRef _propertyId;

        property_id(JsPropertyIdRef propertyId) :
            _propertyId(propertyId)
        {
        }

    public:
        property_id() :
            _propertyId(JS_INVALID_REFERENCE)
        {
        }

        JsPropertyIdRef handle()
        {
            return _propertyId;
        }

        const std::wstring name()
        {
            const wchar_t *result;
            runtime::translate_error_code(JsGetPropertyNameFromId(_propertyId, &result));
            return result;
        }

        bool is_valid() const
        {
            return _propertyId == JS_INVALID_REFERENCE;
        }

        static property_id create(const std::wstring propertyName)
        {
            JsPropertyIdRef propertyId;
            runtime::translate_error_code(JsGetPropertyIdFromName(propertyName.c_str(), &propertyId));
            return property_id(propertyId);
        }
    };

    template<class T>
    class optional
    {
    private:
        bool _hasValue;
        T _value;

    public:
        optional() :
            _hasValue(false),
            _value()
        {
        }

        optional(T value) :
            _hasValue(true),
            _value(value)
        {
        }

        bool has_value()
        {
            return _hasValue;
        }

        bool clear()
        {
            _hasValue = false;
            _value = T();
        }

        T value()
        {
            return _value;
        }
    };

    template<class T>
    class rest : public optional<T>
    {
    public:
        rest() :
            optional()
        {
        }

        rest(T value) :
            optional(value)
        {
        }
    };

    class value : public reference
    {
        friend class function_base;
        friend class runtime;
        friend class context;
        friend class object;

    private:
        JsValueType type() const
        {
            JsValueType type;
            runtime::translate_error_code(JsGetValueType(_ref, &type));
            return type;
        }

    protected:
        value(JsValueRef ref) :
            reference(ref)
        {
        }

        template<class T>
        static JsErrorCode to_native(JsValueRef value, T *result)
        {
            *result = T(value);
            return JsNoError;
        }

        template<class T>
        static JsErrorCode to_native(JsValueRef value, optional<T> *result)
        {
            *result = T();
            // We should never get here with an optional value.
            return JsErrorInvalidArgument;
        }

        template<>
        static JsErrorCode to_native<double>(JsValueRef value, double *result)
        {
            return JsNumberToDouble(value, result);
        }

        template<>
        static JsErrorCode to_native<bool>(JsValueRef value, bool *result)
        {
            return JsBooleanToBool(value, result);
        }

        template<>
        static JsErrorCode to_native<std::wstring>(JsValueRef value, std::wstring *result)
        {
            JsValueType type;
            JsErrorCode error = JsGetValueType(value, &type);
            if (error != JsNoError)
            {
                return error;
            }

            if (type == JsNull)
            {
                *result = std::wstring();
            }
            else
            {
                const wchar_t *resultptr = nullptr;
                size_t length;
                error = JsStringToPointer(value, &resultptr, &length);
                if (error == JsNoError)
                {
                    *result = std::wstring(resultptr, length);
                }
            }
            return error;
        }

        template<class T>
        static JsErrorCode from_native(T value, JsValueRef *result)
        {
            *result = value.handle();
            return JsNoError;
        }

        template<>
        static JsErrorCode from_native(double value, JsValueRef *result)
        {
            return JsDoubleToNumber(value, result);
        }

        template<>
        static JsErrorCode from_native(int value, JsValueRef *result)
        {
            return JsIntToNumber(value, result);
        }

        template<>
        static JsErrorCode from_native(bool value, JsValueRef *result)
        {
            return JsBoolToBoolean(value, result);
        }

        template<>
        static JsErrorCode from_native(std::wstring value, JsValueRef *result)
        {
            if (value.empty())
            {
                return JsGetNullValue(result);
            }
            return JsPointerToString(value.c_str(), value.length(), result);
        }

    public:
        value() :
            reference()
        {
        }

        static value undefined()
        {
            JsValueRef undefinedValue;
            runtime::translate_error_code(JsGetUndefinedValue(&undefinedValue));
            return value(undefinedValue);
        }

        bool is_undefined() const
        {
            return type() == JsUndefined;
        }

        bool is_null() const
        {
            return type() == JsNull;
        }

        bool is_boolean() const
        {
            return type() == JsBoolean;
        }

        bool is_number() const
        {
            return type() == JsNumber;
        }

        bool is_string() const
        {
            return type() == JsString;
        }

        bool is_object() const
        {
            return type() == JsObject;
        }

        bool is_function() const
        {
            return type() == JsFunction;
        }

        bool is_error() const
        {
            return type() == JsError;
        }

        bool is_array() const
        {
            return type() == JsArray;
        }

        boolean to_boolean();
        number to_number();
        string to_string();

        VARIANT to_variant()
        {
            VARIANT result;
            runtime::translate_error_code(JsValueToVariant(_ref, &result));
            return result;
        }

        bool equals(value other)
        {
            bool isEqual;
            runtime::translate_error_code(JsEquals(handle(), other.handle(), &isEqual));
            return isEqual;
        }

        bool strict_equals(value other)
        {
            bool isEqual;
            runtime::translate_error_code(JsStrictEquals(handle(), other.handle(), &isEqual));
            return isEqual;
        }

        static value create(VARIANT *variantValue)
        {
            JsValueRef result;
            runtime::translate_error_code(JsVariantToValue(variantValue, &result));
            return value(result);
        }
    };

    class boolean : public value
    {
        friend class value;

    private:
        boolean(JsValueRef ref) :
            value(ref)
        {
        }

    public:
        boolean() :
            value()
        {
        }

        explicit boolean(value object) :
            value(object.handle())
        {
        }

        bool data()
        {
            bool result;
            runtime::translate_error_code(to_native(_ref, &result));
            return result;
        }

        static boolean true_value()
        {
            JsValueRef trueValue;
            runtime::translate_error_code(JsGetTrueValue(&trueValue));
            return boolean(trueValue);
        }

        static boolean false_value()
        {
            JsValueRef falseValue;
            runtime::translate_error_code(JsGetFalseValue(&falseValue));
            return boolean(falseValue);
        }

        static boolean create(bool value)
        {
            JsValueRef result;
            runtime::translate_error_code(from_native(value, &result));
            return boolean(result);
        }
    };

    class number : public value
    {
        friend class value;

    private:
        number(JsValueRef ref) :
            value(ref)
        {
        }

    public:
        number() :
            value()
        {
        }

        explicit number(value object) :
            value(object.handle())
        {
        }

        double data()
        {
            double result;
            runtime::translate_error_code(to_native(_ref, &result));
            return result;
        }

        static number create(double value)
        {
            JsValueRef result;
            runtime::translate_error_code(from_native(value, &result));
            return number(result);
        }

        static number create(int value)
        {
            JsValueRef result;
            runtime::translate_error_code(from_native(value, &result));
            return number(result);
        }
    };

    class string : public value
    {
        friend class value;

    private:
        string(JsValueRef ref) :
            value(ref)
        {
        }

    public:
        string() :
            value()
        {
        }

        explicit string(value object) :
            value(object.handle())
        {
        }

        int length()
        {
            int length;
            runtime::translate_error_code(JsGetStringLength(_ref, &length));
            return length;
        }

        std::wstring data()
        {
            std::wstring result;
            runtime::translate_error_code(to_native(_ref, &result));
            return result;
        }

        static string create(std::wstring value)
        {
            JsValueRef result;
            runtime::translate_error_code(from_native(value, &result));
            return string(result);
        }
    };

    class object : public value
    {
        friend class function_base;
        friend class value;

    protected:
        object(JsValueRef ref) :
            value(ref)
        {
        }

    public:
        object() :
            value()
        {
        }

        explicit object(value object) :
            value(object.handle())
        {
        }

        bool is_external() const
        {
            bool hasExternalData;
            runtime::translate_error_code(JsHasExternalData(handle(), &hasExternalData));

            return hasExternalData;
        }

        bool is_extension_allowed() const
        {
            bool isExtensionaAllowed;
            runtime::translate_error_code(JsGetExtensionAllowed(handle(), &isExtensionaAllowed));
            return isExtensionaAllowed;
        }

        typedef object prototype_proxy;

        object prototype()
        {
            JsValueRef prototype;
            runtime::translate_error_code(JsGetPrototype(handle(), &prototype));
            return object(prototype);
        }

        void set_prototype(object prototype)
        {
            runtime::translate_error_code(JsSetPrototype(handle(), prototype.handle()));
        }

        void prevent_extension()
        {
            runtime::translate_error_code(JsPreventExtension(handle()));
        }

        template<class T = value>
        T get_property(property_id name)
        {
            JsValueRef value;
            runtime::translate_error_code(JsGetProperty(handle(), name.handle(), &value));

            T returnValue;
            runtime::translate_error_code(to_native(value, &returnValue));

            return returnValue;
        }

        template<class T = value>
        property_descriptor<T> get_own_property_descriptor(property_id name);

        std::vector<std::wstring> get_own_property_names();

        template<class T = value>
        void set_property(property_id name, T value, bool useStrictRules = true)
        {
            JsValueRef valueReference;
            runtime::translate_error_code(from_native(value, &valueReference));

            runtime::translate_error_code(JsSetProperty(handle(), name.handle(), valueReference, useStrictRules));
        }

        bool has_property(property_id name)
        {
            bool hasProperty;
            runtime::translate_error_code(JsHasProperty(handle(), name.handle(), &hasProperty));
            return hasProperty;
        }

        template<class T = value>
        T delete_property(property_id name, bool useStrictRules = true)
        {
            JsValueRef value;
            runtime::translate_error_code(JsDeleteProperty(handle(), name.handle(), useStrictRules, &value));

            T returnValue;
            runtime::translate_error_code(to_native(value, &returnValue));

            return returnValue;
        }

        template<class T>
        bool define_property(property_id name, property_descriptor<T> descriptor);

        template<class T = value>
        T get_index(property_id index)
        {
            JsValueRef value;
            runtime::translate_error_code(JsGetIndexedProperty(handle(), index.handle(), &value));

            T returnValue;
            runtime::translate_error_code(to_native(value, &returnValue));

            return returnValue;
        }

        template<class T = value>
        T get_index(int index)
        {
            JsValueRef indexValue;
            runtime::translate_error_code(JsIntToNumber(index, &indexValue));

            JsValueRef value;
            runtime::translate_error_code(JsGetProperty(handle(), indexValue, &value));

            T returnValue;
            runtime::translate_error_code(to_native(value, &returnValue));

            return returnValue;
        }

        template<class T = value>
        void set_index(property_id index, T value)
        {
            JsValueRef valueReference;
            runtime::translate_error_code(from_native(value, &valueReference));

            runtime::translate_error_code(JsSetIndexedProperty(handle(), index.handle(), valueReference, useStrictRules));
        }

        template<class T = value>
        void set_index(int index, T value)
        {
            JsValueRef indexValue;
            runtime::translate_error_code(JsIntToNumber(index, &indexValue));

            JsValueRef valueReference;
            runtime::translate_error_code(from_native(value, &valueReference));

            runtime::translate_error_code(JsSetIndexedProperty(handle(), indexValue, valueReference, useStrictRules));
        }

        bool has_index(property_id index)
        {
            bool hasProperty;
            runtime::translate_error_code(JsHasProperty(handle(), index.handle(), &hasProperty));
            return hasProperty;
        }

        bool has_index(int index)
        {
            JsValueRef indexValue;
            runtime::translate_error_code(JsIntToNumber(index, &indexValue));

            bool hasProperty;
            runtime::translate_error_code(JsHasProperty(handle(), indexValue, &hasProperty));
            return hasProperty;
        }

        void delete_index(property_id index)
        {
            runtime::translate_error_code(JsDeleteIndexedProperty(handle(), index.handle()));
        }

        void delete_index(int index)
        {
            JsValueRef indexValue;
            runtime::translate_error_code(JsIntToNumber(index, &indexValue));
            runtime::translate_error_code(JsDeleteIndexedProperty(handle(), indexValue));
        }

        static object null_value()
        {
            JsValueRef nullValue;
            runtime::translate_error_code(JsGetNullValue(&nullValue));
            return object(nullValue);
        }

        static object global_object()
        {
            JsValueRef globalObject;
            runtime::translate_error_code(JsGetGlobalObject(&globalObject));
            return object(globalObject);
        }

        static object create()
        {
            JsValueRef objectValue;
            runtime::translate_error_code(JsCreateObject(&objectValue));
            return object(objectValue);
        }
    };

    class external_object : public object
    {
    private:
        external_object(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        external_object() :
            object()
        {
        }

        explicit external_object(value object) :
            object(object.handle())
        {
        }

        void *data()
        {
            void *data;
            runtime::translate_error_code(JsGetExternalData(handle(), &data));
            return data;
        }

        void set_data(void *data)
        {
            runtime::translate_error_code(JsSetExternalData(handle(), data));
        }

        typedef void (CALLBACK *Finalize)(void *data);

        static external_object create(void *data = nullptr, Finalize finalizeCallback = nullptr)
        {
            JsValueRef object;
            runtime::translate_error_code(JsCreateExternalObject(data, finalizeCallback, &object));
            return external_object(object);
        }
    };

    template<class T>
    class array : public object
    {
        friend class value;
        friend class object;

    private:
        array(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        template<class T>
        class array_element
        {
        private:
            array<T> _array;
            value _index;

        public:
            array_element(array<T> array, value index) :
                _array(array), _index(index)
            {
            }

            array<T> array()
            {
                return _array;
            }

            value index()
            {
                return _index;
            }

            array_element operator=(T value)
            {
                JsValueRef valueReference;
                runtime::translate_error_code(value::from_native<T>(value, &valueReference));
                runtime::translate_error_code(JsSetIndexedProperty(_array.handle(), _index.handle(), valueReference));
                return *this;
            }

            operator T()
            {
                JsValueRef valueReference;
                runtime::translate_error_code(JsGetIndexedProperty(_array.handle(), _index.handle(), &valueReference));

                T value;
                runtime::translate_error_code(to_native(valueReference, &value));

                return value;
            }
        };

        array() :
            object(JS_INVALID_REFERENCE)
        {
        }

        explicit array(value object) :
            object(object.handle())
        {
        }

        array_element<T> operator [](int index)
        {
            return array_element<T>(*this, number::create(index));
        }

        int length()
        {
            JsPropertyIdRef lengthName;
            JsValueRef lengthValue;
            double lengthDouble;

            runtime::translate_error_code(JsGetPropertyIdFromName(L"length", &lengthName));
            runtime::translate_error_code(JsGetProperty(handle(), lengthName, &lengthValue));
            runtime::translate_error_code(JsNumberToDouble(lengthValue, &lengthDouble));
            return (int) lengthDouble;
        }

        static array<T> create(unsigned int length)
        {
            JsValueRef array;
            runtime::translate_error_code(JsCreateArray(length, &array));
            return jsrt::array<T>(array);
        }
    };

    class error : public object
    {
    protected:
        error(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        error() :
            object()
        {
        }

        explicit error(value object) :
            object(object.handle())
        {
        }

        std::wstring name()
        {
            optional<value> name = get_property<value>(property_id::create(L"name"));

            if (name.has_value() && name.value().is_string())
            {
                return ((string) name.value()).data();
            }

            return L"";
        }

        std::wstring message()
        {
            optional<value> message = get_property<value>(property_id::create(L"message"));

            if (message.has_value() && message.value().is_string())
            {
                return ((string) message.value()).data();
            }

            return L"";
        }

        static error create(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);
            wchar_t buffer[2048];

            _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, message.c_str(), argptr);

            JsValueRef errorString;
            JsValueRef errorObject;

            // We're ignoring errors because there's nothing we can do here if things fail.
            JsPointerToString(buffer, wcslen(buffer), &errorString);
            JsCreateError(errorString, &errorObject);
            return error(errorObject);
        }

        static error create_type_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);
            wchar_t buffer[2048];

            _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, message.c_str(), argptr);

            JsValueRef errorString;
            JsValueRef errorObject;

            // We're ignoring errors because there's nothing we can do here if things fail.
            JsPointerToString(buffer, wcslen(buffer), &errorString);
            JsCreateTypeError(errorString, &errorObject);
            return error(errorObject);
        }

        static error create_reference_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);
            wchar_t buffer[2048];

            _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, message.c_str(), argptr);

            JsValueRef errorString;
            JsValueRef errorObject;

            // We're ignoring errors because there's nothing we can do here if things fail.
            JsPointerToString(buffer, wcslen(buffer), &errorString);
            JsCreateReferenceError(errorString, &errorObject);
            return error(errorObject);
        }

        static error create_range_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);
            wchar_t buffer[2048];

            _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, message.c_str(), argptr);

            JsValueRef errorString;
            JsValueRef errorObject;

            // We're ignoring errors because there's nothing we can do here if things fail.
            JsPointerToString(buffer, wcslen(buffer), &errorString);
            JsCreateRangeError(errorString, &errorObject);
            return error(errorObject);
        }

        static error create_syntax_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);
            wchar_t buffer[2048];

            _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, message.c_str(), argptr);

            JsValueRef errorString;
            JsValueRef errorObject;

            // We're ignoring errors because there's nothing we can do here if things fail.
            JsPointerToString(buffer, wcslen(buffer), &errorString);
            JsCreateSyntaxError(errorString, &errorObject);
            return error(errorObject);
        }

        static error create_uri_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);
            wchar_t buffer[2048];

            _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, message.c_str(), argptr);

            JsValueRef errorString;
            JsValueRef errorObject;

            // We're ignoring errors because there's nothing we can do here if things fail.
            JsPointerToString(buffer, wcslen(buffer), &errorString);
            JsCreateURIError(errorString, &errorObject);
            return error(errorObject);
        }
    };

    struct notdefined {};

    class function_base : public object
    {
    protected:
        template<class T>
        static bool is_rest(T value)
        {
            return false;
        };

        template<class T>
        static bool is_rest(rest<T> value)
        {
            return true;
        };

        template<class T>
        static bool argument_from_value(int position, JsValueRef *arguments, int argumentCount, T &result)
        {
            if (position > argumentCount - 1)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }
            else if (to_native(arguments[position], &result) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return false;
            }

            return true;
        }

        template<class T>
        static bool argument_from_value(int position, JsValueRef *arguments, int argumentCount, optional<T> &result)
        {
            if (position > argumentCount - 1)
            {
                result = T();
            }
            else
            {
                T nativeValue;
                if (to_native(arguments[position], &nativeValue) != JsNoError)
                {
                    // TODO: Include typename.
                    context::set_exception(error::create_type_error(L"Could not convert value."));
                    result = optional<T>();
                }
                else
                {
                    result = optional<T>(nativeValue);
                }
            }

            return true;
        }

        template<class T>
        static bool argument_from_value(int position, JsValueRef *arguments, int argumentCount, rest < array < T >> &result)
        {
            if (position < argumentCount)
            {
                array<T> rest = array<T>::create(argumentCount - position);

                for (int index = 0; index < argumentCount - position; index++)
                {
                    T value;

                    if (to_native(arguments[position + index], &value) != JsNoError)
                    {
                        // TODO: Include typename.
                        context::set_exception(error::create_type_error(L"Could not convert value."));
                        return false;
                    }

                    rest[index] = value;
                }

                result = rest;
            }

            return true;
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6, P7 &p7, P8 &p8)
        {
            calleeObject = object(callee);

            if (!is_rest(p8) && argumentCount > 9)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return argument_from_value(1, arguments, argumentCount, p1) &&
                argument_from_value(2, arguments, argumentCount, p2) &&
                argument_from_value(3, arguments, argumentCount, p3) &&
                argument_from_value(4, arguments, argumentCount, p4) &&
                argument_from_value(5, arguments, argumentCount, p5) &&
                argument_from_value(6, arguments, argumentCount, p6) &&
                argument_from_value(7, arguments, argumentCount, p7) &&
                argument_from_value(8, arguments, argumentCount, p8);
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6, P7 &p7)
        {
            calleeObject = object(callee);

            if (!is_rest(p7) && argumentCount > 8)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return argument_from_value(1, arguments, argumentCount, p1) &&
                argument_from_value(2, arguments, argumentCount, p2) &&
                argument_from_value(3, arguments, argumentCount, p3) &&
                argument_from_value(4, arguments, argumentCount, p4) &&
                argument_from_value(5, arguments, argumentCount, p5) &&
                argument_from_value(6, arguments, argumentCount, p6) &&
                argument_from_value(7, arguments, argumentCount, p7);
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6>
        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6)
        {
            calleeObject = object(callee);

            if (!is_rest(p6) && argumentCount > 7)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return argument_from_value(1, arguments, argumentCount, p1) &&
                argument_from_value(2, arguments, argumentCount, p2) &&
                argument_from_value(3, arguments, argumentCount, p3) &&
                argument_from_value(4, arguments, argumentCount, p4) &&
                argument_from_value(5, arguments, argumentCount, p5) &&
                argument_from_value(6, arguments, argumentCount, p6);
        }

        template <class P1, class P2, class P3, class P4, class P5>
        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5)
        {
            calleeObject = object(callee);

            if (!is_rest(p5) && argumentCount > 6)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return argument_from_value(1, arguments, argumentCount, p1) &&
                argument_from_value(2, arguments, argumentCount, p2) &&
                argument_from_value(3, arguments, argumentCount, p3) &&
                argument_from_value(4, arguments, argumentCount, p4) &&
                argument_from_value(5, arguments, argumentCount, p5);
        }

        template <class P1, class P2, class P3, class P4>
        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue, P1 &p1, P2 &p2, P3 &p3, P4 &p4)
        {
            calleeObject = object(callee);

            if (!is_rest(p4) && argumentCount > 5)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return argument_from_value(1, arguments, argumentCount, p1) &&
                argument_from_value(2, arguments, argumentCount, p2) &&
                argument_from_value(3, arguments, argumentCount, p3) &&
                argument_from_value(4, arguments, argumentCount, p4);
        }

        template <class P1, class P2, class P3>
        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue, P1 &p1, P2 &p2, P3 &p3)
        {
            calleeObject = object(callee);

            if (!is_rest(p3) && argumentCount > 4)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return argument_from_value(1, arguments, argumentCount, p1) &&
                argument_from_value(2, arguments, argumentCount, p2) &&
                argument_from_value(3, arguments, argumentCount, p3);
        }

        template <class P1, class P2>
        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue, P1 &p1, P2 &p2)
        {
            calleeObject = object(callee);

            if (!is_rest(p2) && argumentCount > 3)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return argument_from_value(1, arguments, argumentCount, p1) &&
                argument_from_value(2, arguments, argumentCount, p2);
        }

        template <class P1>
        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue, P1 &p1)
        {
            calleeObject = object(callee);

            if (!is_rest(p1) && argumentCount > 2)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return argument_from_value(1, arguments, argumentCount, p1);
        }

        static bool unpack_arguments(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, object &calleeObject, value &thisValue)
        {
            calleeObject = object(callee);

            if (argumentCount > 1)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            thisValue = value(arguments[0]);

            return true;
        }

        template<class T>
        static unsigned rest_argument_count(T value)
        {
            return 1;
        };

        template<class T>
        static unsigned rest_argument_count(rest < array < T >> value)
        {
            return value.has_value() ? (int) value.value.length : 0;
        };

        template<class T>
        static void fill_rest(T argument, unsigned start, std::vector<JsValueRef> &arguments)
        {
            runtime::translate_error_code(from_native(argument, &arguments[start]));
        }

        template<class T>
        static void fill_rest(rest < array < T >> rest, unsigned start, std::vector<JsValueRef> &arguments)
        {
            if (rest.has_value())
            {
                for (unsigned index = 0; index < rest.value.length; index++)
                {
                    // TODO: Why this cast? Can we get rid of it?
                    runtime::translate_error_code(from_native((T) rest.value[index], &arguments[start + index]));
                }
            }
        }

        template<class P1, class P2, class P3, class P4>
        static unsigned argument_count(P1 p1, P2 p2, P3 p3, P4 p4)
        {
            return 4 + rest_argument_count(p4);
        }

        template<class P1, class P2, class P3>
        static unsigned argument_count(P1 p1, P2 p2, P3 p3)
        {
            return 3 + rest_argument_count(p3);
        }

        template<class P1>
        static unsigned argument_count(P1 p1)
        {
            return 1 + rest_argument_count(p1);
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
        static std::vector<JsValueRef> pack_arguments(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            unsigned argumentCount = 8 + rest_argument_count(p8);
            std::vector<JsValueRef> arguments(argumentCount);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            runtime::translate_error_code(from_native(p1, &arguments[1]));
            runtime::translate_error_code(from_native(p2, &arguments[2]));
            runtime::translate_error_code(from_native(p3, &arguments[3]));
            runtime::translate_error_code(from_native(p4, &arguments[4]));
            runtime::translate_error_code(from_native(p5, &arguments[5]));
            runtime::translate_error_code(from_native(p6, &arguments[6]));
            runtime::translate_error_code(from_native(p7, &arguments[7]));
            fill_rest(p8, 8, arguments);

            return arguments;
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
        static std::vector<JsValueRef> pack_arguments(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            unsigned argumentCount = 7 + rest_argument_count(p7);
            std::vector<JsValueRef> arguments(argumentCount);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            runtime::translate_error_code(from_native(p1, &arguments[1]));
            runtime::translate_error_code(from_native(p2, &arguments[2]));
            runtime::translate_error_code(from_native(p3, &arguments[3]));
            runtime::translate_error_code(from_native(p4, &arguments[4]));
            runtime::translate_error_code(from_native(p5, &arguments[5]));
            runtime::translate_error_code(from_native(p6, &arguments[6]));
            fill_rest(p7, 7, arguments);

            return arguments;
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6>
        static std::vector<JsValueRef> pack_arguments(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            unsigned argumentCount = 6 + rest_argument_count(p6);
            std::vector<JsValueRef> arguments(argumentCount);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            runtime::translate_error_code(from_native(p1, &arguments[1]));
            runtime::translate_error_code(from_native(p2, &arguments[2]));
            runtime::translate_error_code(from_native(p3, &arguments[3]));
            runtime::translate_error_code(from_native(p4, &arguments[4]));
            runtime::translate_error_code(from_native(p5, &arguments[5]));
            fill_rest(p6, 6, arguments);

            return arguments;
        }

        template <class P1, class P2, class P3, class P4, class P5>
        static std::vector<JsValueRef> pack_arguments(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            unsigned argumentCount = 5 + rest_argument_count(p5);
            std::vector<JsValueRef> arguments(argumentCount);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            runtime::translate_error_code(from_native(p1, &arguments[1]));
            runtime::translate_error_code(from_native(p2, &arguments[2]));
            runtime::translate_error_code(from_native(p3, &arguments[3]));
            runtime::translate_error_code(from_native(p4, &arguments[4]));
            fill_rest(p5, 5, arguments);

            return arguments;
        }

        template <class P1, class P2, class P3, class P4>
        static std::vector<JsValueRef> pack_arguments(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            unsigned argumentCount = 4 + rest_argument_count(p4);
            std::vector<JsValueRef> arguments(argumentCount);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            runtime::translate_error_code(from_native(p1, &arguments[1]));
            runtime::translate_error_code(from_native(p2, &arguments[2]));
            runtime::translate_error_code(from_native(p3, &arguments[3]));
            fill_rest(p4, 4, arguments);

            return arguments;
        }

        template <class P1, class P2, class P3>
        static std::vector<JsValueRef> pack_arguments(value thisValue, P1 p1, P2 p2, P3 p3)
        {
            unsigned argumentCount = 3 + rest_argument_count(p3);
            std::vector<JsValueRef> arguments(argumentCount);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            runtime::translate_error_code(from_native(p1, &arguments[1]));
            runtime::translate_error_code(from_native(p2, &arguments[2]));
            fill_rest(p3, 3, arguments);

            return arguments;
        }

        template <class P1, class P2>
        static std::vector<JsValueRef> pack_arguments(value thisValue, P1 p1, P2 p2)
        {
            unsigned argumentCount = 2 + rest_argument_count(p2);
            std::vector<JsValueRef> arguments(argumentCount);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            runtime::translate_error_code(from_native(p1, &arguments[1]));
            fill_rest(p2, 2, arguments);

            return arguments;
        }

        template <class P1>
        static std::vector<JsValueRef> pack_arguments(value thisValue, P1 p1)
        {
            unsigned argumentCount = 1 + rest_argument_count(p1);
            std::vector<JsValueRef> arguments(argumentCount);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            fill_rest(p1, 1, arguments);

            return arguments;
        }

        static std::vector<JsValueRef> pack_arguments(value thisValue)
        {
            std::vector<JsValueRef> arguments(1);

            if (thisValue.is_valid())
            {
                arguments[0] = thisValue.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            return arguments;
        }

    public:
        function_base() :
            object()
        {
        }

        explicit function_base(value object) :
            object(object.handle())
        {
        }

        value call(value thisValue, std::vector<value> arguments)
        {
            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), (JsValueRef *) arguments.data(), arguments.size(), &resultValue));
            return value(resultValue);
        }
    };

    template<class R>
    class constructor_function : public function_base
    {
    public:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }

        typename R::prototype_proxy constructor_prototype()
        {
            return get_property<R::prototype_proxy>(property_id::create(L"prototype"));
        }
    };

    template<>
    class constructor_function<std::wstring> : public function_base
    {
    public:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }

        // TODO: Prototype for primitive type
    };

    template<>
    class constructor_function<double> : public function_base
    {
    public:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }

        // TODO: Prototype for primitive type
    };

    template<>
    class constructor_function<bool> : public function_base
    {
    public:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }

        // TODO: Prototype for primitive type
    };

    // Arity = 8

    template<class R = notdefined, class P1 = notdefined, class P2 = notdefined, class P3 = notdefined, class P4 = notdefined, class P5 = notdefined, class P6 = notdefined, class P7 = notdefined, class P8 = notdefined>
    class function : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8);

        function<R, P1, P2, P3, P4, P5, P6, P7, P8>() :
            constructor_function<R>()
        {
        }

        function<R, P1, P2, P3, P4, P5, P6, P7, P8>(Signature function) :
            constructor_function<R>()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<R, P1, P2, P3, P4, P5, P6, P7, P8>(value object) :
            constructor_function<R>(object)
        {
        }

    private:

        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;
            P7 p7;
            P8 p8;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4, p5, p6, p7, p8))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue, p1, p2, p3, p4, p5, p6, p7, p8);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4, p5, p6, p7, p8);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            std::vector<JsValueRef> arguments = pack_arguments(value(), p1, p2, p3, p4, p5, p6, p7, p8);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R, P1, P2, P3, P4, P5, P6, P7, P8> create(Signature function)
        {
            return function<R, P1, P2, P3, P4, P5, P6, P7, P8>(function);
        }
    };

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    class function<void, P1, P2, P3, P4, P5, P6, P7, P8> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8);

        function<void, P1, P2, P3, P4, P5, P6, P7, P8>() :
            function_base()
        {
        }

        function<void, P1, P2, P3, P4, P5, P6, P7, P8>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, P1, P2, P3, P4, P5, P6, P7, P8>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;
            P7 p7;
            P8 p8;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4, p5, p6, p7, p8))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue, p1, p2, p3, p4, p5, p6, p7, p8);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4, p5, p6, p7, p8);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void, P1, P2, P3, P4, P5, P6, P7, P8> create(Signature function)
        {
            return function<void, P1, P2, P3, P4, P5, P6, P7, P8>(function);
        }
    };

    // Arity = 7

    template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    class function<R, P1, P2, P3, P4, P5, P6, P7, notdefined> : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7);

        function<R, P1, P2, P3, P4, P5, P6, P7, notdefined>() :
            constructor_function<R>()
        {
        }

        function<R, P1, P2, P3, P4, P5, P6, P7, notdefined>(Signature function) :
            constructor_function<R>()
        {
                runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<R, P1, P2, P3, P4, P5, P6, P7, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    private:

        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;
            P7 p7;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4, p5, p6, p7))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue, p1, p2, p3, p4, p5, p6, p7);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4, p5, p6, p7);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            std::vector<JsValueRef> arguments = pack_arguments(value(), p1, p2, p3, p4, p5, p6, p7);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R, P1, P2, P3, P4, P5, P6, P7> create(Signature function)
        {
            return function<R, P1, P2, P3, P4, P5, P6, P7>(function);
        }
    };

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    class function<void, P1, P2, P3, P4, P5, P6, P7, notdefined> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7);

        function<void, P1, P2, P3, P4, P5, P6, P7, notdefined>() :
            function_base()
        {
        }

        function<void, P1, P2, P3, P4, P5, P6, P7, notdefined>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, P1, P2, P3, P4, P5, P6, P7, notdefined>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;
            P7 p7;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4, p5, p6, p7))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue, p1, p2, p3, p4, p5, p6, p7);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4, p5, p6, p7);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void, P1, P2, P3, P4, P5, P6, P7> create(Signature function)
        {
            return function<void, P1, P2, P3, P4, P5, P6, P7>(function);
        }
    };

    // Arity = 6

    template<class R, class P1, class P2, class P3, class P4, class P5, class P6>
    class function<R, P1, P2, P3, P4, P5, P6, notdefined, notdefined> : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6);

        function<R, P1, P2, P3, P4, P5, P6, notdefined, notdefined>() :
            constructor_function<R>()
        {
            }

        function<R, P1, P2, P3, P4, P5, P6, notdefined, notdefined>(Signature function) :
            constructor_function<R>()
        {
                runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
            }

        explicit function<R, P1, P2, P3, P4, P5, P6, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
            }

    private:

        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4, p5, p6))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue, p1, p2, p3, p4, p5, p6);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4, p5, p6);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            std::vector<JsValueRef> arguments = pack_arguments(value(), p1, p2, p3, p4, p5, p6);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R, P1, P2, P3, P4, P5, P6> create(Signature function)
        {
            return function<R, P1, P2, P3, P4, P5, P6>(function);
        }
    };

    template<class P1, class P2, class P3, class P4, class P5, class P6>
    class function<void, P1, P2, P3, P4, P5, P6, notdefined, notdefined> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6);

        function<void, P1, P2, P3, P4, P5, P6, notdefined, notdefined>() :
            function_base()
        {
        }

        function<void, P1, P2, P3, P4, P5, P6, notdefined, notdefined>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, P1, P2, P3, P4, P5, P6, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4, p5, p6))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue, p1, p2, p3, p4, p5, p6);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4, p5, p6);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void, P1, P2, P3, P4, P5, P6> create(Signature function)
        {
            return function<void, P1, P2, P3, P4, P5, P6>(function);
        }
    };

    // Arity = 5

    template<class R, class P1, class P2, class P3, class P4, class P5>
    class function<R, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

        function<R, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
            }

        function<R, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>(Signature function) :
            constructor_function<R>()
        {
                runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
            }

        explicit function<R, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
            }

    private:

        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4, p5))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue, p1, p2, p3, p4, p5);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4, p5);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            std::vector<JsValueRef> arguments = pack_arguments(value(), p1, p2, p3, p4, p5);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R, P1, P2, P3, P4, P5> create(Signature function)
        {
            return function<R, P1, P2, P3, P4, P5>(function);
        }
    };

    template<class P1, class P2, class P3, class P4, class P5>
    class function<void, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

        function<void, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        function<void, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4, p5))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue, p1, p2, p3, p4, p5);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4, p5);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void, P1, P2, P3, P4, P5> create(Signature function)
        {
            return function<void, P1, P2, P3, P4, P5>(function);
        }
    };

    // Arity = 4

    template<class R, class P1, class P2, class P3, class P4>
    class function<R, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4);

        function<R, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
            }

        function<R, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>(Signature function) :
            constructor_function<R>()
        {
                runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
            }

        explicit function<R, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
            }

    private:

        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue, p1, p2, p3, p4);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4)
        {
            std::vector<JsValueRef> arguments = pack_arguments(value(), p1, p2, p3, p4);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R, P1, P2, P3, P4> create(Signature function)
        {
            return function<R, P1, P2, P3, P4>(function);
        }
    };

    template<class P1, class P2, class P3, class P4>
    class function<void, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4);

        function<void, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        function<void, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3, p4))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue, p1, p2, p3, p4);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3, p4);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void, P1, P2, P3, P4> create(Signature function)
        {
            return function<void, P1, P2, P3, P4>(function);
        }
    };

    // Arity = 3

    template<class R, class P1, class P2, class P3>
    class function<R, P1, P2, P3, notdefined> : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3);

        function<R, P1, P2, P3, notdefined>() :
            constructor_function<R>()
        {
        }

        function<R, P1, P2, P3, notdefined>(Signature function) :
            constructor_function<R>()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<R, P1, P2, P3, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue, p1, p2, p3);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue, P1 p1, P2 p2, P3 p3)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        R construct(P1 p1, P2 p2, P3 p3)
        {
            std::vector<JsValueRef> arguments = pack_arguments(value(), p1, p2, p3);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R, P1, P2, P3> create(Signature function)
        {
            return function<R, P1, P2, P3>(function);
        }
    };

    template<class P1, class P2, class P3>
    class function<void, P1, P2, P3, notdefined> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue, P1 p1, P2 p2, P3 p3);

        function<void, P1, P2, P3, notdefined>() :
            function_base()
        {
        }

        function<void, P1, P2, P3, notdefined>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, P1, P2, P3, notdefined>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;
            P3 p3;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2, p3))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue, p1, p2, p3);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue, P1 p1, P2 p2, P3 p3)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2, p3);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void, P1, P2, P3> create(Signature function)
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }
    };

    // Arity = 2

    template<class R, class P1, class P2>
    class function<R, P1, P2, notdefined, notdefined> : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue, P1 p1, P2 p2);

        function<R, P1, P2, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        function<R, P1, P2, notdefined, notdefined>(Signature function) :
            constructor_function<R>()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<R, P1, P2, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue, p1, p2);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue, P1 p1, P2 p2)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        R construct(P1 p1, P2 p2)
        {
            std::vector<JsValueRef> arguments = pack_arguments(value(), p1, p2);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R, P1, P2> create(Signature function)
        {
            return function<R, P1, P2>(function);

        }
    };

    template<class P1, class P2>
    class function<void, P1, P2, notdefined, notdefined> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue, P1 p1, P2 p2);

        function<void, P1, P2, notdefined, notdefined>() :
            function_base()
        {
        }

        function<void, P1, P2, notdefined, notdefined>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, P1, P2, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;
            P1 p1;
            P2 p2;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1, p2))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue, p1, p2);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue, P1 p1, P2 p2)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1, p2);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void, P1, P2> create(Signature function)
        {
            return function<void, P1, P2>(function);
        }
    };

    // Arity = 1

    template<class R, class P1>
    class function<R, P1, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue, P1 p1);

        function<R, P1, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        function<R, P1, notdefined, notdefined, notdefined>(Signature function) :
            constructor_function<R>()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<R, P1, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;
            P1 p1;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue, p1);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue, P1 p1)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        R construct(P1 p1)
        {
            std::vector<JsValueRef> arguments = pack_arguments(value(), p1);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R, P1> create(Signature function)
        {
            return function<R, P1>(function);
        }
    };

    template<class P1>
    class function<void, P1, notdefined, notdefined, notdefined> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue, P1 p1);

        function<void, P1, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        function<void, P1, notdefined, notdefined, notdefined>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, P1, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;
            P1 p1;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue, p1))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue, p1);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue, P1 p1)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue, p1);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void, P1> create(Signature function)
        {
            return function<void, P1>(function);
        }
    };

    // Arity = 0

    template<class R>
    class function<R, notdefined, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
    public:
        typedef R(CALLBACK *Signature)(value thisValue);

        function<R, notdefined, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        function<R, notdefined, notdefined, notdefined, notdefined>(Signature function) :
            constructor_function<R>()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<R, notdefined, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            object calleeObject;
            value thisValue;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            R result = callback(thisValue);

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                // TODO: Include typename.
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R call(value thisValue)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, result));
            return result;
        }

        R construct()
        {
            std::vector<JsValueRef> arguments = pack_arguments(value());

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        static function<R> create(Signature function)
        {
            return function<R>(function);
        }
    };

    template<>
    class function<void, notdefined, notdefined, notdefined, notdefined> : public function_base
    {
    public:
        typedef void (CALLBACK *Signature)(value thisValue);

        function<void, notdefined, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        function<void, notdefined, notdefined, notdefined, notdefined>(Signature function) :
            function_base()
        {
            runtime::translate_error_code(JsCreateFunction(thunk, function, &_ref));
        }

        explicit function<void, notdefined, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    private:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
        {
            if (isConstructCall)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            object calleeObject;
            value thisValue;

            if (!unpack_arguments(callee, arguments, argumentCount, calleeObject, thisValue))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callbackState;
            callback(thisValue);

            return JS_INVALID_REFERENCE;
        }

    public:
        void call(value thisValue)
        {
            std::vector<JsValueRef> arguments = pack_arguments(thisValue);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), arguments.size(), &resultValue));
        }

        static function<void> create(Signature functionSignature)
        {
            return function<void>(functionSignature);
        }
    };

    template<class T>
    class property_descriptor : public object
    {
    private:
        property_descriptor(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        property_descriptor() :
            object()
        {
        }

        explicit property_descriptor(value object) :
            object(object.handle())
        {
        }

        bool writable()
        {
            return get_property<bool>(property_id::create(L"writable"));
        }

        void set_writable(bool value)
        {
            set_property<bool>(property_id::create(L"writable"), value);
        }

        bool enumerable()
        {
            return get_property<bool>(property_id::create(L"enumerable"));
        }

        void set_enumerable(bool value)
        {
            set_property(property_id::create(L"enumerable"), value);
        }

        bool configurable()
        {
            return get_property<bool>(property_id::create(L"configurable"));
        }

        void set_configurable(bool value)
        {
            set_property(property_id::create(L"configurable"), value);
        }

        T value()
        {
            return get_property<T>(property_id::create(L"value"));
        }

        void set_value(T value)
        {
            set_property(property_id::create(L"value"), value);
        }

        function<T> getter()
        {
            return get_property<function<T>>(property_id::create(L"get"));
        }

        void set_getter(function<T> value)
        {
            set_property(property_id::create(L"get"), value);
        }

        function<void, T> setter()
        {
            return get_property<function<void, T>>(property_id::create(L"set"));
        }

        void setter(function<void, T> value)
        {
            set_property(property_id::create(L"set"), value);
        }

        static property_descriptor<T> create(function<T> getter, function<void, T> setter)
        {
            property_descriptor<T> desc = (property_descriptor<T>)value::create();
            desc.get = getter;
            desc.set = setter;
            return desc;
        }
    };

    class exception
    {
    protected:
        exception() { }
    };

    class invalid_cast_exception : public exception
    {
    };

    class invalid_argument_exception : public exception
    {
    };

    class out_of_memory_exception : public exception
    {
    };

    class in_disabled_state_exception : public exception
    {
    };

    class null_argument_exception : public exception
    {
    };

    class no_current_context_exception : public exception
    {
    };

    class in_exception_state_exception : public exception
    {
    };

    class not_implemented_exception : public exception
    {
    };

    class wrong_thread_exception : public exception
    {
    };

    class runtime_in_use_exception : public exception
    {
    };

    class cannot_disable_execution_exception : public exception
    {
    };

    class heap_enum_in_progress_exception : public exception
    {
    };

    class argument_not_object_exception : public exception
    {
    };

    class in_profile_callback_exception : public exception
    {
    };

    class in_thread_service_callback_exception : public exception
    {
    };

    class cannot_serialize_debug_script_exception : public exception
    {
    };

    class already_debugging_context_exception : public exception
    {
    };

    class already_profiling_context_exception : public exception
    {
    };

    class idle_not_enabled_exception : public exception
    {
    };

    class bad_serialized_script_exception : public exception
    {
    };

    class script_terminated_exception : public exception
    {
    };

    class script_eval_disabled_exception : public exception
    {
    };

    class fatal_exception : public exception
    {
    };

    class script_exception : public exception
    {
    private:
        value _error;

    public:
        script_exception(value error) :
            _error(error)
        {
        }

        value error()
        {
            return _error;
        }
    };

    class compile_error : public error
    {
        friend class runtime;

    private:
        compile_error(JsValueRef ref) :
            error(ref)
        {
        }

    public:
        compile_error() :
            error()
        {
        }

        explicit compile_error(value object) :
            error(object.handle())
        {
        }

        std::wstring message()
        {
            return get_property<std::wstring>(property_id::create(L"message"));
        }

        void set_message(std::wstring value)
        {
            set_property<std::wstring>(property_id::create(L"message"), value);
        }

        double line()
        {
            return get_property<double>(property_id::create(L"line"));
        }

        void set_line(double value)
        {
            set_property(property_id::create(L"line"), value);
        }

        double column()
        {
            return get_property<double>(property_id::create(L"column"));
        }

        void set_column(double value)
        {
            set_property(property_id::create(L"column"), value);
        }

        double length()
        {
            return get_property<double>(property_id::create(L"length"));
        }

        void set_length(double value)
        {
            set_property(property_id::create(L"length"), value);
        }

        std::wstring source()
        {
            return get_property<std::wstring>(property_id::create(L"source"));
        }

        void set_source(std::wstring value)
        {
            set_property(property_id::create(L"source"), value);
        }
    };

    class script_compile_exception
    {
    private:
        compile_error _error;

    public:
        script_compile_exception(compile_error error) :
            _error(error)
        {
        }

        compile_error error()
        {
            return _error;
        }
    };
}
