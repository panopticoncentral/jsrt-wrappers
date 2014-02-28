#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{		
    TEST_CLASS(Runtime)
    {
    public:
        
        TEST_METHOD(Basic)
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            runtime.dispose();
        }
    };
}