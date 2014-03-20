#JsRT Wrappers for C++

The JsRT Wrappers C++ are intended to simplify working with the JsRT interface to the Chakra JavaScript engine in C++. Since the JsRT API is a flat Win32-style interface, there are a number of conventions it uses that are not entirely friendly to the C++ programmer. This library tries to smooth that out as much as possible.

## Install

Simply download the latest release and unzip. `#include` the `jsrt-wrapper.h` header and link in `jsrt-wrapper.lib` for the relevant platform.

## Dependencies

The JsRT APIs are only available on Windows 8.1 or later.

## A Tour Through the Helpers

Most of the wrappers simply provide a nice object-oriented interface over the JsRT API. For example, creating a runtime and a context works like this:

	jsrt::runtime runtime = jsrt::runtime::create();
	jsrt::context context = runtime.create_context();
	
There are, however, a number of special features in the wrappers.

### Context scopes

A context scope (`jsrt::context::scope`) automatically sets the current context when it comes into scope and clears the current context when it goes out of scope. This greatly simplifies working with scopes. For example:

	jsrt::runtime runtime = jsrt::runtime::create();
	jsrt::context context = runtime.create_context();
	{
		jsrt::context::scope scope(context);
		
		... work with context ...
	}
	// Context has automatically been set back to original value.
	runtime.dispose();

### Pinned references

To keep a reference to a Chakra object alive, it must be visible to the Chakra garbage collector (i.e. on the stack or stored in the garbage collected heap) or it has to be pinned using `JsAddRef`. The `pinned<T>` class functions as a smart-pointer template that keeps a reference alive using `JsAddRef` and `JsRelease` automatically. For example:

	{
		jsrt::pinned<jsrt::object> pinned_obj = jsrt::object::create();
		// Object reference does not have to stay on the stack or in the GC heap
	}
	// Reference can now be garbage collected
	
### Value translation

Value getter and setter functions can be strongly-typed, allowing automatic marshalling of JavaScript values to C++ values and vice versa. For example:

	jsrt::object obj = jsrt::object::create();
	obj.set_property(jsrt::property_id::create(L"boolProperty"), true);
	bool bool_value = obj.get_property<bool>(jsrt::property_id::create(L"boolProperty"));
	obj.set_property(jsrt::property_id::create(L"stringProperty"), L"foo");

The wrappers marshal types in the following way:

* number values are marshalled to/from `double`
* string values are marshalled to/from `std::wstring`
* Boolean values are marshalled to/from `bool`

### Strongly-typed arrays

Similarly, arrays can be strongly typed and accessed using numeric indexes:

	jsrt::array<double> darray = jsrt::array<double>::create(1);
	darray[0] = 10;
	darray[1] = 20;

### Functions

Perhaps the most useful aspect of the wrappers is the way that JavaScript functions can be wrapped and their arguments and return values marshalled to C++ types, and vice versa. For example:

	auto f = (jsrt::function<double, double, double>)jsrt::context::evaluate(L"function f(a, b) { return a + b; }; f;");
	double a = f(jsrt::context::undefined(), 1, 2);

Native functions can also be wrapped:

	double add(const jsrt::call_info &info, double a, double b) { return a + b; }
	auto nf = jsrt::function<double, double, double>::create(add);
	jsrt::context::global().set_property(jsrt::property_id::create(L"add"), nf);
	jsrt::context::run(L"add(1, 2)");

When calling a function wrapper, the first argument to the function call is the `this` value for the call. Function wrappers can also be _bound_ to a particular value of `this`. For example:

	auto bf = jsrt::bound_function<jsrt::value, double, double, double>::create(
		jsrt::context::undefined(),
		(jsrt::function<double, double, double>)jsrt::context::evaluate(L"function f(a, b) { return a + b; }; f;"));
	double ba = bf(1, 2);
