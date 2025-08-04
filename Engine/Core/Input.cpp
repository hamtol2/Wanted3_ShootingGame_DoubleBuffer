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
		keyStates[ix].isKeyDown = GetAsyncKeyState(ix) & 0x8000;
	}
}

void Input::DispatchCallbacks()
{
	// 키 눌림 이벤트 발행.
	if (keydownCallbacks.size() > 0)
	{
		for (std::pair<const int, std::vector<Delegate>>& pair : keydownCallbacks)
		{
			// 키 눌림 확인.
			if (GetKeyDown(pair.first))
			{
				// 키 눌림 이벤트가 발생한 경우, 콜백 함수 호출.
				for (Delegate& delegate : pair.second)
				{
					delegate();
				}
			}
		}
	}

	// 키 눌림 해제 이벤트 발행.
	if (keyupCallbacks.size() > 0)
	{
		for (std::pair<const int, std::vector<Delegate>>& pair : keyupCallbacks)
		{
			// 키 눌림 해제 확인.
			if (GetKeyUp(pair.first))
			{
				// 키 눌림 해제 이벤트가 발생한 경우, 콜백 함수 호출.
				for (Delegate& delegate : pair.second)
				{
					delegate();
				}
			}
		}
	}

	// 키 눌림 반복 이벤트 발행.
	if (keyrepeatCallbacks.size() > 0)
	{
		for (std::pair<const int, std::vector<Delegate>>& pair : keyrepeatCallbacks)
		{
			// 키 눌림 반복 이벤트 확인.
			if (GetKey(pair.first))
			{
				// 키 눌림 반복 이벤트가 발생한 경우, 콜백 함수 호출.
				for (Delegate& delegate : pair.second)
				{
					delegate();
				}
			}
		}
	}
}

void Input::SavePreviouseKeyStates()
{
	for (int ix = 0; ix < 255; ++ix)
	{
		keyStates[ix].previouseKeyDown = keyStates[ix].isKeyDown;
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