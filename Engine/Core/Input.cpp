#include "Input.h"
#include "Engine.h"
#include "Render/ScreenBuffer.h"
#include <Windows.h>

// static ���� ����.
Input* Input::instance = nullptr;

Input::Input()
{
	// �̱��� ������ ���� instance ���� ����.
	instance = this;

	// ���콺 �̺�Ʈ Ȱ��ȭ.
	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	BOOL result = SetConsoleMode(inputHandle, mode);
	
	if (result == FALSE)
	{
		int errorCode = GetLastError();
		OutputDebugStringA("���콺 �Է� ���� ����\n");
		__debugbreak();
	}
}

void Input::ProcessInput()
{
	static HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	//static HANDLE inputHandle = CreateFileW(L"CONIN$", 
	//	GENERIC_READ | GENERIC_WRITE,
	//	FILE_SHARE_READ | FILE_SHARE_WRITE,
	//	NULL, OPEN_EXISTING, 0, NULL
	//);

	static bool initialized = false;
	if (!initialized)
	{
		// ���콺 �̺�Ʈ Ȱ��ȭ.
		DWORD mode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
		mode &= ~ENABLE_QUICK_EDIT_MODE;
		BOOL result = SetConsoleMode(inputHandle, mode);

		if (result == FALSE)
		{
			int errorCode = GetLastError();
			OutputDebugStringA("���콺 �Է� ���� ����\n");
			__debugbreak();
		}

		initialized = true;
	}

	const int recordCount = 256;
	INPUT_RECORD records[recordCount] = {};
	DWORD eventReadCount = 0;
	
	if (PeekConsoleInput(inputHandle, records, recordCount, &eventReadCount) && eventReadCount > 0)
	{
		if (ReadConsoleInput(inputHandle, records, recordCount, &eventReadCount))
		{
			//char eventCountAndTypeString[50] = {};
			//sprintf_s(eventCountAndTypeString, 50, "EventCount: %d \n", eventReadCount);
			//OutputDebugStringA(eventCountAndTypeString);

			for (int ix = 0; ix < (int)eventReadCount; ++ix)
			{
				INPUT_RECORD& record = records[ix];

				char eventCountAndTypeString[50] = {};
				sprintf_s(eventCountAndTypeString, 50, "EventCount: %d | EventType: %s \n",
					eventReadCount, 
					record.EventType == WINDOW_BUFFER_SIZE_EVENT ? "BufferSizeEvent" : 
					record.EventType == KEY_EVENT ? "KeyEvent" : 
					record.EventType == MOUSE_EVENT ? "MouseEvent" : 
					record.EventType == FOCUS_EVENT ? "FocusEvent" : 
					"MenuEvent" 
				);

				OutputDebugStringA(eventCountAndTypeString);

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
	}

	//// Ű �Է� �׽�Ʈ.
	//for (int ix = 0; ix < 255; ++ix)
	//{
	//	// Ű �Է� Ȯ��.
	//	keyStates[ix].isKeyDown = (GetAsyncKeyState(ix) & 0x8000) ? true : false;
	//}
}

void Input::DispatchCallbacks()
{
	// Ű ���� �̺�Ʈ ����.
	if (keydownCallbacks.size() > 0)
	{
		for (std::pair<const int, std::vector<Delegate>>& pair : keydownCallbacks)
		{
			// Ű ���� Ȯ��.
			if (GetKeyDown(pair.first))
			{
				// Ű ���� �̺�Ʈ�� �߻��� ���, �ݹ� �Լ� ȣ��.
				for (Delegate& delegate : pair.second)
				{
					delegate();
				}
			}
		}
	}

	// Ű ���� ���� �̺�Ʈ ����.
	if (keyupCallbacks.size() > 0)
	{
		for (std::pair<const int, std::vector<Delegate>>& pair : keyupCallbacks)
		{
			// Ű ���� ���� Ȯ��.
			if (GetKeyUp(pair.first))
			{
				// Ű ���� ���� �̺�Ʈ�� �߻��� ���, �ݹ� �Լ� ȣ��.
				for (Delegate& delegate : pair.second)
				{
					delegate();
				}
			}
		}
	}

	// Ű ���� �ݺ� �̺�Ʈ ����.
	if (keyrepeatCallbacks.size() > 0)
	{
		for (std::pair<const int, std::vector<Delegate>>& pair : keyrepeatCallbacks)
		{
			// Ű ���� �ݺ� �̺�Ʈ Ȯ��.
			if (GetKey(pair.first))
			{
				// Ű ���� �ݺ� �̺�Ʈ�� �߻��� ���, �ݹ� �Լ� ȣ��.
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