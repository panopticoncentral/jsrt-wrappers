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

#define IfComFailError(v) \
    { \
    hr = (v); \
    if (FAILED(hr)) \
        { \
        goto error; \
        } \
    }

extern IDebugApplication *get_debug_application();