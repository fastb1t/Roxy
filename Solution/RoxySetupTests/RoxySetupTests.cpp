#include "pch.h"
#include "CppUnitTest.h"

#include "../Roxy/Roxy/Algorithms.h"

#pragma comment(lib, "../Debug/Roxy.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RoxySetupTests
{
	TEST_CLASS(RoxySetupTests)
	{
	public:
		
		TEST_METHOD(StringToWstring)
		{
			Assert::AreEqual(std::wstring(L"Test String"), Roxy::StringToWstring("Test String"));
		}
	};
}
