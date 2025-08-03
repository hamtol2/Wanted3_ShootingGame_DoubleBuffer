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

	// keydown 콜백 등록.
	Input::Get().RegisterKeydownCallback<Player, &Player::Quit>(VK_ESCAPE, this);
	Input::Get().RegisterKeydownCallback<Player, &Player::Fire>(VK_SPACE, this);
	Input::Get().RegisterKeydownCallback<Player, &Player::ChangeFireMode>('R', this);

	Input::Get().RegisterKeyrepeatCallback<Player, &Player::FireInterval>(VK_SPACE, this);
	Input::Get().RegisterKeyrepeatCallback<Player, &Player::MoveLeft>(VK_LEFT, this);
	Input::Get().RegisterKeyrepeatCallback<Player, &Player::MoveRight>(VK_RIGHT, this);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리.
	//if (Input::Get().GetKeyDown(VK_ESCAPE))
	//{
	//	// 게임 종료.
	//	QuitGame();
	//	return;
	//}

	// 타이머 업데이트.
	timer.Tick(deltaTime);
}

void Player::MoveRight()
{
	Vector2 position = Position();
	position.x += 1;

	if (position.x + width < Engine::Get().Width())
	{
		SetPosition(position);
	}
}

void Player::MoveLeft()
{
	Vector2 position = Position();
	position.x -= 1;

	if (position.x >= 0)
	{
		SetPosition(position);
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

void Player::FireInterval()
{
	if (!CanShoot() || fireMode != FireMode::Repeat)
	{
		return;
	}

	timer.Reset();

	// 탄약 생성.
	Vector2 bulletPosition(position.x + width / 2, position.y);
	owner->AddActor(new PlayerBullet(bulletPosition));
}

void Player::ChangeFireMode()
{
	int mode = (int)fireMode;
	mode = 1 - mode;

	fireMode = (FireMode)mode;
}

void Player::Quit()
{
	QuitGame();
}

bool Player::CanShoot()
{
	return timer.IsTimeout();
}