#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

/*
* 기능
* - 좌우 이동 (키입력 기반).
* - 탄약 발사(액터 생성) (키입력 기반).
*   - 단발 Fire()
*   - 연사 FireInterval(float deltaTime)
*/
class Player : public Actor
{
	// 발사 모드.
	// 단발/연사.
	enum class FireMode
	{
		None = -1,
		OneShot,
		Repeat
	};

	RTTI_DECLARATIONS(Player, Actor)
public:
	Player(float fireInterval = 0.15f, FireMode fireMode = FireMode::OneShot);

	virtual void Tick(float deltaTime) override;

private:
	void Fire();
	void FireInterval(float deltaTime);

private:
	// 발사 모드.
	FireMode fireMode = FireMode::None;

	// 연사 간격 (단위: 초).
	float fireInterval = 0.15f;

	// 발사 간격 제어 타이머.
	Timer timer;
};