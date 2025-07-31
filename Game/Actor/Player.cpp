#include "Player.h"
#include "Input.h"
#include "Utils/Utils.h"
#include "Level/Level.h"
#include "Engine.h"

#include "Actor/PlayerBullet.h"

#include <Windows.h>
#include <iostream>

Player::Player()
	: Actor("<<A>>", Color::Red)
{
	// 시작 위치 (화면의 가운데, 가장 아래쪽).
	int xPosition = Engine::Get().Width() / 2 - width / 2 - 1;
	int yPosition = Engine::Get().Height() - 2;
	SetPosition(Vector2(xPosition, yPosition));
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
	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		// 발사.
		Fire();
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

	//if (Input::Get().GetKey(VK_UP))
	//{
	//	Vector2 position = Position();
	//	position.y -= 1;
	//	SetPosition(position);
	//}

	//if (Input::Get().GetKey(VK_DOWN))
	//{
	//	Vector2 position = Position();
	//	position.y += 1;
	//	SetPosition(position);
	//}
}

void Player::Fire()
{
	// 플레이어 탄약 객체 생성.
	// x: 플레이어의 가운데.
	// y: 플레이어에서 한칸 위.
	Vector2 bulletPosition(position.x + width / 2, position.y - 1);
	owner->AddActor(new PlayerBullet(bulletPosition));
}

void Player::FireInterval(float deltaTime)
{
}