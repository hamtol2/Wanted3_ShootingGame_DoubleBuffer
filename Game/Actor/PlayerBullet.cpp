#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(const Vector2& position)
	: Actor("#", Color::Green, position), yPosition((float)position.y)
{
}

void PlayerBullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 이동 처리.
	yPosition -= moveSpeed * deltaTime;

	Vector2 position = Position();
	//position.y = position.y -1.0f * moveSpeed * deltaTime;
	position.y = (int)yPosition;

	// 계산 값이 화면 위를 벗어나면 탄약 제거.
	if (yPosition < 0.0f)
	{
		Destroy();
		return;
	}

	SetPosition(position);
}