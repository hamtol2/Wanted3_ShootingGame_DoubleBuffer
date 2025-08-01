#pragma once

#include "Core.h"
#include "Input.h"
#include "Math/Color.h"
#include "Math/Vector2.h"

#include <Windows.h>

// 엔진 설정 구조체.
struct EngineSettings
{
	// 콘솔 화면 가로 크기.
	int width = 0;

	// 콘솔 화면 세로 크기.
	int height = 0;

	// 타겟 프레임 속도.
	float framerate = 0.0f;
};

class Level;
class ScreenBuffer;
class Engine_API Engine
{
public:

	// 생성자.
	Engine();

	// 소멸자.
	virtual ~Engine();

	// 엔진 실행 함수.
	void Run();

	// 문자열 그리기 요청 함수.
	void WriteToBuffer(const Vector2& position, const char* image, Color color = Color::White);

	// 버퍼를 곧바로 교환 요청할 때 사용하는 함수.
	void PresentImmediately();

	// 레벨 추가 함수.
	void AddLevel(Level* newLevel);

	// 메모리 해제 함수.
	virtual void CleanUp();

	// 종료 함수.
	void Quit();

	// 싱글톤 접근 함수.
	static Engine& Get();

	// 화면 가로/세로 크기 반환 함수.
	int Width() const;
	int Height() const;

protected:

	// 엔진/게임 초기화 시 사용 (레벨 추가 등).
	virtual void OnInitialized();

	void BeginPlay();
	void Tick(float deltaTime = 0.0f);

	// 화면 지우는 함수 (전체를 빈 문자열로 설정).
	void Clear();

	// 액터 그리기 함수 (백버퍼에 기록).
	void Render();

	// 버퍼 교환 함수 (프론트 버퍼 <-> 백버퍼)
	void Present();

	// 엔진 설정 로드 함수.
	void LoadEngineSettings();

	// 백버퍼 렌더 타겟 반환 함수.
	ScreenBuffer* GetRenderer() const;

	// 글자 버퍼 지우는 함수.
	void ClearImageBuffer();


protected:
	// 엔진 종료 플래그.
	bool isQuit = false;

	// 메인 레벨.
	Level* mainLevel = nullptr;

	// 입력 관리자.
	Input input;

	// 엔진 설정.
	EngineSettings settings;

	// 백버퍼(프레임).
	CHAR_INFO* imageBuffer = nullptr;

	// 이중 버퍼.
	ScreenBuffer* renderTargets[2] = { };

	// 백버퍼로 사용하는 렌더 타겟의 인덱스.
	int currentRenderTargetIndex = 0;

	// 싱글톤 변수.
	static Engine* instance;
};