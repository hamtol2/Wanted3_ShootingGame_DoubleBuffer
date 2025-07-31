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
	// ���� ��ġ (ȭ���� ���, ���� �Ʒ���).
	int xPosition = Engine::Get().Width() / 2 - width / 2 - 1;
	int yPosition = Engine::Get().Height() - 2;
	SetPosition(Vector2(xPosition, yPosition));
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �Է� ó��.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// ���� ����.
		QuitGame();
		return;
	}

	// �̻��� �߻�.
	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		// �߻�.
		Fire();
	}

	// ����Ű �Է�.
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
	// �÷��̾� ź�� ��ü ����.
	// x: �÷��̾��� ���.
	// y: �÷��̾�� ��ĭ ��.
	Vector2 bulletPosition(position.x + width / 2, position.y - 1);
	owner->AddActor(new PlayerBullet(bulletPosition));
}

void Player::FireInterval(float deltaTime)
{
}