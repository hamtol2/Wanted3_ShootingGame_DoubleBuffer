#include "Input.h"
#include <Windows.h>

// static ���� ����.
Input* Input::instance = nullptr;

Input::Input()
{
	// �̱��� ������ ���� instance ���� ����.
	instance = this;
}

void Input::ProcessInput()
{
	// Ű �Է� Ȯ��.
	for (int ix = 0; ix < 255; ++ix)
	{
		keyStates[ix].isKeyDown = GetAsyncKeyState(ix) & 0x8000;
	}
}

void Input::DispatchCallbacks()
{
	for (auto& pair : keydownCallbacks)
	{
		if (GetKeyDown(pair.first))
		{
			pair.second.Invoke();
		}
	}

	for (auto& pair : keyupCallbacks)
	{
		if (GetKeyUp(pair.first))
		{
			pair.second.Invoke();
		}
	}

	for (auto& pair : keyrepeatCallbacks)
	{
		if (GetKey(pair.first))
		{
			pair.second.Invoke();
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