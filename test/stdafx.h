#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include "jsrt.h"
#include "jsrt_wrappers.h"
#include "CppUnitTest.h"

#define MY_TEST_METHOD(name, description) \
    BEGIN_TEST_METHOD_ATTRIBUTE(name) \
        TEST_DESCRIPTION(description) \
    END_TEST_METHOD_ATTRIBUTE() \
    \
    TEST_METHOD(name)

#define MY_TEST_METHOD_DISABLED(name, description) \
    BEGIN_TEST_METHOD_ATTRIBUTE(name) \
        TEST_DESCRIPTION(description) \
        TEST_IGNORE() \
    END_TEST_METHOD_ATTRIBUTE() \
    \
    TEST_METHOD(name)

#define TEST_FAILED_CALL(call, exception) \
    try \
    { \
        call; \
        Assert::Fail(); \
    } \
    catch (const jsrt:: exception &) \
    { \
    }

#define TEST_INVALID_ARG_CALL(call) \
    TEST_FAILED_CALL(call, invalid_argument_exception)

#define TEST_NO_CONTEXT_CALL(call) \
    TEST_FAILED_CALL(call, no_current_context_exception)

#define TEST_NULL_ARG_CALL(call) \
    TEST_FAILED_CALL(call, null_argument_exception)

#define IfComFailError(v) \
    { \
    hr = (v); \
    if (FAILED(hr)) \
        { \
        goto error; \
        } \
    }

extern IDebugApplication *get_debug_application();

template <>
static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const JsValueType& q) 
{
    switch (q)
    {
    case JsUndefined:
        return L"JsUndefined";
    case JsNull:
        return L"JsNull";
    case JsNumber:
        return L"JsNumber";
    case JsString:
        return L"JsString";
    case JsBoolean:
        return L"JsBoolean";
    case JsObject:
        return L"JsObject";
    case JsFunction:
        return L"JsFunction";
    case JsError:
        return L"JsError";
    case JsArray:
        return L"JsArray";
    default:
        return std::wstring();
    }
}
