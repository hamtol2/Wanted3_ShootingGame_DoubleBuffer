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
	fireIntervaltimer.SetTargetTime(fireInterval);

	// keydown �ݹ� ���.
	Input::Get().RegisterKeydownEvent<Player, &Player::Quit>(VK_ESCAPE, this);
	Input::Get().RegisterKeydownEvent<Player, &Player::Fire>(VK_SPACE, this);
	Input::Get().RegisterKeydownEvent<Player, &Player::ChangeFireMode>('R', this);

	// @Test: ���콺 Ŭ�� �׽�Ʈ.
	//Input::Get().RegisterKeydownEvent<Player, &Player::OnClick>(VK_LBUTTON, this);
	//Input::Get().RegisterKeyupEvent<Player, &Player::OnClick>(VK_LBUTTON, this);

	// keyrepeat �ݹ� ���.
	Input::Get().RegisterKeyrepeatEvent<Player, &Player::FireInterval>(VK_SPACE, this);
	Input::Get().RegisterKeyrepeatEvent<Player, &Player::MoveLeft>(VK_LEFT, this);
	Input::Get().RegisterKeyrepeatEvent<Player, &Player::MoveRight>(VK_RIGHT, this);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//if (Input::Get().GetKey(VK_LBUTTON))
	//{
	//	//__debugbreak();
	//	OnClick();
	//}
	
	// ���� Ÿ�̸� ������Ʈ.
	fireIntervaltimer.Tick(deltaTime);
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
	// �߻� ���� ���� Ȯ��.
	// ����� �ð��� �������� & ���� ��� ����.
	if (!CanShoot() || fireMode != FireMode::Repeat)
	{
		return;
	}

	// Ÿ�̸� ����.
	fireIntervaltimer.Reset();

	// ź�� ����.
	Vector2 bulletPosition(position.x + width / 2, position.y);
	owner->AddActor(new PlayerBullet(bulletPosition));
}

void Player::ChangeFireMode()
{
	// �ܹ� ��� / ���� ��� ��ȯ.
	int mode = (int)fireMode;
	mode = 1 - mode;

	fireMode = (FireMode)mode;
}

void Player::OnClick()
{
	//OutputDebugStringA("���콺 Ŭ����\n");

	Vector2 cursorPosition = Input::Get().MousePosition();
	SetPosition(cursorPosition);
}

void Player::Quit()
{
	QuitGame();
}

bool Player::CanShoot()
{
	// ���� ��忡�� �߻簡 ������ ����� �ð��� �������� Ȯ��.
	return fireIntervaltimer.IsTimeout();
}