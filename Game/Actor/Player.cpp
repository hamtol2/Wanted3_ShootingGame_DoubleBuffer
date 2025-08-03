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

	// keydown �ݹ� ���.
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

	// �Է� ó��.
	//if (Input::Get().GetKeyDown(VK_ESCAPE))
	//{
	//	// ���� ����.
	//	QuitGame();
	//	return;
	//}

	// Ÿ�̸� ������Ʈ.
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
	// �÷��̾� ź�� ��ü ����.
	// x: �÷��̾��� ���.
	// y: �÷��̾�� ��ĭ ��.
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

	// ź�� ����.
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