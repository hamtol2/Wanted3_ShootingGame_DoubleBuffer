#include "ScreenBuffer.h"
#include <iostream>

ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
	: screenSize(screenSize)
{
	// 접근 모드 설정 (읽기/쓰기).
	unsigned long accessMode = GENERIC_READ | GENERIC_WRITE;

	// 공유 모드 설정 (파일 입출력).
	unsigned long shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

	// 콘솔 버퍼 생성.
	buffer = CreateConsoleScreenBuffer(accessMode, shareMode, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// 예외처리: 버퍼 생성 실패.
	if (buffer == INVALID_HANDLE_VALUE)
	{
		//__debugbreak();
		std::cout << "버퍼 생성 실패\n";
		return;
	}

	// 콘솔 버퍼 크기 설정.
	SetConsoleScreenBufferSize(buffer, Vector2(screenSize.x, screenSize.y));
	SMALL_RECT rect{ 0, 0, (short)screenSize.x, (short)screenSize.y };

	// 콘솔 창 크기 설정.
	SetConsoleWindowInfo(buffer, true, &rect);

	// 커서 안보이게 설정.
	CONSOLE_CURSOR_INFO info{ 1, FALSE };
	SetConsoleCursorInfo(buffer, &info);

	// @Test.
	DWORD mode;
	GetConsoleMode(buffer, &mode);
	mode |= ENABLE_QUICK_EDIT_MODE;
	mode |= ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(buffer, mode);
}

ScreenBuffer::ScreenBuffer(HANDLE console, const Vector2& screenSize)
	: screenSize(screenSize), buffer(console)
{
	// 커서 안보이게 설정.
	CONSOLE_CURSOR_INFO cursorInfo{ 1, FALSE };
	SetConsoleCursorInfo(buffer, &cursorInfo);

	// 
	CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo = {};
	GetConsoleScreenBufferInfoEx(buffer, &bufferInfo);
	bufferInfo.dwSize.X = screenSize.x + 1;
	bufferInfo.dwSize.Y = screenSize.y + 1;
	SetConsoleScreenBufferInfoEx(buffer, &bufferInfo);

	// @Test.
	DWORD mode;
	GetConsoleMode(buffer, &mode);
	mode |= ENABLE_QUICK_EDIT_MODE;
	mode |= ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(buffer, mode);
}

ScreenBuffer::~ScreenBuffer()
{
	// 생성한 핸들 해제.
	if (buffer)
	{
		CloseHandle(buffer);
	}
}

void ScreenBuffer::Clear()
{
	// 커서 위치.
	Vector2 position;

	// 콘솔에 기록한 글자 수를 반환받기 위한 변수.
	DWORD writtenCount = 0;

	// 화면 버퍼 전체에 빈 문자열 기록.
	// 콘솔 전체에 한 글자를 입력할 때 사용 (memset과 비슷).
	FillConsoleOutputCharacter(buffer, ' ', (screenSize.x + 1) * screenSize.y + 1, position, &writtenCount);
}

void ScreenBuffer::Render(CHAR_INFO* charInfo)
{
	// 버퍼 위치 초기화.
	Vector2 bufferPosition;

	// 기록할 영역의 크기.
	SMALL_RECT writeRegion{ 0, 0, (short)screenSize.x - 1, (short)screenSize.y - 1 };

	// 전달된 문자 정보를 버퍼에 기록.
	WriteConsoleOutputA(buffer, charInfo, screenSize, bufferPosition, &writeRegion);
}