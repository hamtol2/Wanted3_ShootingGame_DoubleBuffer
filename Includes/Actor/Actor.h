#pragma once

#include "Core.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "RTTI.h"

// 전방 선언.
class Level;
class Engine_API Actor : public RTTI
{
	friend class Level;
	RTTI_DECLARATIONS(Actor, RTTI)

public:
	Actor(
		const char* image = "",
		Color color = Color::White,
		const Vector2& position = Vector2::Zero
	);
	virtual ~Actor();

	// 이벤트 함수.

	// 객체 생애주기(Lifetime)에 1번만 호출됨 (초기화 목적).
	virtual void BeginPlay();

	// 프레임 마다 호출 (반복성 작업/지속성이 필요한 작업).
	virtual void Tick(float deltaTime);

	// 그리기 함수.
	virtual void Render();

	// BeginPlay 호출 여부 확인.
	inline bool HasBeganPlay() const { return hasBeganPlay; }

	// 위치 설정/값 읽는 함수.
	void SetPosition(const Vector2& newPosition);
	Vector2 Position() const;

	// 문자열 길이 반환.
	int Width() const;

	// Sorting Order 설정.
	void SetSortingOrder(unsigned int sortingOrder);

	// 오너십 설정.
	void SetOwner(Level* newOwner);
	Level* GetOwner();

	// 충돌 확인 요청 함수 (간단한 AABB 로직).
	bool TestIntersect(const Actor* const other);

	// 액터의 이미지 값 변경 함수.
	void ChangeImage(const char* newImage);

	// 객체 삭제 함수.
	void Destroy();

	// 객체가 삭제되기 직전에 호출되는 함수.
	virtual void OnDestroy();

	// 수명 주기 설정 함수.
	void SetLifetime(float newLifetime);

	// 게임 종료 요청 함수.
	void QuitGame();

protected:
	// 개체의 위치.
	Vector2 position;

	// 그릴 값.
	//char image = ' ';
	char* image = nullptr;

	// 문자열 길이.
	int width = 0;

	// 텍스트 색상 값.
	Color color;

	// BeginPlay 호출이 되었는지 확인.
	bool hasBeganPlay = false;

	// 정렬 순서.
	unsigned int sortingOrder = 0;

	// 액터가 활성 상태인지 알려주는 변수.
	bool isActive = true;

	// 삭제 요청됐는지 알려주는 변수.
	bool isExpired = false;

	// 수명 주기 (단위: 초).
	float lifetime = 0.0f;

	// 이 플래그를 true로 설정하면 수명 주기를 사용해 자동 제거.
	bool autoDestroy = false;

	// 소유 레벨(오너십).
	Level* owner = nullptr;
};