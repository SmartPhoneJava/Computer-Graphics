#include "stdafx.h"
#include "CppUnitTest.h"
#include "../lab7/Cut.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//namespace UnitTests
//{		
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

	};

	/*
Cut* newCutInt(int x1, int y1, int x2, int y2);

void deleteCut(Cut **cut);

void debugCut(Cut *cut, const char* text, int number);

void debugCutVisibility(Cut *cut, const char* text, int number);

bool compareCuts(Cut* A, Cut *B);*/