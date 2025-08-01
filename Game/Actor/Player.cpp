#include "Player.h"
#include "Input.h"
#include "Utils/Utils.h"
#include "Level/Level.h"
#include "Engine.h"

#include "Actor/PlayerBullet.h"

#include <Windows.h>
#include <iostream>

Player::Player(float fireInterval, FireMode fireMode)
	: Actor("<<A>>", Color::Red), 
	fireInterval(fireInterval), fireMode(fireMode)
{
	// 시작 위치 (화면의 가운데, 가장 아래쪽).
	int xPosition = Engine::Get().Width() / 2 - width / 2 - 1;
	int yPosition = Engine::Get().Height() - 2;
	SetPosition(Vector2(xPosition, yPosition));

	// 타이머 시간 설정.
	timer.SetTargetTime(fireInterval);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 게임 종료.
		QuitGame();
		return;
	}

	// 미사일 발사.
	if (fireMode == FireMode::OneShot)
	{
		if (Input::Get().GetKeyDown(VK_SPACE))
		{
			// 발사.
			Fire();
		}
	}
	else if (fireMode == FireMode::Repeat)
	{
		if (Input::Get().GetKey(VK_SPACE))
		{
			// 발사.
			FireInterval(deltaTime);
		}
	}

	// 방향키 입력.
	if (Input::Get().GetKey(VK_LEFT))
	{
		Vector2 position = Position();
		position.x -= 1;
		SetPosition(position);
	}

	if (Input::Get().GetKey(VK_RIGHT))
	{
		Vector2 position = Position();
		position.x += 1;
		SetPosition(position);
	}

	// 연사 모드 전환.
	if (Input::Get().GetKeyDown('R'))
	{
		int mode = (int)fireMode;
		mode = 1 - mode;

		fireMode = (FireMode)mode;
	}
}

void Player::Fire()
{
	// 플레이어 탄약 객체 생성.
	// x: 플레이어의 가운데.
	// y: 플레이어에서 한칸 위.
	Vector2 bulletPosition(position.x + width / 2, position.y);
	owner->AddActor(new PlayerBullet(bulletPosition));
}

void Player::FireInterval(float deltaTime)
{
	// 타이머 업데이트.
	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}

	timer.Reset();

	// 탄약 생성.
	Vector2 bulletPosition(position.x + width / 2, position.y);
	owner->AddActor(new PlayerBullet(bulletPosition));
}