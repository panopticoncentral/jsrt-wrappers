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

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include "jsrt.h"
#include "jsrt-wrappers.h"
#include "profiler.h"
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

#define TEST_SCRIPT_EXCEPTION_CALL(call) \
    TEST_FAILED_CALL(call, script_exception)

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
    case JsSymbol:
        return L"JsSymbol";
    case JsArrayBuffer:
        return L"JsArrayBuffer";
    case JsTypedArray:
        return L"JsTypedArray";
    case JsDataView:
        return L"JsDataView";
    default:
        return std::wstring();
    }
}

template <>
static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const JsPropertyIdType& q)
{
    switch (q)
    {
    case JsPropertyIdTypeString:
        return L"JsPropertyIdTypeString";
    case JsPropertyIdTypeSymbol:
        return L"JsPropertyIdTypeSymbol";
    default:
        return std::wstring();
    }
}
