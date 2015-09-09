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

#include <algorithm>
#include <string>
#include <vector>
#include <initializer_list>

#pragma once

namespace jsrt
{
    struct notdefined {};

    class value;
    template<class T>
    class property_descriptor;
    class context;
    class function_base;
    class boolean;
    class number;
    class string;
    class object;
    template<class R = notdefined, class P1 = notdefined, class P2 = notdefined, class P3 = notdefined, class P4 = notdefined, class P5 = notdefined, class P6 = notdefined, class P7 = notdefined, class P8 = notdefined>
    class function;

    /// <summary>
    ///     A class that wraps a handle to a Chakra runtime.
    /// </summary>
    class runtime
    {
        friend class context;

    private:
        JsRuntimeHandle _handle;

        explicit runtime(JsRuntimeHandle runtime) :
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
        /// <param name="callback_state">
        ///     User provided state that will be passed back to the callback.
        /// </param>
        /// <param name="allocationCallback">
        ///     Memory allocation callback to be called for memory allocation events.
        /// </param>
        void set_memory_allocation_callback(void *callback_state, JsMemoryAllocationCallback allocationCallback);

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
        /// <param name="callback_state">
        ///     User provided state that will be passed back to the callback.
        /// </param>
        /// <param name="beforeCollectCallback">The callback function being set.</param>
        void set_before_collect_callback(void *callback_state, JsBeforeCollectCallback beforeCollectCallback);

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
        ///     If the error code is not <c>JsNoError</c>, this will throw the corresponding
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

        explicit reference(JsRef ref) :
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

        bool operator ==(const reference &other)
        {
            return this->handle() == other.handle();
        }

        bool operator !=(const reference &other)
        {
            return this->handle() != other.handle();
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
        explicit context(JsContextRef context) :
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
        ///     Enumerates the heap of the current context.
        /// </summary>
        /// <remarks>
        ///     <para>
        ///     While the heap is being enumerated, the current context cannot be removed, and all
        ///     calls to modify the state of the context will fail until the heap enumerator is 
        ///     released.
        ///     </para>
        ///     <para>
        ///     Requires an active script context.
        ///     </para>
        /// </remarks>
        /// <returns>The heap enumerator.</returns>
        static IActiveScriptProfilerHeapEnum *enumerate_heap();

        /// <summary>
        ///     Returns a value that indicates whether the heap of the current context is being 
        ///     enumerated.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>Whether the heap is being enumerated.</returns>
        static bool is_enumerating_heap();

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

        /// <summary>
        ///     Gets the value of <c>null</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The <c>null</c> value.</returns>
        static value null();

        /// <summary>
        ///     Gets the global object in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The global object.</returns>
        static object global();
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
    ///     A missing optional value.
    /// </summary>
    struct missing
    {
    };

    template<class T> 
    struct optional_string_type
    {
        // bogus incomplete type for T != std::wstring
        struct type;
    };

    template<> 
    struct optional_string_type<std::wstring>
    {
        typedef const wchar_t *type;
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
        ///     Constructs an optional missing value.
        /// </summary>
        optional(missing value) :
            _hasValue(false),
            _value()
        {
        }

        /// <summary>
        ///     Constructs an optional string value from a buffer.
        /// </summary>
        optional(typename optional_string_type<T>::type value) :
            optional((std::wstring)value)
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
    ///     A reference to a JavaScript value.
    /// </summary>
    class value : public reference
    {
        friend class function_base;
        friend class runtime;
        friend class context;
        friend class object;

    protected:
        template<class T>
        static JsErrorCode to_native(JsValueRef value, T *result)
        {
            *result = T(object(value));
            return JsNoError;
        }

        template<class T>
        static JsErrorCode to_native(JsValueRef value, optional<T> *result)
        {
            T innerValue;
            JsErrorCode error = to_native(value, &innerValue);

            if (error != JsNoError)
            {
                return error;
            }

            *result = optional<T>(innerValue);
            return JsNoError;
        }

        template<>
        static JsErrorCode to_native<int>(JsValueRef value, int *result)
        {
            // This may work in the future, so we want to prevent it from
            // falling back to the general case above.
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

        template<class T>
        static JsErrorCode from_native(optional<T> value, JsValueRef *result)
        {
            if (!value.has_value())
            {
                // shouldn't get here
                return JsErrorInvalidArgument;
            }

            return from_native(value.value(), result);
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

        template<>
        static JsErrorCode from_native(const wchar_t *value, JsValueRef *result)
        {
            if (value == nullptr)
            {
                return JsGetNullValue(result);
            }
            return JsPointerToString(value, wcslen(value), result);
        }

        explicit value(JsValueRef ref) :
            reference(ref)
        {
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
        ///     This function is equivalent to the <c>==</c> operator in JavaScript.
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
        ///     This function is equivalent to the <c>===</c> operator in JavaScript.
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

    /// <summary>
    ///     A reference to a JavaScript Boolean value.
    /// </summary>
    class boolean : public value
    {
        friend class value;

    private:
        explicit boolean(JsValueRef ref) :
            value(ref)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid value handle.
        /// </summary>
        boolean() :
            value()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to a <c>boolean</c> handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit boolean(value object) :
            value(object.handle())
        {
        }

        /// <summary>
        ///     Retrieves the <c>bool</c> value of a Boolean value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The converted value.</returns>
        bool data()
        {
            bool result;
            runtime::translate_error_code(to_native(_ref, &result));
            return result;
        }

        /// <summary>
        ///     Gets the value of <c>true</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The <c>true</c> value.</returns>
        static boolean true_value()
        {
            JsValueRef trueValue;
            runtime::translate_error_code(JsGetTrueValue(&trueValue));
            return boolean(trueValue);
        }

        /// <summary>
        ///     Gets the value of <c>false</c> in the current script context.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The <c>false</c> value.</returns>
        static boolean false_value()
        {
            JsValueRef falseValue;
            runtime::translate_error_code(JsGetFalseValue(&falseValue));
            return boolean(falseValue);
        }

        /// <summary>
        ///     Creates a Boolean value from a <c>bool</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The value to be converted.</param>
        /// <returns>The converted value.</returns>
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
        /// <param name="value">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        static boolean convert(value value);
    };

    /// <summary>
    ///     A reference to a JavaScript number value.
    /// </summary>
    class number : public value
    {
        friend class value;

    private:
        explicit number(JsValueRef ref) :
            value(ref)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid value handle.
        /// </summary>
        number() :
            value()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to a <c>number</c> handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit number(value object) :
            value(object.handle())
        {
        }

        /// <summary>
        ///     Retrieves the <c>double</c> value of a number value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The <c>double</c> value.</returns>
        double data()
        {
            double result;
            runtime::translate_error_code(to_native(_ref, &result));
            return result;
        }

        /// <summary>
        ///     Creates a number value from a <c>double</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The <c>double</c> to convert to a number value.</param>
        /// <returns>The new number value.</returns>
        static number create(double value)
        {
            JsValueRef result;
            runtime::translate_error_code(from_native(value, &result));
            return number(result);
        }

        /// <summary>
        ///     Creates a number value from an <c>int</c> value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The <c>int</c> to convert to a number value.</param>
        /// <returns>The new number value.</returns>
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
        /// <param name="value">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        static number convert(value value);
    };

    /// <summary>
    ///     A reference to a JavaScript string value.
    /// </summary>
    class string : public value
    {
        friend class value;

    private:
        explicit string(JsValueRef ref) :
            value(ref)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid value handle.
        /// </summary>
        string() :
            value()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to a <c>string</c> handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit string(value object) :
            value(object.handle())
        {
        }

        /// <summary>
        ///     Gets the length of a string value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The length of the string.</returns>
        int length()
        {
            int length;
            runtime::translate_error_code(JsGetStringLength(_ref, &length));
            return length;
        }

        /// <summary>
        ///     Returns the underlying string value.
        /// </summary>
        std::wstring data()
        {
            std::wstring result;
            runtime::translate_error_code(to_native(_ref, &result));
            return result;
        }

        /// <summary>
        ///     Creates a string value from a <c>std::wstring</c>.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="value">The string to convert to a string value.</param>
        /// <returns>The new string value.</returns>
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
        /// <param name="value">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        static string convert(value value);
    };

    /// <summary>
    ///     A reference to a JavaScript object.
    /// </summary>
    class object : public value
    {
        friend class function_base;
        friend class context;
        friend class value;

    protected:
        explicit object(JsValueRef ref) :
            value(ref)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid object handle.
        /// </summary>
        object() :
            value()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to an <c>object</c> handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit object(value object) :
            value(object.handle())
        {
        }

        /// <summary>
        ///     Determines whether an object is an external object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>Whether the object is an external object.</returns>
        bool is_external() const
        {
            bool hasExternalData;
            runtime::translate_error_code(JsHasExternalData(handle(), &hasExternalData));

            return hasExternalData;
        }

        /// <summary>
        ///     Returns a value that indicates whether an object is extensible or not.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>Whether the object is extensible or not.</returns>
        bool is_extension_allowed() const
        {
            bool isExtensionaAllowed;
            runtime::translate_error_code(JsGetExtensionAllowed(handle(), &isExtensionaAllowed));
            return isExtensionaAllowed;
        }

        /// <summary>
        ///     Returns the prototype of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The object's prototype.</returns>
        object prototype()
        {
            JsValueRef prototype;
            runtime::translate_error_code(JsGetPrototype(handle(), &prototype));
            return object(prototype);
        }

        /// <summary>
        ///     Sets the prototype of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The object's new prototype.</returns>
        void set_prototype(object prototype)
        {
            runtime::translate_error_code(JsSetPrototype(handle(), prototype.handle()));
        }

        /// <summary>
        ///     Makes an object non-extensible.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        void prevent_extension()
        {
            runtime::translate_error_code(JsPreventExtension(handle()));
        }

        /// <summary>
        ///     Gets an object's property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="name">The ID of the property.</param>
        /// <returns>The value of the property.</returns>
        template<class T = value>
        T get_property(property_id name)
        {
            JsValueRef value;
            runtime::translate_error_code(JsGetProperty(handle(), name.handle(), &value));

            T returnValue;
            runtime::translate_error_code(to_native(value, &returnValue));

            return returnValue;
        }

        /// <summary>
        ///     Gets a property descriptor for an object's own property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="name">The ID of the property.</param>
        /// <returns>The property descriptor.</returns>
        template<class T = value>
        property_descriptor<T> get_own_property_descriptor(property_id name)
        {
            JsValueRef valueHandle;
            runtime::translate_error_code(JsGetOwnPropertyDescriptor(handle(), name.handle(), &valueHandle));

            return property_descriptor<T>(value(valueHandle));
        }

        /// <summary>
        ///     Gets the list of all properties on the object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>An array of property names.</returns>
        std::vector<std::wstring> get_own_property_names();

        /// <summary>
        ///     Puts an object's property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="name">The ID of the property.</param>
        /// <param name="value">The new value of the property.</param>
        /// <param name="use_strict_rules">The property set should follow strict mode rules.</param>
        template<class T = value>
        void set_property(property_id name, T value, bool use_strict_rules = true)
        {
            JsValueRef valueReference;
            runtime::translate_error_code(from_native(value, &valueReference));

            runtime::translate_error_code(JsSetProperty(handle(), name.handle(), valueReference, use_strict_rules));
        }

        /// <summary>
        ///     Determines whether an object has a property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="name">The ID of the property.</param>
        /// <returns>Whether the object (or a prototype) has the property.</returns>
        bool has_property(property_id name)
        {
            bool hasProperty;
            runtime::translate_error_code(JsHasProperty(handle(), name.handle(), &hasProperty));
            return hasProperty;
        }

        /// <summary>
        ///     Deletes an object's property.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="name">The ID of the property.</param>
        /// <param name="use_strict_rules">The property set should follow strict mode rules.</param>
        /// <results>Whether the property was deleted.</results>
        template<class T = value>
        T delete_property(property_id name, bool use_strict_rules = true)
        {
            JsValueRef value;
            runtime::translate_error_code(JsDeleteProperty(handle(), name.handle(), use_strict_rules, &value));

            T returnValue;
            runtime::translate_error_code(to_native(value, &returnValue));

            return returnValue;
        }

        /// <summary>
        ///     Defines a new object's own property from a property descriptor.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="name">The ID of the property.</param>
        /// <param name="descriptor">The property descriptor.</param>
        /// <result>Whether the property was defined.</result>
        template<class T>
        bool define_property(property_id name, property_descriptor<T> descriptor)
        {
            bool value;
            runtime::translate_error_code(JsDefineProperty(handle(), name.handle(), descriptor.handle(), &value));

            return value;
        }

        /// <summary>
        ///     Retrieve the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to retrieve.</param>
        /// <returns>The retrieved value.</returns>
        template<class T = value>
        T get_index(value index)
        {
            JsValueRef value;
            runtime::translate_error_code(JsGetIndexedProperty(handle(), index.handle(), &value));

            T returnValue;
            runtime::translate_error_code(to_native(value, &returnValue));

            return returnValue;
        }

        /// <summary>
        ///     Retrieve the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to retrieve.</param>
        /// <returns>The retrieved value.</returns>
        template<class T = value>
        T get_index(int index)
        {
            JsValueRef indexValue;
            runtime::translate_error_code(JsIntToNumber(index, &indexValue));

            JsValueRef value;
            runtime::translate_error_code(JsGetIndexedProperty(handle(), indexValue, &value));

            T returnValue;
            runtime::translate_error_code(to_native(value, &returnValue));

            return returnValue;
        }

        /// <summary>
        ///     Set the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to set.</param>
        /// <param name="value">The value to set.</param>
        template<class T = value>
        void set_index(value index, T value)
        {
            JsValueRef valueReference;
            runtime::translate_error_code(from_native(value, &valueReference));

            runtime::translate_error_code(JsSetIndexedProperty(handle(), index.handle(), valueReference));
        }

        /// <summary>
        ///     Set the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to set.</param>
        /// <param name="value">The value to set.</param>
        template<class T = value>
        void set_index(int index, T value)
        {
            JsValueRef indexValue;
            runtime::translate_error_code(JsIntToNumber(index, &indexValue));

            JsValueRef valueReference;
            runtime::translate_error_code(from_native(value, &valueReference));

            runtime::translate_error_code(JsSetIndexedProperty(handle(), indexValue, valueReference));
        }

        /// <summary>
        ///     Tests whether an object has a value at the specified index.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to test.</param>
        /// <returns>Whether the object has an value at the specified index.</returns>
        bool has_index(value index)
        {
            bool hasProperty;
            runtime::translate_error_code(JsHasIndexedProperty(handle(), index.handle(), &hasProperty));
            return hasProperty;
        }

        /// <summary>
        ///     Tests whether an object has a value at the specified index.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to test.</param>
        /// <returns>Whether the object has an value at the specified index.</returns>
        bool has_index(int index)
        {
            JsValueRef indexValue;
            runtime::translate_error_code(JsIntToNumber(index, &indexValue));

            bool hasProperty;
            runtime::translate_error_code(JsHasIndexedProperty(handle(), indexValue, &hasProperty));
            return hasProperty;
        }

        /// <summary>
        ///     Delete the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to delete.</param>
        void delete_index(value index)
        {
            runtime::translate_error_code(JsDeleteIndexedProperty(handle(), index.handle()));
        }

        /// <summary>
        ///     Delete the value at the specified index of an object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="index">The index to delete.</param>
        void delete_index(int index)
        {
            JsValueRef indexValue;
            runtime::translate_error_code(JsIntToNumber(index, &indexValue));
            runtime::translate_error_code(JsDeleteIndexedProperty(handle(), indexValue));
        }

        /// <summary>
        ///     Creates a new object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The new object.</returns>
        static object create()
        {
            JsValueRef objectValue;
            runtime::translate_error_code(JsCreateObject(&objectValue));
            return object(objectValue);
        }
    };

    /// <summary>
    ///     A reference to an external JavaScript object (i.e. one that wraps a void pointer).
    /// </summary>
    class external_object : public object
    {
        friend class value;

    private:
        explicit external_object(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid object handle.
        /// </summary>
        external_object() :
            object()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to an external <c>object</c> handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit external_object(value object) :
            object(object.handle())
        {
        }

        /// <summary>
        ///     Retrieves the data from an external object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>
        ///     The external data stored in the object. Can be null if no external data is stored
        ///     in the object.
        /// </returns>
        void *data()
        {
            void *data;
            runtime::translate_error_code(JsGetExternalData(handle(), &data));
            return data;
        }

        /// <summary>
        ///     Sets the external data on an external object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="data">
        ///     The external data to be stored in the object. Can be null if no external data is 
        ///     to be stored in the object.
        /// </param>
        void set_data(void *data)
        {
            runtime::translate_error_code(JsSetExternalData(handle(), data));
        }

        /// <summary>
        ///     A finalizer callback.
        /// </summary>
        /// <param name="data">
        ///     The external data that was passed in when creating the object being finalized.
        /// </param>
        typedef void (CALLBACK *Finalize)(void *data);

        /// <summary>
        ///     Creates a new object that stores some external data.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="data">External data that the object will represent. May be null.</param>
        /// <param name="finalizeCallback">
        ///     A callback for when the object is finalized. May be null.
        /// </param>
        /// <returns>The new object.</returns>
        static external_object create(void *data = nullptr, Finalize finalizeCallback = nullptr)
        {
            JsValueRef object;
            runtime::translate_error_code(JsCreateExternalObject(data, finalizeCallback, &object));
            return external_object(object);
        }
    };

    /// <summary>
    ///     A reference to a JavaScript array.
    /// </summary>
    template<class T = value>
    class array : public object
    {
        friend class object;
        friend class value;

    private:
        explicit array(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        /// <summary>
        ///     Represents an element of a JavaScript array.
        /// </summary>
        template<class T>
        class array_element
        {
        private:
            array<T> _array;
            value _index;

        public:
            /// <summary>
            ///     Creates a new JavaScript array element handle.
            /// </summary>
            /// <param name="array">The array the element is from.</param>
            /// <param name="index">The index of the element.</param>
            array_element(array<T> array, value index) :
                _array(array), _index(index)
            {
            }

            /// <summary>
            ///     The array the element is from.
            /// </summary>
            array<T> array()
            {
                return _array;
            }

            /// <summary>
            ///     The index of the element.
            /// </summary>
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

        /// <summary>
        ///     Creates an invalid handle to an array.
        /// </summary>
        array() :
            object()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to an <c>array</c> handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit array(value object) :
            object(object.handle())
        {
        }

        array_element<T> operator [](int index)
        {
            return array_element<T>(*this, number::create(index));
        }

        /// <summary>
        ///     Gets the length of an array value.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <returns>The length of the array.</returns>
        int length()
        {
            JsPropertyIdRef lengthName;
            JsValueRef lengthValue;
            double lengthDouble;

            // CONSIDER: Caching this somewhere?
            runtime::translate_error_code(JsGetPropertyIdFromName(L"length", &lengthName));
            runtime::translate_error_code(JsGetProperty(handle(), lengthName, &lengthValue));
            runtime::translate_error_code(JsNumberToDouble(lengthValue, &lengthDouble));
            return (int) lengthDouble;
        }

        /// <summary>
        ///     Creates a JavaScript array object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="length">The initial length of the array.</param>
        /// <returns>The new array object.</returns>
        static array<T> create(unsigned int length)
        {
            JsValueRef array;
            runtime::translate_error_code(JsCreateArray(length, &array));
            return jsrt::array<T>(array);
        }

        /// <summary>
        ///     Creates a JavaScript array object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="values">The initial values of the array.</param>
        /// <returns>The new array object.</returns>
        static array<T> create(std::initializer_list<T> values)
        {
            array<T> array = create((unsigned int) values.size());
            int index = 0;
            for (auto iter = values.begin(); iter != values.end(); iter++)
            {
                array[index++] = *iter;
            }

            return array;
        }
    };

    /// <summary>
    ///     A reference to a JavaScript error.
    /// </summary>
    class error : public object
    {
        friend class value;

    private:
        static JsValueRef format_message(std::wstring message, va_list argptr)
        {
            wchar_t buffer[2048];
            _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, message.c_str(), argptr);

            JsValueRef errorString;
            runtime::translate_error_code(JsPointerToString(buffer, wcslen(buffer), &errorString));

            return errorString;
        }

    protected:
        explicit error(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid handle to an error.
        /// </summary>
        error() :
            object()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to an <c>error</c> handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit error(value object) :
            object(object.handle())
        {
        }

        /// <summary>
        ///     The <c>name</c> property of the error.
        /// </summary>
        std::wstring name()
        {
            optional<value> name = get_property<value>(property_id::create(L"name"));

            if (name.has_value() && name.value().type() == JsString)
            {
                return ((string) name.value()).data();
            }

            return L"";
        }

        /// <summary>
        ///     The <c>message</c> property of the error.
        /// </summary>
        std::wstring message()
        {
            optional<value> message = get_property<value>(property_id::create(L"message"));

            if (message.has_value() && message.value().type() == JsString)
            {
                return ((string) message.value()).data();
            }

            return L"";
        }

        /// <summary>
        ///     Creates a new JavaScript error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        static error create(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);

            JsValueRef errorObject;

            runtime::translate_error_code(JsCreateError(format_message(message, argptr), &errorObject));
            return error(errorObject);
        }

        /// <summary>
        ///     Creates a new JavaScript TypeError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        static error create_type_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);

            JsValueRef errorObject;

            runtime::translate_error_code(JsCreateTypeError(format_message(message, argptr), &errorObject));
            return error(errorObject);
        }

        /// <summary>
        ///     Creates a new JavaScript ReferenceError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        static error create_reference_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);

            JsValueRef errorObject;

            runtime::translate_error_code(JsCreateReferenceError(format_message(message, argptr), &errorObject));
            return error(errorObject);
        }

        /// <summary>
        ///     Creates a new JavaScript RangeError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        static error create_range_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);

            JsValueRef errorObject;

            runtime::translate_error_code(JsCreateRangeError(format_message(message, argptr), &errorObject));
            return error(errorObject);
        }

        /// <summary>
        ///     Creates a new JavaScript SyntaxError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        static error create_syntax_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);

            JsValueRef errorObject;

            runtime::translate_error_code(JsCreateSyntaxError(format_message(message, argptr), &errorObject));
            return error(errorObject);
        }

        /// <summary>
        ///     Creates a new JavaScript URIError error object
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        static error create_uri_error(std::wstring message, ...)
        {
            va_list argptr;
            va_start(argptr, message);

            JsValueRef errorObject;

            runtime::translate_error_code(JsCreateURIError(format_message(message, argptr), &errorObject));
            return error(errorObject);
        }
    };

    /// <summary>
    ///     Information about a function call.
    /// </summary>
    class call_info
    {
    private:
        value _callee;
        value _this_value;
        bool _is_construct_call;

    public:
        /// <summary>
        ///     Constructs an empty call information instance.
        /// </summary>
        call_info() :
            _callee(value()),
            _this_value(value()),
            _is_construct_call(false)
        {
        }

        /// <summary>
        ///     Constructs a call information instance.
        /// </summary>
        call_info(value callee, value this_value, bool is_construct_call) :
            _callee(callee),
            _this_value(this_value),
            _is_construct_call(is_construct_call)
        {
        }

        /// <summary>
        ///     The JavaScript function being called.
        /// </summary>
        value callee() const
        {
            return this->_callee;
        }

        /// <summary>
        ///     The <c>this</c> value for the function call.
        /// </summary>
        value this_value() const
        {
            return this->_this_value;
        }

        /// <summary>
        ///     Whether the call was a function call or a <c>new</c> call.
        /// </summary>
        bool is_construct_call() const
        {
            return this->_is_construct_call;
        }
    };

    /// <summary>
    ///     A reference to a JavaScript function.
    /// </summary>
    class function_base : public object
    {
        friend class context;
        friend class value;

    private:
        template<class T>
        static bool is_rest(T value)
        {
            return false;
        };

        template<class T>
        static bool is_rest(std::vector<T> value)
        {
            return true;
        };

        template<class T>
        static bool argument_from_value(int position, JsValueRef *arguments, int argument_count, T &result)
        {
            if (position > argument_count - 1)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }
            else if (to_native(arguments[position], &result) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return false;
            }

            return true;
        }

        template<class T>
        static bool argument_from_value(int position, JsValueRef *arguments, int argument_count, optional<T> &result)
        {
            if (position > argument_count - 1)
            {
                result = missing();
            }
            else
            {
                T nativeValue;
                if (to_native(arguments[position], &nativeValue) != JsNoError)
                {
                    context::set_exception(error::create_type_error(L"Could not convert value."));
                    result = missing();
                }
                else
                {
                    result = optional<T>(nativeValue);
                }
            }

            return true;
        }

        template<class T>
        static bool argument_from_value(int position, JsValueRef *arguments, int argument_count, std::vector<T> &result)
        {
            bool succeeded = true;

            if (position < argument_count)
            {
                result = std::vector<T>(argument_count - position);
                std::transform(arguments + position, arguments + argument_count, result.begin(), [&](JsValueRef v)
                {
                    T value;

                    if (to_native(v, &value) != JsNoError)
                    {
                        context::set_exception(error::create_type_error(L"Could not convert value."));
                        succeeded = true;
                    }

                    return value;
                });
            }

            return succeeded;
        }

        template<class T>
        static size_t optional_argument_count(T value)
        {
            return 1;
        };

        template<class T>
        static size_t optional_argument_count(optional<T> value)
        {
            return value.has_value() ? 1 : 0;
        };

        template<class T>
        static size_t optional_argument_count(std::vector<T> &value)
        {
            return value.size();
        };

        template<class T>
        static void fill_rest(T argument, unsigned start, std::vector<JsValueRef> &arguments)
        {
            runtime::translate_error_code(from_native(argument, &arguments[start]));
        }

        template<class T>
        static void fill_rest(std::vector<T> &rest, unsigned start, std::vector<JsValueRef> &arguments)
        {
            std::transform(rest.begin(), rest.end(), arguments.begin() + start, [&](T v) 
            {
                JsValueRef value;
                runtime::translate_error_code(from_native(v, &value));
                return value;
            });
        }

    protected:
        template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6, P7 &p7, P8 &p8)
        {
            if (!is_rest(p8) && argument_count > 9)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return argument_from_value(1, arguments, argument_count, p1) &&
                argument_from_value(2, arguments, argument_count, p2) &&
                argument_from_value(3, arguments, argument_count, p3) &&
                argument_from_value(4, arguments, argument_count, p4) &&
                argument_from_value(5, arguments, argument_count, p5) &&
                argument_from_value(6, arguments, argument_count, p6) &&
                argument_from_value(7, arguments, argument_count, p7) &&
                argument_from_value(8, arguments, argument_count, p8);
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6, P7 &p7)
        {
            if (!is_rest(p7) && argument_count > 8)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return argument_from_value(1, arguments, argument_count, p1) &&
                argument_from_value(2, arguments, argument_count, p2) &&
                argument_from_value(3, arguments, argument_count, p3) &&
                argument_from_value(4, arguments, argument_count, p4) &&
                argument_from_value(5, arguments, argument_count, p5) &&
                argument_from_value(6, arguments, argument_count, p6) &&
                argument_from_value(7, arguments, argument_count, p7);
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6>
        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6)
        {
            if (!is_rest(p6) && argument_count > 7)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return argument_from_value(1, arguments, argument_count, p1) &&
                argument_from_value(2, arguments, argument_count, p2) &&
                argument_from_value(3, arguments, argument_count, p3) &&
                argument_from_value(4, arguments, argument_count, p4) &&
                argument_from_value(5, arguments, argument_count, p5) &&
                argument_from_value(6, arguments, argument_count, p6);
        }

        template <class P1, class P2, class P3, class P4, class P5>
        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5)
        {
            if (!is_rest(p5) && argument_count > 6)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return argument_from_value(1, arguments, argument_count, p1) &&
                argument_from_value(2, arguments, argument_count, p2) &&
                argument_from_value(3, arguments, argument_count, p3) &&
                argument_from_value(4, arguments, argument_count, p4) &&
                argument_from_value(5, arguments, argument_count, p5);
        }

        template <class P1, class P2, class P3, class P4>
        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, P1 &p1, P2 &p2, P3 &p3, P4 &p4)
        {
            if (!is_rest(p4) && argument_count > 5)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return argument_from_value(1, arguments, argument_count, p1) &&
                argument_from_value(2, arguments, argument_count, p2) &&
                argument_from_value(3, arguments, argument_count, p3) &&
                argument_from_value(4, arguments, argument_count, p4);
        }

        template <class P1, class P2, class P3>
        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, P1 &p1, P2 &p2, P3 &p3)
        {
            if (!is_rest(p3) && argument_count > 4)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return argument_from_value(1, arguments, argument_count, p1) &&
                argument_from_value(2, arguments, argument_count, p2) &&
                argument_from_value(3, arguments, argument_count, p3);
        }

        template <class P1, class P2>
        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, P1 &p1, P2 &p2)
        {
            if (!is_rest(p2) && argument_count > 3)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return argument_from_value(1, arguments, argument_count, p1) &&
                argument_from_value(2, arguments, argument_count, p2);
        }

        template <class P1>
        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, P1 &p1)
        {
            if (!is_rest(p1) && argument_count > 2)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return argument_from_value(1, arguments, argument_count, p1);
        }

        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info)
        {
            if (argument_count > 1)
            {
                context::set_exception(error::create(L"Incorrect number of arguments."));
                return false;
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return true;
        }

        static bool unpack_arguments(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, call_info &info, std::vector<value> &value_arguments)
        {
            value_arguments = std::vector<value>(argument_count - 1);

            for (int index = 0; index < argument_count - 1; index++)
            {
                value_arguments[index] = value(arguments[index + 1]);
            }

            info = call_info(value(callee), value(arguments[0]), is_construct_call);

            return true;
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
        static std::vector<JsValueRef> pack_arguments(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            size_t argument_count = 1 +
                optional_argument_count(p1) +
                optional_argument_count(p2) +
                optional_argument_count(p3) +
                optional_argument_count(p4) +
                optional_argument_count(p5) +
                optional_argument_count(p6) +
                optional_argument_count(p7) +
                optional_argument_count(p8);
            std::vector<JsValueRef> arguments(argument_count);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            switch (argument_count)
            {
                default:
                    fill_rest(p8, 8, arguments);
                    // fall through
                case 8:
                    runtime::translate_error_code(from_native(p7, &arguments[7]));
                    // fall through
                case 7:
                    runtime::translate_error_code(from_native(p6, &arguments[6]));
                    // fall through
                case 6:
                    runtime::translate_error_code(from_native(p5, &arguments[5]));
                    // fall through
                case 5:
                    runtime::translate_error_code(from_native(p4, &arguments[4]));
                    // fall through
                case 4:
                    runtime::translate_error_code(from_native(p3, &arguments[3]));
                    // fall through
                case 3:
                    runtime::translate_error_code(from_native(p2, &arguments[2]));
                    // fall through
                case 2:
                    runtime::translate_error_code(from_native(p1, &arguments[1]));
                    // fall through
                case 1:
                    // Nothing to do
                    break;
            }

            return arguments;
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
        static std::vector<JsValueRef> pack_arguments(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            size_t argument_count = 1 +
                optional_argument_count(p1) +
                optional_argument_count(p2) +
                optional_argument_count(p3) +
                optional_argument_count(p4) +
                optional_argument_count(p5) +
                optional_argument_count(p6) +
                optional_argument_count(p7);
            std::vector<JsValueRef> arguments(argument_count);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            switch (argument_count)
            {
                default:
                    fill_rest(p7, 7, arguments);
                    // fall through
                case 7:
                    runtime::translate_error_code(from_native(p6, &arguments[6]));
                    // fall through
                case 6:
                    runtime::translate_error_code(from_native(p5, &arguments[5]));
                    // fall through
                case 5:
                    runtime::translate_error_code(from_native(p4, &arguments[4]));
                    // fall through
                case 4:
                    runtime::translate_error_code(from_native(p3, &arguments[3]));
                    // fall through
                case 3:
                    runtime::translate_error_code(from_native(p2, &arguments[2]));
                    // fall through
                case 2:
                    runtime::translate_error_code(from_native(p1, &arguments[1]));
                    // fall through
                case 1:
                    // Nothing to do
                    break;
            }

            return arguments;
        }

        template <class P1, class P2, class P3, class P4, class P5, class P6>
        static std::vector<JsValueRef> pack_arguments(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            size_t argument_count = 1 +
                optional_argument_count(p1) +
                optional_argument_count(p2) +
                optional_argument_count(p3) +
                optional_argument_count(p4) +
                optional_argument_count(p5) +
                optional_argument_count(p6);
            std::vector<JsValueRef> arguments(argument_count);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            switch (argument_count)
            {
                default:
                    fill_rest(p6, 6, arguments);
                    // fall through
                case 6:
                    runtime::translate_error_code(from_native(p5, &arguments[5]));
                    // fall through
                case 5:
                    runtime::translate_error_code(from_native(p4, &arguments[4]));
                    // fall through
                case 4:
                    runtime::translate_error_code(from_native(p3, &arguments[3]));
                    // fall through
                case 3:
                    runtime::translate_error_code(from_native(p2, &arguments[2]));
                    // fall through
                case 2:
                    runtime::translate_error_code(from_native(p1, &arguments[1]));
                    // fall through
                case 1:
                    // Nothing to do
                    break;
            }

            return arguments;
        }

        template <class P1, class P2, class P3, class P4, class P5>
        static std::vector<JsValueRef> pack_arguments(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            size_t argument_count = 1 +
                optional_argument_count(p1) +
                optional_argument_count(p2) +
                optional_argument_count(p3) +
                optional_argument_count(p4) +
                optional_argument_count(p5);
            std::vector<JsValueRef> arguments(argument_count);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            switch (argument_count)
            {
                default:
                    fill_rest(p5, 5, arguments);
                    // fall through
                case 5:
                    runtime::translate_error_code(from_native(p4, &arguments[4]));
                    // fall through
                case 4:
                    runtime::translate_error_code(from_native(p3, &arguments[3]));
                    // fall through
                case 3:
                    runtime::translate_error_code(from_native(p2, &arguments[2]));
                    // fall through
                case 2:
                    runtime::translate_error_code(from_native(p1, &arguments[1]));
                    // fall through
                case 1:
                    // Nothing to do
                    break;
            }

            return arguments;
        }

        template <class P1, class P2, class P3, class P4>
        static std::vector<JsValueRef> pack_arguments(value this_value, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            size_t argument_count = 1 +
                optional_argument_count(p1) +
                optional_argument_count(p2) +
                optional_argument_count(p3) +
                optional_argument_count(p4);
            std::vector<JsValueRef> arguments(argument_count);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            switch (argument_count)
            {
                default:
                    fill_rest(p4, 4, arguments);
                    // fall through
                case 4:
                    runtime::translate_error_code(from_native(p3, &arguments[3]));
                    // fall through
                case 3:
                    runtime::translate_error_code(from_native(p2, &arguments[2]));
                    // fall through
                case 2:
                    runtime::translate_error_code(from_native(p1, &arguments[1]));
                    // fall through
                case 1:
                    // Nothing to do
                    break;
            }

            return arguments;
        }

        template <class P1, class P2, class P3>
        static std::vector<JsValueRef> pack_arguments(value this_value, P1 p1, P2 p2, P3 p3)
        {
            size_t argument_count = 1 +
                optional_argument_count(p1) +
                optional_argument_count(p2) +
                optional_argument_count(p3);
            std::vector<JsValueRef> arguments(argument_count);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            switch (argument_count)
            {
                default:
                    fill_rest(p3, 3, arguments);
                    // fall through
                case 3:
                    runtime::translate_error_code(from_native(p2, &arguments[2]));
                    // fall through
                case 2:
                    runtime::translate_error_code(from_native(p1, &arguments[1]));
                    // fall through
                case 1:
                    // Nothing to do
                    break;
            }

            return arguments;
        }

        template <class P1, class P2>
        static std::vector<JsValueRef> pack_arguments(value this_value, P1 p1, P2 p2)
        {
            size_t argument_count = 1 +
                optional_argument_count(p1) +
                optional_argument_count(p2);
            std::vector<JsValueRef> arguments(argument_count);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            switch (argument_count)
            {
                default:
                    fill_rest(p2, 2, arguments);
                    // fall through
                case 2:
                    runtime::translate_error_code(from_native(p1, &arguments[1]));
                    // fall through
                case 1:
                    // Nothing to do
                    break;
            }

            return arguments;
        }

        template <class P1>
        static std::vector<JsValueRef> pack_arguments(value this_value, P1 p1)
        {
            size_t argument_count = 1 +
                optional_argument_count(p1);
            std::vector<JsValueRef> arguments(argument_count);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            switch (argument_count)
            {
                default:
                    fill_rest(p1, 1, arguments);
                    // fall through
                case 1:
                    // Nothing to do
                    break;
            }

            return arguments;
        }

        static std::vector<JsValueRef> pack_arguments(value this_value)
        {
            std::vector<JsValueRef> arguments(1);

            if (this_value.is_valid())
            {
                arguments[0] = this_value.handle();
            }
            else
            {
                // TODO: Why do we have to do this?
                runtime::translate_error_code(JsGetUndefinedValue(&arguments[0]));
            }

            return arguments;
        }

        static std::vector<JsValueRef> pack_arguments(value this_value, std::initializer_list<value> arguments)
        {
            std::vector<JsValueRef> call_args(arguments.size() + 1);
            call_args[0] = this_value.handle();
            unsigned int index = 1;
            for (const value argument : arguments)
            {
                call_args[index++] = argument.handle();
            }

            return call_args;
        }

        template <class R>
        R call_function(std::vector<JsValueRef> &arguments)
        {
            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), (unsigned short)arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

        template <>
        void call_function(std::vector<JsValueRef> &arguments)
        {
            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), arguments.data(), (unsigned short)arguments.size(), &resultValue));
        }

        explicit function_base(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        /// <summary>
        ///     The signature of a function callback.
        /// </summary>
        /// <param name="call_info">Information about the call.</param>
        /// <param name="arguments">Arguments to the call.</param>
        /// <returns>The result of the call.</returns>
        typedef value (*Signature)(const call_info &call_info, const std::vector<value> &arguments);

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            std::vector<value> argument_vector;
            call_info info;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, argument_vector))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            value result = value();
            try
            {
                result = callback(info, argument_vector);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        /// <summary>
        ///     Creates an invalid handle to a function.
        /// </summary>
        function_base() :
            object()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to a <c>function</c> handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit function_base(value object) :
            object(object.handle())
        {
        }

        /// <summary>
        ///     Calls the JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="this_value">The value of <c>this</c> for the call.</param>
        /// <param name="arguments">Arguments to the call.</param>
        /// <returns>The result of the call.</returns>
        value operator()(value this_value, std::initializer_list<value> arguments)
        {
            std::vector<JsValueRef> call_arguments = pack_arguments(this_value, arguments);

            JsValueRef resultValue;
            runtime::translate_error_code(JsCallFunction(handle(), (JsValueRef *) call_arguments.data(), (unsigned short) call_arguments.size(), &resultValue));
            return value(resultValue);
        }

        /// <summary>
        ///     Constructs a JavaScript object.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="arguments">Arguments to the constructor call.</param>
        /// <returns>The result of the constructor call.</returns>
        value construct(std::initializer_list<value> arguments)
        {
            std::vector<JsValueRef> call_arguments = pack_arguments(value(), arguments);

            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), (JsValueRef *) call_arguments.data(), (unsigned short) call_arguments.size(), &resultValue));
            return value(resultValue);
        }

        /// <summary>
        ///     Creates a new JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="signature">The method to call when the function is invoked.</param>
        /// <returns>The new function object.</returns>
        static function_base create(Signature signature)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, signature, &ref));
            return function_base(ref);
        }

        /// <summary>
        ///     Creates a new JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="signature">The method to call when the function is invoked.</param>
        /// <returns>The new function object.</returns>
        template<class R, class... Parameters>
        static function<R, Parameters...> create(R(*signature)(const jsrt::call_info &info, Parameters...))
        {
            return function<R, Parameters...>::create(signature);
        }

        /// <summary>
        ///     Creates a new JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="signature">The method to call when the function is invoked.</param>
        /// <returns>The new function object.</returns>
        template<class R>
        static function<R> create(R(*signature)(const jsrt::call_info &info))
        {
            return function<R>::create(signature);
        }
    };

    /// <summary>
    ///     A reference to a JavaScript function that creates an object.
    /// </summary>
    template<class R>
    class constructor_function : public function_base
    {
        friend class value;

    protected:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(JsValueRef ref) :
            function_base(ref)
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }

        R construct_object(std::vector<JsValueRef> &arguments)
        {
            JsValueRef resultValue;
            runtime::translate_error_code(JsConstructObject(handle(), arguments.data(), (unsigned short)arguments.size(), &resultValue));

            R result;
            runtime::translate_error_code(to_native(resultValue, &result));
            return result;
        }

    public:
        /// <summary>
        ///     The object that will be the prototype of objects created by this function.
        /// </summary>
        typename object constructor_prototype()
        {
            return get_property<object>(property_id::create(L"prototype"));
        }

        /// <summary>
        ///     Sets the object that will be the prototype of objects created by this function.
        /// </summary>
        /// <param name="prototype">The prototype object.</param>
        void set_constructor_prototype(typename object prototype)
        {
            return set_property<object>(property_id::create(L"prototype"), prototype);
        }
    };

    // An instantiation of constructor_function that disallows constructing non-object values.
    template<>
    class constructor_function<value> : public function_base
    {
        friend class value;

    protected:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(JsValueRef ref) :
            function_base(ref)
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }
    };

    // An instantiation of constructor_function that disallows constructing non-object values.
    template<>
    class constructor_function<std::wstring> : public function_base
    {
        friend class value;

    protected:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(JsValueRef ref) :
            function_base(ref)
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }
    };

    // An instantiation of constructor_function that disallows constructing non-object values.
    template<>
    class constructor_function<double> : public function_base
    {
        friend class value;

    protected:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(JsValueRef ref) :
            function_base(ref)
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }
    };

    // An instantiation of constructor_function that disallows constructing non-object values.
    template<>
    class constructor_function<bool> : public function_base
    {
        friend class value;

    protected:
        constructor_function() :
            function_base()
        {
        }

        explicit constructor_function(JsValueRef ref) :
            function_base(ref)
        {
        }

        explicit constructor_function(value object) :
            function_base(object)
        {
        }
    };

#pragma region Arity 8
    template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    class function : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, P1, P2, P3, P4, P5, P6, P7, P8>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8);

        function<R, P1, P2, P3, P4, P5, P6, P7, P8>() :
            constructor_function<R>()
        {
        }

        explicit function<R, P1, P2, P3, P4, P5, P6, P7, P8>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;
            P7 p7;
            P8 p8;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4, p5, p6, p7, p8))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info, p1, p2, p3, p4, p5, p6, p7, p8);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            return call_function<R>(pack_arguments(this_value, p1, p2, p3, p4, p5, p6, p7, p8));
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            return construct_object(pack_arguments(value(), p1, p2, p3, p4, p5, p6, p7, p8));
        }

        static function<R, P1, P2, P3, P4, P5, P6, P7, P8> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    class function<void, P1, P2, P3, P4, P5, P6, P7, P8> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, P1, P2, P3, P4, P5, P6, P7, P8>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8);

        function<void, P1, P2, P3, P4, P5, P6, P7, P8>() :
            function_base()
        {
        }

        explicit function<void, P1, P2, P3, P4, P5, P6, P7, P8>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;
            P7 p7;
            P8 p8;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4, p5, p6, p7, p8))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info, p1, p2, p3, p4, p5, p6, p7, p8);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            call_function<void>(pack_arguments(this_value, p1, p2, p3, p4, p5, p6, p7, p8));
        }

        static function<void, P1, P2, P3, P4, P5, P6, P7, P8> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

#pragma region Arity 7
    template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    class function<R, P1, P2, P3, P4, P5, P6, P7, notdefined> : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, P1, P2, P3, P4, P5, P6, P7, notdefined>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7);

        function<R, P1, P2, P3, P4, P5, P6, P7, notdefined>() :
            constructor_function<R>()
        {
        }

        explicit function<R, P1, P2, P3, P4, P5, P6, P7, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;
            P7 p7;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4, p5, p6, p7))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info, p1, p2, p3, p4, p5, p6, p7);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            return call_function<R>(pack_arguments(this_value, p1, p2, p3, p4, p5, p6, p7));
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            return construct_object(pack_arguments(value(), p1, p2, p3, p4, p5, p6, p7));
        }

        static function<R, P1, P2, P3, P4, P5, P6, P7> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    class function<void, P1, P2, P3, P4, P5, P6, P7, notdefined> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, P1, P2, P3, P4, P5, P6, P7, notdefined>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7);

        function<void, P1, P2, P3, P4, P5, P6, P7, notdefined>() :
            function_base()
        {
        }

        explicit function<void, P1, P2, P3, P4, P5, P6, P7, notdefined>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;
            P7 p7;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4, p5, p6, p7))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info, p1, p2, p3, p4, p5, p6, p7);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            call_function<void>(pack_arguments(this_value, p1, p2, p3, p4, p5, p6, p7));
        }

        static function<void, P1, P2, P3, P4, P5, P6, P7> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

#pragma region Arity 6
    template<class R, class P1, class P2, class P3, class P4, class P5, class P6>
    class function<R, P1, P2, P3, P4, P5, P6, notdefined, notdefined> : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, P1, P2, P3, P4, P5, P6, notdefined, notdefined>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6);

        function<R, P1, P2, P3, P4, P5, P6, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        explicit function<R, P1, P2, P3, P4, P5, P6, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4, p5, p6))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info, p1, p2, p3, p4, p5, p6);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            return call_function<R>(pack_arguments(this_value, p1, p2, p3, p4, p5, p6));
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            return construct_object(pack_arguments(value(), p1, p2, p3, p4, p5, p6));
        }

        static function<R, P1, P2, P3, P4, P5, P6> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<class P1, class P2, class P3, class P4, class P5, class P6>
    class function<void, P1, P2, P3, P4, P5, P6, notdefined, notdefined> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, P1, P2, P3, P4, P5, P6, notdefined, notdefined>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6);

        function<void, P1, P2, P3, P4, P5, P6, notdefined, notdefined>() :
            function_base()
        {
        }

        explicit function<void, P1, P2, P3, P4, P5, P6, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;
            P6 p6;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4, p5, p6))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info, p1, p2, p3, p4, p5, p6);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            call_function<void>(pack_arguments(this_value, p1, p2, p3, p4, p5, p6));
        }

        static function<void, P1, P2, P3, P4, P5, P6> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

#pragma region Arity 5
    template<class R, class P1, class P2, class P3, class P4, class P5>
    class function<R, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

        function<R, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        explicit function<R, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4, p5))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info, p1, p2, p3, p4, p5);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            return call_function<R>(pack_arguments(this_value, p1, p2, p3, p4, p5));
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            return construct_object(pack_arguments(value(), p1, p2, p3, p4, p5));
        }

        static function<R, P1, P2, P3, P4, P5> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<class P1, class P2, class P3, class P4, class P5>
    class function<void, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

        function<void, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        explicit function<void, P1, P2, P3, P4, P5, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;
            P5 p5;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4, p5))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info, p1, p2, p3, p4, p5);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            call_function<void>(pack_arguments(this_value, p1, p2, p3, p4, p5));
        }

        static function<void, P1, P2, P3, P4, P5> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

#pragma region Arity 4
    template<class R, class P1, class P2, class P3, class P4>
    class function<R, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4);

        function<R, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        explicit function<R, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info, p1, p2, p3, p4);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            return call_function<R>(pack_arguments(this_value, p1, p2, p3, p4));
        }

        R construct(P1 p1, P2 p2, P3 p3, P4 p4)
        {
            return construct_object(pack_arguments(value(), p1, p2, p3, p4));
        }

        static function<R, P1, P2, P3, P4> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<class P1, class P2, class P3, class P4>
    class function<void, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3, P4 p4);

        function<void, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        explicit function<void, P1, P2, P3, P4, notdefined, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;
            P4 p4;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3, p4))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info, p1, p2, p3, p4);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            call_function<void>(pack_arguments(this_value, p1, p2, p3, p4));
        }

        static function<void, P1, P2, P3, P4> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

#pragma region Arity 3
    template<class R, class P1, class P2, class P3>
    class function<R, P1, P2, P3, notdefined, notdefined, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, P1, P2, P3, notdefined, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3);

        function<R, P1, P2, P3, notdefined, notdefined, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        explicit function<R, P1, P2, P3, notdefined, notdefined, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info, p1, p2, p3);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value, P1 p1, P2 p2, P3 p3)
        {
            return call_function<R>(pack_arguments(this_value, p1, p2, p3));
        }

        R construct(P1 p1, P2 p2, P3 p3)
        {
            return construct_object(pack_arguments(value(), p1, p2, p3));
        }

        static function<R, P1, P2, P3> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<class P1, class P2, class P3>
    class function<void, P1, P2, P3, notdefined, notdefined, notdefined, notdefined, notdefined> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, P1, P2, P3, notdefined, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info, P1 p1, P2 p2, P3 p3);

        function<void, P1, P2, P3, notdefined, notdefined, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        explicit function<void, P1, P2, P3, notdefined, notdefined, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;
            P1 p1;
            P2 p2;
            P3 p3;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2, p3))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info, p1, p2, p3);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value, P1 p1, P2 p2, P3 p3)
        {
            call_function<void>(pack_arguments(this_value, p1, p2, p3));
        }

        static function<void, P1, P2, P3> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

#pragma region Arity 2
    template<class R, class P1, class P2>
    class function<R, P1, P2, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, P1, P2, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info, P1 p1, P2 p2);

        function<R, P1, P2, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        explicit function<R, P1, P2, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;
            P1 p1;
            P2 p2;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info, p1, p2);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value, P1 p1, P2 p2)
        {
            return call_function<R>(pack_arguments(this_value, p1, p2));
        }

        R construct(P1 p1, P2 p2)
        {
            return construct_object(pack_arguments(value(), p1, p2));
        }

        static function<R, P1, P2> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<class P1, class P2>
    class function<void, P1, P2, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, P1, P2, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info, P1 p1, P2 p2);

        function<void, P1, P2, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        explicit function<void, P1, P2, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;
            P1 p1;
            P2 p2;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1, p2))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info, p1, p2);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value, P1 p1, P2 p2)
        {
            call_function<void>(pack_arguments(this_value, p1, p2));
        }

        static function<void, P1, P2> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

#pragma region Arity 1
    template<class R, class P1>
    class function<R, P1, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, P1, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info, P1 p1);

        function<R, P1, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        explicit function<R, P1, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;
            P1 p1;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info, p1);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value, P1 p1)
        {
            return call_function<R>(pack_arguments(this_value, p1));
        }

        R construct(P1 p1)
        {
            return construct_object(pack_arguments(value(), p1));
        }

        static function<R, P1> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<class P1>
    class function<void, P1, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, P1, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info, P1 p1);

        function<void, P1, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        explicit function<void, P1, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;
            P1 p1;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info, p1))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info, p1);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value, P1 p1)
        {
            call_function<void>(pack_arguments(this_value, p1));
        }

        static function<void, P1> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

#pragma region Arity 0
    template<class R>
    class function<R, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined> : public constructor_function<R>
    {
        friend class value;

    protected:
        explicit function<R, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            constructor_function<R>(ref)
        {
        }

    public:
        typedef R(*Signature)(const call_info &call_info);

        function<R, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>() :
            constructor_function<R>()
        {
        }

        explicit function<R, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(value object) :
            constructor_function<R>(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            call_info info;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            R result = R();
            try
            {
                result = callback(info);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            JsValueRef resultValue;
            if (from_native(result, &resultValue) != JsNoError)
            {
                context::set_exception(error::create_type_error(L"Could not convert value."));
                return JS_INVALID_REFERENCE;
            }

            return resultValue;
        }

    public:
        R operator ()(value this_value)
        {
            return call_function<R>(pack_arguments(this_value));
        }

        R construct()
        {
            return construct_object(pack_arguments(value()));
        }

        static function<R> create(Signature function)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function, &ref));
            return decltype(create(nullptr))(ref);
        }
    };

    template<>
    class function<void, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined> : public function_base
    {
        friend class value;

    protected:
        explicit function<void, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(JsValueRef ref) :
            function_base(ref)
        {
        }

    public:
        typedef void (*Signature)(const call_info &call_info);

        function<void, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>() :
            function_base()
        {
        }

        explicit function<void, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined, notdefined>(value object) :
            function_base(object)
        {
        }

    protected:
        static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
        {
            if (is_construct_call)
            {
                context::set_exception(error::create(L"Cannot call function as a constructor."));
                return JS_INVALID_REFERENCE;
            }

            call_info info;

            if (!unpack_arguments(callee, is_construct_call, arguments, argument_count, info))
            {
                return JS_INVALID_REFERENCE;
            }

            Signature callback = (Signature) callback_state;
            try
            {
                callback(info);
            }
            catch (...)
            {
                context::set_exception(error::create(L"Fatal error."));
                return JS_INVALID_REFERENCE;
            }

            return JS_INVALID_REFERENCE;
        }

    public:
        void operator ()(value this_value)
        {
            call_function<void>(pack_arguments(this_value));
        }

        static function<void> create(Signature function_signature)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function_signature, &ref));
            return decltype(create(nullptr))(ref);
        }
    };
#pragma endregion

    /// <summary>
    ///     A JavaScript function that is bound to a particular <c>this</c> value.
    /// </summary>
    template<class TThis, class R, class... Parameters>
    class bound_function : public function<R, Parameters...>
    {
        friend class value;

    private:
        TThis _this_value;

        explicit bound_function<TThis, R, Parameters...>(TThis this_value, JsValueRef ref) :
            function<R, Parameters...>(ref),
            _this_value(this_value)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid handle to a bound function.
        /// </summary>
        bound_function<TThis, R, Parameters...>() :
            function<R, Parameters...>()
        {
        }

        /// <summary>
        ///     Creates a value handle from a <c>this</c> value and a function.
        /// </summary>
        /// <param name="this_value">
        ///     The value the <c>this</c> parameter should be bound to.
        /// </param>
        /// <param name="func">A reference to the function.</param>
        bound_function<TThis, R, Parameters...>(TThis this_value, function<R, Parameters...> func) :
            function<R, Parameters...>(func),
            _this_value(this_value)
        {
        }

        /// <summary>
        ///     Calls the bound JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="arguments">Arguments to the call.</param>
        /// <returns>The result of the call.</returns>
        R operator()(Parameters... arguments)
        {
            return call_function<R>(pack_arguments(_this_value, arguments...));
        }

        /// <summary>
        ///     Creates a new bound JavaScript function.
        /// </summary>
        /// <remarks>
        ///     Requires an active script context.
        /// </remarks>
        /// <param name="this_value">
        ///     The value of <c>this</c> for all calls to this function.
        /// </param>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <returns>The new function object.</returns>
        static bound_function<TThis, R, Parameters...> create(TThis this_value, Signature function_signature)
        {
            JsValueRef ref;
            runtime::translate_error_code(JsCreateFunction(thunk, function_signature, &ref));
            return decltype(create(TThis(), nullptr))(this_value, ref);
        }
    };

    /// <summary>
    ///     A reference to a JavaScript property descriptor.
    /// </summary>
    template<class T>
    class property_descriptor : public object
    {
        friend class value;

    private:
        explicit property_descriptor(JsValueRef ref) :
            object(ref)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid handle to a property descriptor.
        /// </summary>
        property_descriptor() :
            object()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to a property descriptor handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit property_descriptor(value object) :
            object(object.handle())
        {
        }

        /// <summary>
        ///     Returns the <c>writable</c> property of the descriptor.
        /// </summary>
        bool writable()
        {
            return get_property<bool>(property_id::create(L"writable"));
        }

        /// <summary>
        ///     Sets the <c>writable</c> property of the descriptor.
        /// </summary>
        void set_writable(bool value)
        {
            set_property<bool>(property_id::create(L"writable"), value);
        }

        /// <summary>
        ///     Returns the <c>enumerable</c> property of the descriptor.
        /// </summary>
        bool enumerable()
        {
            return get_property<bool>(property_id::create(L"enumerable"));
        }

        /// <summary>
        ///     Sets the <c>enumerable</c> property of the descriptor.
        /// </summary>
        void set_enumerable(bool value)
        {
            set_property(property_id::create(L"enumerable"), value);
        }

        /// <summary>
        ///     Returns the <c>configurable</c> property of the descriptor.
        /// </summary>
        bool configurable()
        {
            return get_property<bool>(property_id::create(L"configurable"));
        }

        /// <summary>
        ///     Sets the <c>enumerable</c> property of the descriptor.
        /// </summary>
        void set_configurable(bool value)
        {
            set_property(property_id::create(L"configurable"), value);
        }

        /// <summary>
        ///     Returns the <c>value</c> property of the descriptor.
        /// </summary>
        T value()
        {
            return get_property<T>(property_id::create(L"value"));
        }

        /// <summary>
        ///     Sets the <c>value</c> property of the descriptor.
        /// </summary>
        void set_value(T value)
        {
            set_property(property_id::create(L"value"), value);
        }

        /// <summary>
        ///     Returns the <c>get</c> property of the descriptor.
        /// </summary>
        function<T> getter()
        {
            return get_property<function<T>>(property_id::create(L"get"));
        }

        /// <summary>
        ///     Sets the <c>get</c> property of the descriptor.
        /// </summary>
        void set_getter(function<T> value)
        {
            set_property(property_id::create(L"get"), value);
        }

        /// <summary>
        ///     Returns the <c>set</c> property of the descriptor.
        /// </summary>
        function<void, T> setter()
        {
            return get_property<function<void, T>>(property_id::create(L"set"));
        }

        /// <summary>
        ///     Sets the <c>set</c> property of the descriptor.
        /// </summary>
        void set_setter(function<void, T> value)
        {
            set_property(property_id::create(L"set"), value);
        }

        /// <summary>
        ///     Creates a new property descriptor.
        /// </summary>
        /// <returns>The new property descriptor.</returns>
        static property_descriptor<T> create()
        {
            return (property_descriptor<T>)object::create();
        }

        /// <summary>
        ///     Creates a new property descriptor.
        /// </summary>
        /// <param name="getter">The property getter.</param>
        /// <param name="setter">The property setter.</param>
        /// <returns>The new property descriptor.</returns>
        static property_descriptor<T> create(function<T> getter, function<void, T> setter)
        {
            property_descriptor<T> desc = (property_descriptor<T>)object::create();
            desc.set_getter(getter);
            desc.set_setter(setter);
            return desc;
        }
    };

    /// <summary>
    ///     An exception used to indicate failure of a JsRT call.
    /// </summary>
    class exception
    {
    protected:
        exception() { }
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
        /// <summary>
        ///     Creates a script exception with the specified error.
        /// </summary>
        /// <param name="error">The script error.</param>
        script_exception(value error) :
            _error(error)
        {
        }

        /// <summary>
        ///     The error object.
        /// </summary>
        value error() const
        {
            return _error;
        }
    };

    /// <summary>
    ///     A JavaScript error object representing a compilation error.
    /// </summary>
    class compile_error : public error
    {
        friend class runtime;
        friend class value;

    private:
        explicit compile_error(JsValueRef ref) :
            error(ref)
        {
        }

    public:
        /// <summary>
        ///     Creates an invalid handle.
        /// </summary>
        compile_error() :
            error()
        {
        }

        /// <summary>
        ///     Converts the <c>value</c> handle to a compile error handle.
        /// </summary>
        /// <remarks>
        ///     The type of the underlying value is not checked.
        /// </remarks>
        explicit compile_error(value object) :
            error(object.handle())
        {
        }

        /// <summary>
        ///     The <c>message</c> property of the error.
        /// </summary>
        std::wstring message()
        {
            return get_property<std::wstring>(property_id::create(L"message"));
        }

        /// <summary>
        ///     The <c>line</c> property of the error.
        /// </summary>
        double line()
        {
            return get_property<double>(property_id::create(L"line"));
        }

        /// <summary>
        ///     The <c>column</c> property of the error.
        /// </summary>
        double column()
        {
            return get_property<double>(property_id::create(L"column"));
        }

        /// <summary>
        ///     The <c>length</c> property of the error.
        /// </summary>
        double length()
        {
            return get_property<double>(property_id::create(L"length"));
        }

        /// <summary>
        ///     The <c>source</c> property of the error.
        /// </summary>
        std::wstring source()
        {
            return get_property<std::wstring>(property_id::create(L"source"));
        }
    };

    class script_compile_exception : public exception
    {
    private:
        compile_error _error;

    public:
        /// <summary>
        ///     Creates a compile exception with the specified error.
        /// </summary>
        /// <param name="error">The compile error.</param>
        script_compile_exception(compile_error error) :
            _error(error)
        {
        }

        /// <summary>
        ///     The compile error.
        /// </summary>
        compile_error error() const
        {
            return _error;
        }
    };
}
