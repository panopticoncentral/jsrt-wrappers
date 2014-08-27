// Copyright 2014 Adrian Panella
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
/// \brief Wrappers to create a JS proxy of a native C++ object

#pragma once

#include <memory>
#include <jsrt.h>
#include "jsrt-wrappers.h"


namespace jsrt
{



	/// <summary>
	///     Helper class to proxy javascript object with C++ Object.
	/// </summary>
	template<class O>
	class proxy_object : public external_object
	{
	private:
		/// <summary>
		///     The signature of a member-function callback.
		/// </summary>
		/// <remarks>
		/// <param name="arguments">Arguments to the call will be deduced from the rest of the "P" arguments of template.</param>
		/// <returns>The result of the call.</returns>
		template<typename R, typename... P> using Signature = R(O::*)(P...);

		/// <summary>
		///     Internal base class to member-callback wrapper object, to hava common base in templated version
		/// </summary>
		class callback_info_base {
		public:
			virtual ~callback_info_base(){}
		};

		/// <summary>
		///     Internal templated class to member-callback wrapper object.
		/// </summary>
		/// <remarks>
		///     Used to proxy call to member functions. 
		///     This is required as a pointer-to-member cannot be converted to void * and passed as data for callback-state 
		/// </remarks>
		template<class R, class... P>
		class callback_info : public callback_info_base {
		public:
			O* self;
			Signature<R, P...> member;

			callback_info(O &obj, Signature<R, P...>mem ) : 
				self(&obj), member(mem) {}

			R operator()(P... p) {
				return (self->*member)(p...);
			}
		};

		/// <summary>
		///     Reference to proxied C++ obj.
		/// </summary>
		O& cppObj;

		/// <summary>
		///     Cache of member-callback wrappers object for each bound property/method.
		/// </summary>
		/// <remarks>
		///     Callaback_info objects cannot be made in the stack, as they are pointed by JS objects 
		///     and they need to exist when a callback is called.
		///		Made of unique_ptr so it is automatically destroyed with the proxy_object
		/// </remarks>
		std::vector<std::unique_ptr<callback_info_base>> callbacks;

	public:

		/// <summary>
		///     Main constructor.
		/// </summary>
		/// <remarks>
		///     Initializes from C++ object, to create new jsObject.
		///     To make the created object usefull, calls to the correspondig bind... functions should be made
		/// </remarks>
		proxy_object(O &obj) :
			cppObj(obj), external_object(external_object::create(&obj, nullptr)) {};

		~proxy_object() {};


		/// <summary>
		///     Binds native data member to js object's property.
		/// </summary>
		template<class T>
		void bindProperty(const std::wstring &propName, T *data, 
			bool isWritable = false, bool isEnumerable = true, bool isConfigurable = false) 
		{
			property_descriptor<value> descriptor = property_descriptor<value>::create();
			property_id propId = property_id::create(propName);
			descriptor.set_configurable(isConfigurable);
			descriptor.set_enumerable(isEnumerable);
			JsValueRef getter;
			runtime::translate_error_code(JsCreateFunction(thunkGet<T>, data, &getter));
			descriptor.set_property(property_id::create(L"get"), value(getter));
			if (isWritable) {
				JsValueRef setter;
				runtime::translate_error_code(JsCreateFunction( thunkSet<T>, data, &setter));
				descriptor.set_property(property_id::create(L"set"), value(setter));
			}
			define_property(propId, descriptor);
		}

		/// <summary>
		///     Binds getter/setter methods to js object's property.
		/// </summary>
		template<class T>
		void bindAccesor(const std::wstring &propName, Signature<T>getter, Signature<void,T>setter
			, bool isEnumerable = true, bool isConfigurable = false){
			property_descriptor<value> descriptor = property_descriptor<value>::create();
			property_id propId = property_id::create(propName);
			descriptor.set_configurable(isConfigurable);
			descriptor.set_enumerable(isEnumerable);

			if (getter != nullptr) descriptor.set_property(property_id::create(L"get"), createCallback<T>(getter));
			if (setter != nullptr) descriptor.set_property(property_id::create(L"set"), createCallback<void, T>(setter));
			define_property(propId, descriptor);
		}

		/// <summary>
		///     Binds native member function to js object method. Overload without call_info
		/// </summary>
		template<class R, class... P>
		void bindMethod(const std::wstring &propName, Signature<R, P...>member,
			bool isWritable = false, bool isEnumerable = true, bool isConfigurable = false) {

			value callBack = createCallback(member);
			property_descriptor<value> descriptor = property_descriptor<value>::create();
			property_id propId = property_id::create(propName);
			descriptor.set_configurable(isConfigurable);
			descriptor.set_enumerable(isEnumerable);
			descriptor.set_writable(isWritable);
			descriptor.set_value(callBack);
			define_property(propId, descriptor);
		}

	private:



		/// <summary>
		///     General proxy callbacks passed to JS for property getter/setter.
		/// </summary>
		template<class T>
		static JsValueRef CALLBACK thunkGet(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
		{
			T* prop=static_cast<T*>(callback_state);

			JsValueRef resultValue;
			auto errCode = from_native(*prop, &resultValue);
			if (errCode == JsErrorInExceptionState) { //don't throw exception if already in exception
				return JS_INVALID_REFERENCE;
			}
			else if (errCode != JsNoError) {
				context::set_exception(error::create_type_error(L"Could not convert value."));
				return JS_INVALID_REFERENCE;
			}

			return resultValue;
		}

		template<class T>
		static JsValueRef CALLBACK thunkSet(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
		{
			if (argument_count!=2)
				throw jsrt::invalid_argument_exception();

			T* prop = static_cast<T*>(callback_state);
			if (to_native(arguments[1], prop) != JsNoError) {
				context::set_exception(error::create_type_error(L"Could not convert value."));
				return JS_INVALID_REFERENCE;
			}
			return JS_INVALID_REFERENCE;
		}


		/// <summary>
		///     Callback wrapper. 
		/// </summary>
		template<class R, class... P>
		value createCallback(Signature<R, P...>member)
		{
			JsValueRef ref;
			callbacks.push_back(std::make_unique<callback_info<R, P...>>(this->cppObj, member));
			callback_info<R, P...> *cb = (callback_info<R, P...> *)callbacks.back().get();

			runtime::translate_error_code(JsCreateFunction(thunk<R, P...>, cb, &ref));
			return value(ref);
		}


		/// <summary>
		///     Helpers to convert and validate JS arguments to templated arguments of corresponding member function.
		/// </summary>
		/// <remarks>
		///     It will generate compilation error if the member functions expects a parameter by reference/pointer.
		///     To have a reference to the js object the parameter should be of object/value/.. type, which are implicit references. 
		/// </remarks>
		template<class T>
		static T unpack(JsValueRef *arguments, int &argItem, int argument_count, int &templateItem, const int maxTemplate, int increment) {
			
			T retValue {}; //will generate compilation error if T is a reference

			//if we are done with the arguments and this is not optional, throw exception
			if (increment >0 && argItem >= argument_count  && !std::is_base_of<optional_base, T>::value)
				throw jsrt::invalid_argument_exception();
			else if (increment <0 && templateItem  + 1 > argument_count - 1 && !std::is_base_of<optional_base, T>::value)
				throw jsrt::invalid_argument_exception();

			//if this is last parameter but there are more arguments or it is a last "rest" parameter
			else if ((increment>0 && templateItem == maxTemplate - 1 && (argItem < argument_count - 1 || (std::is_base_of<rest_base, T>::value && argItem == argument_count - 1)))
				|| (increment<0 && templateItem == maxTemplate - 1 && (argument_count - 1>maxTemplate || (std::is_base_of<rest_base, T>::value && argument_count - 1==maxTemplate))))
			{
				unpack_rest(retValue, arguments, argItem, increment>0 ? argument_count-1 : maxTemplate, increment);
				templateItem+=increment;
				return retValue;
			}

			//genuine parameter
			//will generate compilation error if T is not convertible to jsrt::value
			else if ((increment > 0 && argItem < argument_count) ||
				(increment < 0 && templateItem + 1 <= argument_count - 1)) {
				if (to_native(arguments[argItem], &retValue) != JsNoError) throw jsrt::fatal_exception();
				argItem += increment;
			}

			templateItem+=increment;
			
			return retValue;
		}

		template <class R>
		static R unpack_rest(R trait, ...) {
			//last parameter is not "rest" type, throw exception
			throw jsrt::invalid_argument_exception();
		}

		
		template<class T>
		static void unpack_rest(rest<T> &restValue, JsValueRef *arguments, int &argItem, USHORT limit, int increment)
		{
			T retValue;

			int i;

			for (i=min(argItem, limit); i<=max(argItem,limit); i++) {
				if (to_native(arguments[i], &retValue) != JsNoError) throw jsrt::fatal_exception();
				restValue.push_back(retValue);
			}
			argItem += (i - min(argItem, limit)) * increment;
		}

		/// <summary>
		///     General proxy form method callback passed to JS.
		/// </summary>
		template<class R, class... P>
		static JsValueRef CALLBACK thunk(JsValueRef callee, bool is_construct_call, JsValueRef *arguments, unsigned short argument_count, void *callback_state)
		{
			call_info info(value(callee), value(arguments[0]), is_construct_call);

			callback_info<R, P...> *callback = (callback_info<R, P...>*)callback_state;

			//differentiates overrides with/without return value;
			std::conditional<std::is_void<R>::value, bool, int *>::type isVoid = 0; 
			auto result = thunk2<R, P...>(callback, arguments, argument_count, isVoid);

			return result;
		}

		//to call arguments in order we use initialization list passed over by this helper
		//NOTE: VC++ 2013 seems not to follow the stadard and calls in reversed order
		static struct ordered_pass {
			template<class R, class F, typename ...T> 
			ordered_pass(R* r, F f, bool discard, T... arg) {
				*r = f(arg...);
			}
			
			template<class F, typename ...T>
			ordered_pass(F f, bool discard, T... arg) {
				f(arg...);
			}

			static void iterate(...) {}
		};

		//override without call_info and with return value
		template<class R, class... P>
		static JsValueRef thunk2(callback_info<R, P...>*callback, JsValueRef *arguments, unsigned short argument_count, int *notReturnsVoid) {
			R result = R();
			const unsigned maxTemplate = sizeof...(P);

			int argItem = argument_count - 1, templateItem = maxTemplate - 1, increment = -1;

			try {
				ordered_pass{&result, *callback, false, //if correct order, reset counters for left-right
					 unpack<P>(arguments, argItem, argument_count, templateItem, maxTemplate, increment)... };
			}
			catch (const invalid_argument_exception &) {
				context::set_exception(error::create_type_error(L"Incorrect number of arguments."));
				return JS_INVALID_REFERENCE;
			}
			catch (...) {
				context::set_exception(error::create(L"Fatal error."));
				return JS_INVALID_REFERENCE;
			}

			JsValueRef resultValue;
			auto errCode = from_native(result, &resultValue);
			if (errCode == JsErrorInExceptionState) { //don't throw exception if already in exception
				return JS_INVALID_REFERENCE;
			}
			else if (errCode != JsNoError) {
				context::set_exception(error::create_type_error(L"Could not convert value."));
				return JS_INVALID_REFERENCE;
			}

			return resultValue;
		}

		//override without call_info and without return value
		template<class R, class... P>
		static JsValueRef thunk2(callback_info<R, P...>*callback, JsValueRef *arguments, unsigned short argument_count, bool returnsVoid) {
			JsValueRef undefined = context::undefined().handle();
			const unsigned maxTemplate = sizeof...(P);

			//default starts from end, due to VC++ not std compliant and call right-left
			int argItem = argument_count-1, templateItem = maxTemplate-1, increment=-1; 

			try {
				//use initializer list call, to force expansion order evaluation -- NOT WORKING in VC++ 2013!!
				ordered_pass{ *callback, false, //(argItem=1, templateItem=0, increment=1,false), //if correct order, reset counters for left-right
					unpack<P>(arguments, argItem, argument_count, templateItem, maxTemplate,increment)... };
			}
			catch (const invalid_argument_exception &) {
				context::set_exception(error::create_type_error(L"Incorrect number of arguments."));
				return JS_INVALID_REFERENCE;
			}
			catch (...) {
				context::set_exception(error::create(L"Fatal error."));
				return JS_INVALID_REFERENCE;
			}

			return undefined;
		}



	};

};