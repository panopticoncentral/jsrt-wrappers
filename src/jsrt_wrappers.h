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

    /// <summary>
    ///     A templated class that pins a reference on the Chakra heap.
    /// </summary>
    /// <remarks>
    ///     Chakra handles are only visible to the Chakra garbage collector if they are stored
    ///     inside another Chakra object or are on the stack. If a handle is stored outside of
    ///     these two places, it needs to be pinned using this class.
    /// </remarks>
    template<class T>
    class pinned
    {
    private:
        T _reference;

    public:
        /// <summary>
        ///     Constructs an invalid pinned reference.
        /// </summary>
        pinned() :
            _reference()
        {
        }

        /// <summary>
        ///     Constructs a pinned reference from a reference.
        /// </summary>
        /// <param name="reference">The reference to pin.</param>
        pinned(T reference) :
            _reference(reference)
        {
            if (_reference.is_valid())
            {
                _reference.add_reference();
            }
        }

        /// <summary>
        ///     Constructs a pinned reference from a reference.
        /// </summary>
        /// <param name="reference">The reference to pin.</param>
        pinned(const pinned<T>& right) :
            _reference(right._reference)
        {
            if (_reference.is_valid())
            {
                _reference.add_reference();
            }
        }

        pinned(pinned<T>&& right) :
            _reference(right._reference)
        {
            right._reference = T();
        }

        /// <summary>
        ///     Releases a pinned reference.
        /// </summary>
        void release()
        {
            if (_reference.is_valid())
            {
                _reference.release();
                _reference = T();
            }
        }

        ~pinned()
        {
            release();
        }

        T* operator->() const
        {
            return (T *) &_reference;
        }

        T operator*() const
        {
            return _reference;
        }

        pinned<T>& operator=(const pinned<T>& right)
        {
            if (_reference.is_valid())
            {
                _reference.release();
            }
            _reference = right._reference;
            if (_reference.is_valid())
            {
                _reference.add_reference();
            }
            return *this;
        }

        pinned<T>& operator=(pinned<T>&& right)
        {
            if (this != &right)
            {
                if (_reference.is_valid())
                {
                    _reference.release();
                }
                _reference = right._reference;
                right._reference = T();
            }
            return *this;
        }
    };

    /// <summary>
    ///     A reference to a script context.
    /// </summary>
    /// <remarks>
    ///     <para>
    ///     Each script context contains its own global object, distinct from the global object in 
    ///     other script contexts.
    ///     </para>
    ///     <para>
    ///     Many Chakra hosting APIs require an "active" script context, which can be set using 
    ///     a <c>context::scope</c>. Chakra hosting APIs that require a current context to be set 
    ///     will note that explicitly in their documentation.
    ///     </para>
    /// </remarks>
    class context : public reference
    {
        friend class runtime;

    private:
        context(JsContextRef context) :
            reference(context)
        {
        }

    public:
        /// <summary>
        ///     Constructs an invalid context reference.
        /// </summary>
        context() :
            reference()
        {
        }

        /// <summary>
        ///     Establishes a context as "current" for the life of the object.
        /// </summary>
        /// <remarks>
        ///     Scopes should only be declared on the stack.
        /// </remarks>
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
            /// <summary>
            ///     Construct a scope object, setting the context as current.
            /// </summary>
            /// <param name="context">The context to set current.</param>
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

        /// <summary>
        ///     Gets the runtime that the context belongs to.
        /// </summary>
        runtime parent()
        {
            JsRuntimeHandle runtimeHandle;
            runtime::translate_error_code(JsGetRuntime(_ref, &runtimeHandle));
            return runtime(runtimeHandle);
        }

        /// <summary>
        ///     Starts profiling in the current context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="callback">The profiling callback to use.</param>
        /// <param name="event_mask">The profiling events to callback with.</param>
        /// <param name="profiling_context">A context to pass to the profiling callback.</param>
        static void start_profiling(IActiveScriptProfilerCallback *callback, PROFILER_EVENT_MASK event_mask = PROFILER_EVENT_MASK_TRACE_ALL, unsigned long profiling_context = 0)
        {
            runtime::translate_error_code(JsStartProfiling(callback, event_mask, profiling_context));
        }

        /// <summary>
        ///     Stops profiling in the current context.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Will not throw an error if profiling has not started.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="reason">
        ///     The reason for stopping profiling to pass to the profiler callback.
        /// </param>
        static void stop_profiling(HRESULT reason)
        {
            runtime::translate_error_code(JsStopProfiling(reason));
        }

        /// <summary>
        ///     Starts debugging in the current context.
        /// </summary>
        /// <param name="debug_application">The debug application to use for debugging.</param>
        static void start_debugging(IDebugApplication *debug_application);

        /// <summary>
        ///     Gets the current script context on the thread.
        /// </summary>
        static context current()
        {
            JsContextRef contextHandle;
            runtime::translate_error_code(JsGetCurrentContext(&contextHandle));

            return context(contextHandle);
        }

        /// <summary>
        ///     Tells the runtime to do any idle processing it need to do.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     If idle processing has been enabled for the current runtime, calling <c>idle</c> will 
        ///     inform the current runtime that the host is idle and that the runtime can perform 
        ///     memory cleanup tasks.
        ///     </para>
        ///     <para>
        ///     <c>idle</c> returns the number of system ticks until there will be more idle work
        ///     for the runtime to do. Calling <c>idle</c> before this number of ticks has passed will do
        ///     no work.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>
        ///     The next system tick when there will be more idle work to do. Can be null. Returns the 
        ///     maximum number of ticks if there no upcoming idle work to do.
        /// </returns>
        static unsigned int idle()
        {
            unsigned int nextIdleTick;
            runtime::translate_error_code(JsIdle(&nextIdleTick));
            return nextIdleTick;
        }

        /// <summary>
        ///     Determines whether the runtime of the current context is in an exception state.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     If a call into the runtime results in an exception (either as the result of running a 
        ///     script or due to something like a conversion failure), the runtime is placed into an 
        ///     "exception state." All calls into any context created by the runtime (except for the 
        ///     exception APIs) will throw a <c>in_exception_state_exception</c> until the exception is 
        ///     cleared.
        ///     </para>
        ///     <para>
        ///     If the runtime of the current context is in the exception state when a callback returns 
        ///     into the engine, the engine will automatically rethrow the exception.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>
        ///     Whether the runtime of the current context is in the exception state.
        /// </returns>
        static bool has_exception()
        {
            bool hasException;
            runtime::translate_error_code(JsHasException(&hasException));
            return hasException;
        }

        /// <summary>
        ///     Sets the runtime of the current context to an exception state.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     If the runtime of the current context is already in an exception state, this API will 
        ///     throw <c>in_exception_state_exception</c>.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="exception">
        ///     The JavaScript exception to set for the runtime of the current context.
        /// </param>
        static void set_exception(value exception);

        /// <summary>
        ///     Returns the exception that caused the runtime of the current context to be in the 
        ///     exception state and resets the exception state for that runtime.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     If the runtime of the current context is not in an exception state, this API will throw a
        ///     <c>invalid_argument_exception</c>. If the runtime is disabled, this will throw an exception
        ///     indicating that the script was terminated, but it will not clear the exception (the 
        ///     exception will be cleared if the runtime is re-enabled using 
        ///     <c>enable_execution</c>).
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>The exception for the runtime of the current context.</returns>
        static value get_and_clear_exception();

        /// <summary>
        ///     Parses a script and returns a function representing the script.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="script">The script to parse.</param>
        /// <param name="source_context">
        ///     A cookie identifying the script that can be used by debuggable script contexts.
        /// </param>
        /// <param name="source_url">The location the script came from.</param>
        /// <returns>A function representing the script code.</returns>
        static function_base parse(std::wstring script, JsSourceContext source_context = JS_SOURCE_CONTEXT_NONE, std::wstring source_url = std::wstring());

        /// <summary>
        ///     Executes a script.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="script">The script to run.</param>
        /// <param name="source_context">
        ///     A cookie identifying the script that can be used by debuggable script contexts.
        /// </param>
        /// <param name="source_url">The location the script came from.</param>
        static void run(std::wstring script, JsSourceContext source_context = JS_SOURCE_CONTEXT_NONE, std::wstring source_url = std::wstring())
        {
            runtime::translate_error_code(JsRunScript(script.c_str(), source_context, source_url.c_str(), nullptr));
        }

        /// <summary>
        ///     Executes a script.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="script">The script to run.</param>
        /// <param name="source_context">
        ///     A cookie identifying the script that can be used by debuggable script contexts.
        /// </param>
        /// <param name="source_url">The location the script came from.</param>
        /// <returns>The result of the script, if any.</returns>
        static value evaluate(std::wstring script, JsSourceContext source_context = JS_SOURCE_CONTEXT_NONE, std::wstring source_url = std::wstring());

        /// <summary>
        ///     Serializes a parsed script to a buffer than can be reused.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     <c>serialize</c> parses a script and then stores the parsed form of the script in a 
        ///     runtime-independent format. The serialized script then can be deserialized in any
        ///     runtime without requiring the script to be re-parsed.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="script">The script to serialize.</param>
        /// <param name="buffer">The buffer to put the serialized script into. Can be null.</param>
        /// <param name="buffer_size">
        ///     The size of the input buffer. Should be 0 if the buffer is null.
        /// </param>
        /// <returns>
        ///     The size of the buffer, in bytes, required to hold the serialized script.
        /// </returns>
        static unsigned long serialize(std::wstring script, unsigned char *buffer, unsigned long buffer_size)
        {
            runtime::translate_error_code(JsSerializeScript(script.c_str(), buffer, &buffer_size));
            return buffer_size;
        }

        /// <summary>
        ///     Parses a serialized script and returns a function representing the script.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="script">The script to parse.</param>
        /// <param name="buffer">The serialized script.</param>
        /// <param name="source_context">
        ///     A cookie identifying the script that can be used by debuggable script contexts.
        /// </param>
        /// <param name="source_url">The location the script came from.</param>
        /// <returns>A function representing the script code.</returns>
        static function_base parse_serialized(std::wstring script, unsigned char *buffer, JsSourceContext source_context = JS_SOURCE_CONTEXT_NONE, std::wstring source_url = std::wstring());

        /// <summary>
        ///     Runs a serialized script.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="script">The source code of the serialized script.</param>
        /// <param name="buffer">The serialized script.</param>
        /// <param name="source_context">
        ///     A cookie identifying the script that can be used by debuggable script contexts.
        /// </param>
        /// <param name="source_url">The location the script came from.</param>
        static void run_serialized(std::wstring script, unsigned char *buffer, JsSourceContext source_context = JS_SOURCE_CONTEXT_NONE, std::wstring source_url = std::wstring())
        {
            runtime::translate_error_code(JsRunSerializedScript(script.c_str(), buffer, source_context, source_url.c_str(), nullptr));
        }

        /// <summary>
        ///     Runs a serialized script.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="script">The source code of the serialized script.</param>
        /// <param name="buffer">The serialized script.</param>
        /// <param name="source_context">
        ///     A cookie identifying the script that can be used by debuggable script contexts.
        /// </param>
        /// <param name="source_url">The location the script came from.</param>
        /// <returns>
        ///     The result of running the script, if any.
        /// </returns>
        static value evaluate_serialized(std::wstring script, unsigned char *buffer, JsSourceContext source_context = JS_SOURCE_CONTEXT_NONE, std::wstring source_url = std::wstring());

        /// <summary>
        ///     Gets the value of <c>undefined</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The <c>undefined</c> value.</returns>
        static value undefined();
    };

    /// <summary>
    ///     A property identifier.
    /// </summary>
    /// <remarks>
    ///     Property identifiers are used to refer to properties of JavaScript objects instead of using
    ///     strings.
    /// </remarks>
    class property_id : public reference
    {
    private:
        property_id(JsPropertyIdRef propertyId) :
            reference(propertyId)
        {
        }

    public:
        /// <summary>
        ///     Constructs an invalid property ID.
        /// </summary>
        property_id() :
            reference()
        {
        }

        /// <summary>
        ///     Gets the name associated with the property ID.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>The name associated with the property ID.</returns>
        const std::wstring name()
        {
            const wchar_t *result;
            runtime::translate_error_code(JsGetPropertyNameFromId(_ref, &result));
            return result;
        }

        /// <summary>
        ///     Gets the property ID associated with the name. 
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     Property IDs are specific to a context and cannot be used across contexts.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="name">
        ///     The name of the property ID to get or create. The name may consist of only digits.
        /// </param>
        /// <returns>The property ID in this runtime for the given name.</returns>
        static property_id create(const std::wstring name)
        {
            JsPropertyIdRef propertyId;
            runtime::translate_error_code(JsGetPropertyIdFromName(name.c_str(), &propertyId));
            return property_id(propertyId);
        }
    };

    /// <summary>
    ///     An optional value.
    /// </summary>
    template<class T>
    class optional
    {
    private:
        bool _hasValue;
        T _value;

    public:
        /// <summary>
        ///     Constructs an empty optional value.
        /// </summary>
        optional() :
            _hasValue(false),
            _value()
        {
        }

        /// <summary>
        ///     Constructs an optional value with a value in it.
        /// </summary>
        optional(T value) :
            _hasValue(true),
            _value(value)
        {
        }

        /// <summary>
        ///     Returns whether the optional value has a value in it.
        /// </summary>
        bool has_value()
        {
            return _hasValue;
        }

        /// <summary>
        ///     Clears the optional value.
        /// </summary>
        void clear()
        {
            _hasValue = false;
            _value = T();
        }

        /// <summary>
        ///     Gets the optional value.
        /// </summary>
        T value()
        {
            return _value;
        }
    };

    /// <summary>
    ///     Represents a variable number of values.
    /// </summary>
    template<class T>
    class rest : public optional<T>
    {
    public:
        /// <summary>
        ///     Constructs a variable number of values with no values.
        /// </summary>
        rest() :
            optional()
        {
        }

        /// <summary>
        ///     Constructs a variable number of values.
        /// </summary>
        rest(T value) :
            optional(value)
        {
        }
    };

    /// <summary>
    ///     A reference to a JavaScript value.
    /// </summary>
    class value : public reference
    {
        friend class function_base;
        friend class runtime;
        friend class context;
        friend class object;

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
        /// <summary>
        ///     Creates an invalid value handle.
        /// </summary>
        value() :
            reference()
        {
        }

        /// <summary>
        ///     Gets the JavaScript type of a value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The type of the value.</returns>
        JsValueType type() const
        {
            JsValueType type;
            runtime::translate_error_code(JsGetValueType(_ref, &type));
            return type;
        }

        /// <summary>
        ///     Converts the value to a <c>VARIANT</c> as a projection of a JavaScript value.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     The projection <c>VARIANT</c> can be used by COM automation clients to call into the
        ///     projected JavaScript object.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="variant">
        ///     A pointer to a <c>VARIANT</c> struct that will be initialized as a projection.
        /// </param>
        void to_variant(VARIANT *variant)
        {
            runtime::translate_error_code(JsValueToVariant(_ref, variant));
        }

        /// <summary>
        ///     Compare two JavaScript values for equality.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     This function is equivalent to the <c>==</c> operator in Javascript.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="other>The object to compare.</param>
        /// <returns>Whether the values are equal.</returns>
        bool equals(value other)
        {
            bool isEqual;
            runtime::translate_error_code(JsEquals(handle(), other.handle(), &isEqual));
            return isEqual;
        }

        /// <summary>
        ///     Compare two JavaScript values for strict equality.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     This function is equivalent to the <c>===</c> operator in Javascript.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="other>The object to compare.</param>
        /// <returns>Whether the values are equal.</returns>
        bool strict_equals(value other)
        {
            bool isEqual;
            runtime::translate_error_code(JsStrictEquals(handle(), other.handle(), &isEqual));
            return isEqual;
        }

        /// <summary>
        ///     Creates a JavaScript value that is a projection of the passed in <c>VARIANT</c>.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     The projected value can be used by script to call a COM automation object from script. 
        ///     Hosts are responsible for enforcing COM threading rules.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <param name="variant_value">A <c>VARIANT</c> to be projected.</param>
        /// <returns>A JavaScript value that is a projection of the <c>VARIANT</c>.</returns>
        static value from_variant(VARIANT *variant_value)
        {
            JsValueRef result;
            runtime::translate_error_code(JsVariantToValue(variant_value, &result));
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

        /// <summary>
        ///     Converts the value to Boolean using standard JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        static boolean convert(value value);
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

        /// <summary>
        ///     Converts the value to number using standard JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        number convert(value value);
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

        /// <summary>
        ///     Converts the value to string using standard JavaScript semantics.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        string convert(value value);
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

            if (name.has_value() && name.value().type() == JsString)
            {
                return ((string) name.value()).data();
            }

            return L"";
        }

        std::wstring message()
        {
            optional<value> message = get_property<value>(property_id::create(L"message"));

            if (message.has_value() && message.value().type() == JsString)
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
            runtime::translate_error_code(to_native(resultValue, &result));
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

    class script_compile_exception : public exception
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
