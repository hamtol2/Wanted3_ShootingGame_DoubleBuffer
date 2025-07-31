#pragma once

#include <Windows.h>
#include "Math/Vector2.h"
#include "Math/Color.h"
//#include <iostream>

// 프로젝트에서 다양하게 사용할 유틸리티 함수 모음.
namespace Utils
{
	// 두 값을 서로 바꿀 때 사용하는 함수.
	template<typename T>
	void Swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	// 콘솔 출력 핸들 가져오는 함수.
	inline HANDLE GetConsoleHandle()
	{
		return GetStdHandle(STD_OUTPUT_HANDLE);
	}

	// 콘솔 커서 위치 이동 함수.
	inline void SetConsolePosition(COORD coord)
	{
		static HANDLE handle = GetConsoleHandle();
		SetConsoleCursorPosition(handle, coord);
	}

	inline void SetConsolePosition(const Vector2& position)
	{
		SetConsolePosition(static_cast<COORD>(position));
	}

	// 콘솔 텍스트 색상 설정 함수.
	inline void SetConsoleTextColor(WORD color)
	{
		static HANDLE handle = GetConsoleHandle();
		SetConsoleTextAttribute(handle, color);
	}

	inline void SetConsoleTextColor(Color color)
	{
		SetConsoleTextColor(static_cast<WORD>(color));
	}

	// 랜덤 생성 함수.
	inline int Random(int min, int max)
	{
		// max와 min의 차이 구하기.
		int diff = (max - min) + 1;
		return ((diff * rand()) / (RAND_MAX + 1)) + min;
	}

	inline float RandomFloat(float min, float max)
	{
		// 0 ~ 1 사이의 난수 구하기.
		float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		return random * (max - min) + min;
	}
}