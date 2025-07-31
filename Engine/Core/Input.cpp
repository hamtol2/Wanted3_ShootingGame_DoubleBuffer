#include "Input.h"
#include <Windows.h>

// static 변수 정의.
Input* Input::instance = nullptr;

Input::Input()
{
	// 싱글톤 실행을 위해 instance 변수 설정.
	instance = this;
}

void Input::ProcessInput()
{
	// 키 입력 확인.
	for (int ix = 0; ix < 255; ++ix)
	{
		keyStates[ix].isKeyDown
			= GetAsyncKeyState(ix) & 0x8000;
	}
}

void Input::SavePreviouseKeyStates()
{
	for (int ix = 0; ix < 255; ++ix)
	{
		keyStates[ix].previouseKeyDown
			= keyStates[ix].isKeyDown;
	}
}

bool Input::GetKey(int keyCode)
{
	return keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyDown(int keyCode)
{
	return !keyStates[keyCode].previouseKeyDown
		&& keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyUp(int keyCode)
{
	return keyStates[keyCode].previouseKeyDown
		&& !keyStates[keyCode].isKeyDown;
}

Input& Input::Get()
{
	return *instance;
}