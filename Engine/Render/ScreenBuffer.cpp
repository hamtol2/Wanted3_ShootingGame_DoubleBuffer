#include "ScreenBuffer.h"
#include <iostream>

ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
	: screenSize(screenSize)
{
	// ���� ��� ���� (�б�/����).
	unsigned long accessMode = GENERIC_READ | GENERIC_WRITE;

	// ���� ��� ���� (���� �����).
	unsigned long shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

	// �ܼ� ���� ����.
	buffer = CreateConsoleScreenBuffer(accessMode, shareMode, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// ����ó��: ���� ���� ����.
	if (buffer == INVALID_HANDLE_VALUE)
	{
		//__debugbreak();
		std::cout << "���� ���� ����\n";
		return;
	}

	// �ܼ� â ũ�� ����.
	SMALL_RECT rect{ 0, 0, (short)screenSize.x, (short)screenSize.y };
	BOOL setWindowsInfoResult = SetConsoleWindowInfo(buffer, true, &rect);
	if (!setWindowsInfoResult)
	{
		std::cerr << "Failed to set console window size\n";
		__debugbreak();
	}

	// �ܼ� ���� ũ�� ����.
	BOOL setScreenBufferSizeResult 
		= SetConsoleScreenBufferSize(buffer, Vector2(screenSize.x + 1, screenSize.y + 1));
	if (!setScreenBufferSizeResult)
	{
		std::cerr << "Failed to set screen buffer size\n";
		__debugbreak();
	}

	// Ŀ�� �Ⱥ��̰� ����.
	CONSOLE_CURSOR_INFO info{ 1, FALSE };
	SetConsoleCursorInfo(buffer, &info);
}

ScreenBuffer::ScreenBuffer(HANDLE console, const Vector2& screenSize)
	: screenSize(screenSize), buffer(console)
{
	// �ܼ� â ũ�� ����.
	SMALL_RECT rect{ 0, 0, (short)screenSize.x, (short)screenSize.y };
	BOOL setWindowsInfoResult = SetConsoleWindowInfo(buffer, true, &rect);
	if (!setWindowsInfoResult)
	{
		std::cerr << "Failed to set console window size\n";
		__debugbreak();
	}

	// �ܼ� ���� ũ�� ����.
	BOOL setScreenBufferSizeResult 
		= SetConsoleScreenBufferSize(buffer, Vector2(screenSize.x + 1, screenSize.y + 1));
	if (!setScreenBufferSizeResult)
	{
		std::cerr << "Failed to set screen buffer size\n";
		__debugbreak();
	}

	// Ŀ�� �Ⱥ��̰� ����.
	CONSOLE_CURSOR_INFO cursorInfo{ 1, FALSE };
	SetConsoleCursorInfo(buffer, &cursorInfo);
}

ScreenBuffer::~ScreenBuffer()
{
	// ������ �ڵ� ����.
	if (buffer)
	{
		CloseHandle(buffer);
	}
}

void ScreenBuffer::Clear()
{
	// Ŀ�� ��ġ.
	Vector2 position;

	// �ֿܼ� ����� ���� ���� ��ȯ�ޱ� ���� ����.
	DWORD writtenCount = 0;

	// ȭ�� ���� ��ü�� �� ���ڿ� ���.
	// �ܼ� ��ü�� �� ���ڸ� �Է��� �� ��� (memset�� ���).
	FillConsoleOutputCharacter(buffer, ' ', (screenSize.x + 1) * screenSize.y + 1, position, &writtenCount);
}

void ScreenBuffer::Render(CHAR_INFO* charInfo)
{
	// ���� ��ġ �ʱ�ȭ.
	Vector2 bufferPosition;

	// ����� ������ ũ��.
	SMALL_RECT writeRegion{ 0, 0, (short)screenSize.x - 1, (short)screenSize.y - 1 };

	// ���޵� ���� ������ ���ۿ� ���.
	WriteConsoleOutputA(buffer, charInfo, screenSize, bufferPosition, &writeRegion);
}