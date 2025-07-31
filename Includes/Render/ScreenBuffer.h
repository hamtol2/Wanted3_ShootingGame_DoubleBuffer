#pragma once

#include <Windows.h>
#include "Math/Vector2.h"

/*
* 작성자: 장세윤.
* Screen buffer
* 더블 버퍼링 구현에 사용할 화면 버퍼 클래스.
*/

class ScreenBuffer
{
	friend class Engine;

public:
	// 새로운 버퍼를 생성할 때 사용할 생성자.
	ScreenBuffer(const Vector2& screenSize);

	// conslle에 전달된 핸들을 사용해 콘솔 버퍼 생성.
	ScreenBuffer(HANDLE console, const Vector2& screenSize);

	// 소멸자.
	~ScreenBuffer();

	// 버퍼 지우기.
	void Clear();

	// 콘솔 버퍼에 문자열 버퍼를 쓸 때 사용하는 함수.
	void Render(CHAR_INFO* charInfo);

private:

	// 콘솔 핸들.
	HANDLE buffer = nullptr;

	// 콘솔 화면 크기.
	Vector2 screenSize;
};