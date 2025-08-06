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

	// 오른쪽 이동 함수.
	void MoveRight();

	// 왼쪽 이동 함수.
	void MoveLeft();

	// 한 발씩 발사하는 함수.
	void Fire();

	// 연사 모드 함수.
	void FireInterval();

	// 한 발씩 발사/연사 모드 변경 함수.
	void ChangeFireMode();

	// @Test: 클릭 이벤트 테스트 함수.
	void OnClick();

	// 게임 종료 함수.
	void Quit();

	// 연사 모드일 때 발사가 가능한지 확인하는 함수.
	bool CanShoot();

private:
	// 발사 모드.
	FireMode fireMode = FireMode::None;

	// 연사 간격 (단위: 초).
	float fireInterval = 0.15f;

	// 발사 간격 제어 타이머.
	Timer fireIntervaltimer;
};