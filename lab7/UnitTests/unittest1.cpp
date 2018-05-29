#include "stdafx.h"
#include "CppUnitTest.h"
#include "../lab7/Alg.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


	TEST_CLASS(CutTest)
	{
	public:

		TEST_METHOD(newCut_t)
		{
			int a = 3;
			int b = 3;

			Assert::AreEqual(a, b, L"message", LINE_INFO());
		}
		TEST_METHOD(newT)
		{
			int a = 10, b = 20, c = 100, d = 30;

			Assert::AreEqual(b, a, L"message", LINE_INFO());
			//Assert::AreEqual(cut->begin->y, b, L"message", LINE_INFO());
			//Assert::AreEqual(cut->end->x, c, L"message", LINE_INFO());
			//Assert::AreEqual(cut->end->y, d, L"message", LINE_INFO());
		}
		/*
		TEST_METHOD(cutTan_t_HORIZONTAL)
		{
			int a = 3;
			int b = 3;
			Assert::AreEqual(a, b, L"message", LINE_INFO());
		}
		TEST_METHOD(cutTan_t_random)
		{
			int a = 3;
			int b = 3;
			Assert::AreEqual(a, b, L"message", LINE_INFO());
		}
		*/

	};
	TEST_CLASS(CutTest1)
	{
	public:
		TEST_METHOD(newCut_t)
		{
			int a = 3;
			int b = 3;

			Assert::AreEqual(a, b, L"message", LINE_INFO());
		}
		

	};
