#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(object)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty object handle.")
        {
            jsrt::object handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            jsrt::object object;
            TEST_NO_CONTEXT_CALL(object.is_external());
            TEST_NO_CONTEXT_CALL(object.is_extension_allowed());
            TEST_NO_CONTEXT_CALL(object.prevent_extension());
            TEST_NO_CONTEXT_CALL(object.prototype());
            TEST_NO_CONTEXT_CALL(object.set_prototype(object));
            TEST_NO_CONTEXT_CALL(object.has_property(jsrt::property_id()));
            TEST_NO_CONTEXT_CALL(object.set_property(jsrt::property_id(), jsrt::value()));
            TEST_NO_CONTEXT_CALL(object.get_property(jsrt::property_id()));
            TEST_NO_CONTEXT_CALL(object.delete_property(jsrt::property_id()));
            TEST_NO_CONTEXT_CALL(object.get_own_property_descriptor(jsrt::property_id()));
            TEST_NO_CONTEXT_CALL(object.define_property(jsrt::property_id(), jsrt::property_descriptor<double>()));
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object object;
                TEST_NULL_ARG_CALL(object.is_external());
                TEST_NULL_ARG_CALL(object.is_extension_allowed());
                TEST_NULL_ARG_CALL(object.prevent_extension());
                TEST_NULL_ARG_CALL(object.prototype());
                TEST_NULL_ARG_CALL(object.set_prototype(object));
                TEST_NULL_ARG_CALL(object.has_property(jsrt::property_id()));
                TEST_NULL_ARG_CALL(object.set_property(jsrt::property_id(), jsrt::value()));
                TEST_NULL_ARG_CALL(object.get_property(jsrt::property_id()));
                TEST_NULL_ARG_CALL(object.delete_property(jsrt::property_id()));
                TEST_NULL_ARG_CALL(object.get_own_property_descriptor(jsrt::property_id()));
                TEST_NULL_ARG_CALL(object.define_property(jsrt::property_id(), jsrt::property_descriptor<double>()));

                jsrt::object valid_object = jsrt::object::create();
                TEST_NULL_ARG_CALL(valid_object.set_prototype(jsrt::object()));
                TEST_INVALID_ARG_CALL(valid_object.has_property(jsrt::property_id()));
                TEST_INVALID_ARG_CALL(valid_object.set_property(jsrt::property_id(), jsrt::value()));
                TEST_NULL_ARG_CALL(valid_object.set_property(jsrt::property_id::create(L"foo"), jsrt::value()));
                TEST_INVALID_ARG_CALL(valid_object.get_property(jsrt::property_id()));
                TEST_INVALID_ARG_CALL(valid_object.delete_property(jsrt::property_id()));
                TEST_INVALID_ARG_CALL(valid_object.get_own_property_descriptor(jsrt::property_id()));
                TEST_INVALID_ARG_CALL(valid_object.define_property(jsrt::property_id(), jsrt::property_descriptor<double>()));
                TEST_NULL_ARG_CALL(valid_object.define_property(jsrt::property_id::create(L"foo"), jsrt::property_descriptor<double>()));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(create, "Test ::create method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::value value = jsrt::object::create();
                jsrt::object object = (jsrt::object)value;
                Assert::IsFalse(object.is_external());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(extension, "Test ::is_extension_allowed and ::prevent_extension methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object object = jsrt::object::create();
                Assert::IsTrue(object.is_extension_allowed());
                object.set_property(jsrt::property_id::create(L"foo"), 10);
                object.prevent_extension();
                Assert::IsFalse(object.is_extension_allowed());
                TEST_SCRIPT_EXCEPTION_CALL(object.set_property(jsrt::property_id::create(L"bar"), 20));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(prototype, "Test ::prototype and ::set_prototype methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object object = jsrt::object::create();
                jsrt::object prototype = object.prototype();
                prototype = jsrt::object::create();
                object.set_prototype(prototype);
                Assert::AreEqual(prototype.handle(), object.prototype().handle());
                object.prevent_extension();
                TEST_SCRIPT_EXCEPTION_CALL(object.set_prototype(jsrt::object::create()));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(properties, "Test property methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object object = jsrt::object::create();

                object.set_property(jsrt::property_id::create(L"foo"), 10);
                object.set_property(jsrt::property_id::create(L"bar"), 20.0);
                object.set_property(jsrt::property_id::create(L"baz"), true);
                object.set_property(jsrt::property_id::create(L"x"), L"foo");
                object.set_property(jsrt::property_id::create(L"y"), object);

                TEST_INVALID_ARG_CALL(object.get_property<int>(jsrt::property_id::create(L"foo")));
                Assert::AreEqual(object.get_property<double>(jsrt::property_id::create(L"foo")), 10.0);
                Assert::AreEqual(object.get_property<double>(jsrt::property_id::create(L"bar")), 20.0);
                Assert::AreEqual(object.get_property<bool>(jsrt::property_id::create(L"baz")), true);
                Assert::AreEqual(object.get_property<std::wstring>(jsrt::property_id::create(L"x")), (std::wstring)L"foo");
                Assert::AreEqual(object.get_property(jsrt::property_id::create(L"y")).handle(), object.handle());

                Assert::AreEqual(object.get_property(jsrt::property_id::create(L"z")).handle(), jsrt::context::undefined().handle());
                Assert::IsFalse(object.has_property(jsrt::property_id::create(L"z")));
                object.set_property(jsrt::property_id::create(L"z"), 10);
                Assert::IsTrue(object.has_property(jsrt::property_id::create(L"z")));
                Assert::AreEqual(object.get_property<double>(jsrt::property_id::create(L"z")), 10.0);
                object.delete_property(jsrt::property_id::create(L"z"));
                Assert::IsFalse(object.has_property(jsrt::property_id::create(L"z")));
                Assert::AreEqual(object.get_property(jsrt::property_id::create(L"z")).handle(), jsrt::context::undefined().handle());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(descriptors, "Test ::get_own_property_descriptor and ::define_property methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::object object = jsrt::object::create();

                jsrt::property_descriptor<double> desc = jsrt::property_descriptor<double>::create();
                desc.set_configurable(false);
                desc.set_writable(false);
                desc.set_value(10);
                object.define_property(jsrt::property_id::create(L"a"), desc);
                object.set_property(jsrt::property_id::create(L"a"), 20, false);
                Assert::AreEqual(object.get_property<double>(jsrt::property_id::create(L"a")), 10.0);
                TEST_SCRIPT_EXCEPTION_CALL(object.set_property(jsrt::property_id::create(L"a"), 20));
                TEST_SCRIPT_EXCEPTION_CALL(object.delete_property(jsrt::property_id::create(L"a")));
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(enumerable, "")
        {

        }

        MY_TEST_METHOD(indexes, "")
        {

        }
    };
}