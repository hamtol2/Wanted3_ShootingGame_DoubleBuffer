#include "Engine.h"
#include <iostream>
#include <Windows.h>

#include "Level/Level.h"
#include "Utils/Utils.h"
#include "Input.h"
#include "Render/ScreenBuffer.h"

// 정적 변수 초기화.
Engine* Engine::instance = nullptr;

BOOL WINAPI ConsoleMessageProcedure(DWORD CtrlType)
{
	switch (CtrlType)
	{
	case CTRL_CLOSE_EVENT:
		// Engine의 메모리 해제.
		//Engine::Get().~Engine();
		Engine::Get().CleanUp();
		return false;
	}

	return false;
}

Engine::Engine()
{
	instance = this;

	// 콘솔 커서 끄기.
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 1;

	SetConsoleCursorInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&info
	);
	
	// 엔진 설정 로드.
	LoadEngineSettings();

	// 랜덤 종자값(seed) 설정.
	srand(static_cast<unsigned int>(time(nullptr)));

	// 이미지 버퍼 생성 / 콘솔에 보낼 버퍼 생성.
	Vector2 screenSize(settings.width, settings.height);
	imageBuffer = new ImageBuffer((screenSize.x + 1) * screenSize.y + 1);

	// 버퍼 초기화 (문자 버퍼).
	ClearImageBuffer();

	// 두 개의 버퍼 생성.
	//renderTargets[0] = new ScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE), screenSize);
	renderTargets[0] = new ScreenBuffer(screenSize);
	renderTargets[1] = new ScreenBuffer(screenSize);

	// 버퍼 교환.
	Present();

	// 콘솔 창 이벤트 등록.
	SetConsoleCtrlHandler(ConsoleMessageProcedure, TRUE);

	// 콘솔 창 크기 변경 안되도록 설정.
	// "관리자 모드에서만 제대로 실행됨"
	DisableToResizeWindow();

	// cls 호출.
	system("cls");
}

Engine::~Engine()
{
	CleanUp();
}

void Engine::Run()
{
	// 밀리 세컨드 단위로 현재 시간을 알려줌.
	//float currentTime = timeGetTime();
	LARGE_INTEGER currentTime;
	LARGE_INTEGER previousTime;
	QueryPerformanceCounter(&currentTime);
	previousTime = currentTime;

	// 하드웨어 시계의 정밀도(주파수) 가져오기.
	// 나중에 초로 변환하기 위해.
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	// 타겟 프레임.
	float targetFrameRate 
		= settings.framerate == 0.0f ? 60.0f : settings.framerate;

	// 타겟 한 프레임 시간.
	float oneFrameTime = 1.0f / targetFrameRate;

	// GameLoop.
	while (true)
	{
		// 엔진 종료 여부 확인.
		if (isQuit)
		{
			// 루프 종료.
			break;
		}

		// 프레임 시간 계산.
		// (현재 시간 - 이전 시간) / 주파수 = 초단위.
		QueryPerformanceCounter(&currentTime);

		// 프레임 시간.
		float deltaTime = 
			(currentTime.QuadPart - previousTime.QuadPart)
			/ (float)frequency.QuadPart;

		// 입력은 최대한 빨리.
		input.ProcessInput();

		// 고정 프레임.
		if (deltaTime >= oneFrameTime)
		{
			BeginPlay();
			input.DispatchCallbacks();
			Tick(deltaTime);
			Render();

			// 시간 업데이트.
			previousTime = currentTime;

			// 현재 프레임의 입력을 기록.
			input.SavePreviouseKeyStates();

			// 이전 프레임에 추가 및 삭제 요청된 액터 처리.
			if (mainLevel)
			{
				mainLevel->ProcessAddAndDestroyActors();
			}

			// 레벨 전환 처리.
			if (nextLevel)
			{
				SafeDelete(mainLevel);
				mainLevel = nextLevel;
				nextLevel = nullptr;
			}

			// @Test.
			//CONSOLE_SCREEN_BUFFER_INFO info = {};
			//auto result = GetConsoleScreenBufferInfo(GetRenderer()->buffer, &info);
			//if (result)
			//{
			//	char title[100] = {};
			//	sprintf_s(title, 100, "dwSize: (%d, %d)", 
			//		info.dwSize.X, info.dwSize.Y);
			//	SetConsoleTitleA(title);
			//}
		}
	}

	// 정리(텍스트 색상 원래대로 돌려놓기).
	Utils::SetConsoleTextColor(
		FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
	);
}

void Engine::WriteToBuffer(
	const Vector2& position, 
	const char* image, 
	Color color, 
	int sortingOrder)
{
	// 문자열 길이.
	int length = static_cast<int>(strlen(image));
	int finalLength = length;

	// 최종 위치.
	Vector2 finalPosition = position;

	// @Incomplte: 화면 벗어난 글자만 빼는 로직 구현중.
	// 화면 왼쪽으로 벗어나는지 확인.
	char* finalImage = nullptr;
	bool originalImageChanged = false;
	if (position.x < 0)
	{
		// 정상 위치(양수 좌표)에 있는 글자 수 계산.
		finalLength = length + position.x;

		// @Incomplete: 음수가 나오면 안됨.
		if (finalLength < 0)
		{
			return;
		}

		// 메모리 할당/초기화.
		finalImage = new char[finalLength + 1];
		memset(finalImage, 0, finalLength + 1);

		// 필요한 문자만 복사.
		strcpy_s(finalImage, finalLength + 1, image + (length - finalLength));

		// 최종 위치 보정.
		finalPosition.x = 0;

		// 원래 이미지(문자열) 값이 변경됐다고 표시.
		originalImageChanged = true;
	}

	// @Incomplete: 화면 벗어난 글자만 빼는 로직 작성중.
	// 화면 오른쪽으로 벗어나는지 확인.
	if (position.x + length > settings.width)
	{
		// 정상 위치(화면 안쪽)에 있는 글자 수 계산.
		finalLength = settings.width - position.x;

		// @Incomplete: 음수가 나오면 안됨.
		if (finalLength < 0)
		{
			return;
		}

		// 메모리 할당/초기화.
		finalImage = new char[finalLength + 1];
		memset(finalImage, 0, finalLength + 1);

		// 필요한 문자만 복사.
		memcpy(finalImage, image, finalLength);

		// 원래 이미지(문자열) 값이 변경됐다고 표시.
		originalImageChanged = true;
	}

	// 문자열 기록.
	for (int ix = 0; ix < finalLength; ++ix)
	{
		// 기록할 문자 위치.
		// @Incomplete: 화면 버퍼 크기 안 넘어가게 예외처리 필요함.
		int index = (position.y * (settings.width)) + finalPosition.x + ix;

		// 그리려는 대상의 sortingOrder가 현재 그려져있는 글자의 값보다 작으면 안그리기. 
		// (글자가 겹치는 경우의 처리).
		if (imageBuffer->sortingOrderArray[index] > sortingOrder)
		{
			continue;
		}

		// 버퍼에 문자 기록.
		// 원래 이미지 값이 변경된 경우에는 변경된 문자열을 버퍼에 기록.
		if (originalImageChanged)
		{
			imageBuffer->charInfoArray[index].Char.AsciiChar = finalImage[ix];
		}

		// 문자열 변경이 없었다면 원래 문자열을 버퍼에 기록.
		else
		{
			imageBuffer->charInfoArray[index].Char.AsciiChar = image[ix];
		}
		
		// 색상 기록.
		imageBuffer->charInfoArray[index].Attributes = (WORD)color;
		
		// 뎊스 기록.
		imageBuffer->sortingOrderArray[index] = sortingOrder;
	}

	// finalImage를 생성한 경우, 메모리 해제.
	if (originalImageChanged && finalImage)
	{
		SafeDelete(finalImage);
	}
}

void Engine::AddLevel(Level* newLevel)
{
	// 기존에 있던 레벨은 제거.
	if (mainLevel)
	{
		delete mainLevel;
	}

	mainLevel = newLevel;
}

void Engine::ChangeLevel(Level* newLevel)
{
	this->nextLevel = newLevel;
}

void Engine::CleanUp()
{
	// 레벨 삭제.
	SafeDelete(mainLevel);

	// 문자 버퍼 삭제.
	//SafeDeleteArray(imageBuffer);
	//SafeDeleteArray(charInfo);
	SafeDelete(imageBuffer);

	// 렌더 타겟 삭제.
	SafeDelete(renderTargets[0]);
	SafeDelete(renderTargets[1]);
}

void Engine::Quit()
{
	// 종료 플래그 설정.
	isQuit = true;
}

Engine& Engine::Get()
{
	return *instance;
}

int Engine::Width() const
{
	return settings.width;
}

int Engine::Height() const
{
	return settings.height;
}

void Engine::OnInitialized()
{

}

void Engine::DisableToResizeWindow()
{
	// 콘솔 창 핸들 가져오기.
	HWND window = GetConsoleWindow();

	// 콘솔 창에 설정된 스타일 값 가져오기.
	LONG style = GetWindowLong(window, GWL_STYLE);

	// 콘솔 창 스타일에서 크기 조절 관련 옵션 제거.
	style &= ~WS_MAXIMIZEBOX;
	style &= ~WS_SIZEBOX;

	// 콘솔창에 변경된 스타일 적용.
	SetWindowLongW(window, GWL_STYLE, style);
}

void Engine::BeginPlay()
{
	if (mainLevel)
	{
		mainLevel->BeginPlay();
	}
}

void Engine::Tick(float deltaTime)
{
	// 레벨 업데이트.
	if (mainLevel)
	{
		mainLevel->Tick(deltaTime);
	}
}

void Engine::Clear()
{
	ClearImageBuffer();
	GetRenderer()->Clear();
}

void Engine::Render()
{
	// 화면 지우기.
	Clear();

	// 레벨 그리기.
	if (mainLevel)
	{
		mainLevel->Render();
	}

	// 백버퍼에 데이터 쓰기.
	//GetRenderer()->Render(imageBuffer);
	GetRenderer()->Render(imageBuffer->charInfoArray);

	// 버퍼 교환.
	Present();
}

void Engine::Present()
{
	// 버퍼 교환.
	SetConsoleActiveScreenBuffer(GetRenderer()->buffer);

	// 인덱스 뒤집기. 1->0, 0->1.
	currentRenderTargetIndex = 1 - currentRenderTargetIndex;
}

void Engine::PresentImmediately()
{
	// 백버퍼에 데이터 쓰기.
	//GetRenderer()->Render(imageBuffer);
	GetRenderer()->Render(imageBuffer->charInfoArray);

	Present();
}

void Engine::LoadEngineSettings()
{
	// 엔진 설정 파일 열기.
	FILE* file = nullptr;
	fopen_s(&file, "../Settings/EngineSettings.txt", "rt");
	if (file == nullptr)
	{
		std::cout << "Failed to load engine settings.\n";
		__debugbreak();
		return;
	}

	// 로드.

	// FP(File Position) 포인터를 가장 뒤로 옮기기.
	fseek(file, 0, SEEK_END);

	// 이 위치 구하기.
	size_t fileSize = ftell(file);

	// 다시 첫 위치로 되돌리기.
	//fseek(file, 0, SEEK_SET);
	rewind(file);

	// 파일 내용을 저장할 버퍼 할당.
	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);

	// 내용 읽기.
	size_t readSize = fread(buffer, sizeof(char), fileSize, file);

	// 파싱(Parcing, 구문 해석->필요한 정보를 얻는 과정).
	char* context = nullptr;
	char* token = nullptr;

	token = strtok_s(buffer, "\n", &context);

	// 구문 분석.
	while (token != nullptr)
	{
		// 키/값 분리.
		char header[10] = { };

		// 아래 구문이 제대로 동작하려면 키와 값 사이의 빈칸이 있어야 함.
		sscanf_s(token, "%s", header, 10);

		// 헤더 문자열 비교.
		if (strcmp(header, "framerate") == 0)
		{
			sscanf_s(token, "framerate = %f", &settings.framerate);
		}
		else if (strcmp(header, "width") == 0)
		{
			sscanf_s(token, "width = %d", &settings.width);
		}
		else if (strcmp(header, "height") == 0)
		{
			sscanf_s(token, "height = %d", &settings.height);
		}

		// 그 다음줄 분리.
		token = strtok_s(nullptr, "\n", &context);
	}


	// 버퍼 해제.
	SafeDeleteArray(buffer);

	// 파일 닫기.
	fclose(file);
}

ScreenBuffer* Engine::GetRenderer() const
{
	return renderTargets[currentRenderTargetIndex];
}

void Engine::ClearImageBuffer()
{
	// 글자 버퍼 덮어쓰기.
	for (int y = 0; y < settings.height; ++y)
	{
		for (int x = 0; x < settings.width; ++x)
		{
			int index = (y * (settings.width)) + x;
			CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
			buffer.Char.AsciiChar = ' ';
			buffer.Attributes = 0;
			imageBuffer->sortingOrderArray[index] = -1;
		}

		// 각 줄 끝에 개행 문자 추가.
		int index = (y * (settings.width)) + settings.width;
		CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
		buffer.Char.AsciiChar = '\n';
		buffer.Attributes = 0;
		imageBuffer->sortingOrderArray[index] = -1;
	}

	// 마지막에 널 문자 추가.
	int index = (settings.width) * settings.height + 1;
	CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
	buffer.Char.AsciiChar = '\0';
	buffer.Attributes = 0;
	imageBuffer->sortingOrderArray[index] = -1;
}