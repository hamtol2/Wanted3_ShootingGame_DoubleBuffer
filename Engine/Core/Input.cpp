#include "Input.h"
#include "Engine.h"
#include "Render/ScreenBuffer.h"
#include <Windows.h>

// static 변수 정의.
Input* Input::instance = nullptr;

Input::Input()
{
	// 싱글톤 실행을 위해 instance 변수 설정.
	instance = this;

	// 마우스 이벤트 활성화.
	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | /*ENABLE_PROCESSED_INPUT | */ENABLE_EXTENDED_FLAGS;
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	BOOL result = SetConsoleMode(inputHandle, mode);

	if (result == FALSE)
	{
		int errorCode = GetLastError();
		OutputDebugStringA("마우스 입력 설정 실패\n");
		__debugbreak();
	}
}

void Input::ProcessInput()
{
	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD record = { };
	DWORD events = 0;

	if (PeekConsoleInput(inputHandle, &record, 1, &events) && events > 0)
	{
		char eventCountString[50] = {};
		sprintf_s(eventCountString, 50, "EventCount: %d | eventType: %d \n", events, record.EventType);
		OutputDebugStringA(eventCountString);

		if (ReadConsoleInput(inputHandle, &record, 1, &events))
		{
			//if (record.EventType == WINDOW_BUFFER_SIZE_EVENT)
			//{
			//	continue;
			//}

			// @Test: 마우스 이벤트 들어오는지 확인.
			if (record.EventType == MOUSE_EVENT)
			{
				char debugMessage[100] = {};
				sprintf_s(debugMessage, 100, "EventType: %d | Click Status: %d \n",
					record.EventType, (record.Event.MouseEvent.dwButtonState));
				OutputDebugStringA(debugMessage);
			}

			switch (record.EventType)
			{
			case KEY_EVENT:
			{
				if (record.Event.KeyEvent.bKeyDown)
				{
					keyStates[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = true;
				}
				else
				{
					keyStates[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = false;
				}
			}
			break;

			case MOUSE_EVENT:
			{
				mousePosition.x = record.Event.MouseEvent.dwMousePosition.X;
				mousePosition.y = record.Event.MouseEvent.dwMousePosition.Y;

				keyStates[VK_LBUTTON].isKeyDown
					= (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;

				keyStates[VK_RBUTTON].isKeyDown
					= (record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;
			}
			break;
			}
		}
	}

	//// 키 입력 테스트.
	//for (int ix = 0; ix < 255; ++ix)
	//{
	//	keyStates[ix].isKeyDown = (GetAsyncKeyState(ix) & 0x8000) ? true : false;
	//}
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

Vector2 Input::MousePosition() const
{
	return mousePosition;
}

Input& Input::Get()
{
	return *instance;
}