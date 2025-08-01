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
	// ���� ��ġ (ȭ���� ���, ���� �Ʒ���).
	int xPosition = Engine::Get().Width() / 2 - width / 2 - 1;
	int yPosition = Engine::Get().Height() - 2;
	SetPosition(Vector2(xPosition, yPosition));

	// Ÿ�̸� �ð� ����.
	timer.SetTargetTime(fireInterval);
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
	if (fireMode == FireMode::OneShot)
	{
		if (Input::Get().GetKeyDown(VK_SPACE))
		{
			// �߻�.
			Fire();
		}
	}
	else if (fireMode == FireMode::Repeat)
	{
		if (Input::Get().GetKey(VK_SPACE))
		{
			// �߻�.
			FireInterval(deltaTime);
		}
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

	// ���� ��� ��ȯ.
	if (Input::Get().GetKeyDown('R'))
	{
		int mode = (int)fireMode;
		mode = 1 - mode;

		fireMode = (FireMode)mode;
	}
}

void Player::Fire()
{
	// �÷��̾� ź�� ��ü ����.
	// x: �÷��̾��� ���.
	// y: �÷��̾�� ��ĭ ��.
	Vector2 bulletPosition(position.x + width / 2, position.y);
	owner->AddActor(new PlayerBullet(bulletPosition));
}

void Player::FireInterval(float deltaTime)
{
	// Ÿ�̸� ������Ʈ.
	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}

	timer.Reset();

	// ź�� ����.
	Vector2 bulletPosition(position.x + width / 2, position.y);
	owner->AddActor(new PlayerBullet(bulletPosition));
}