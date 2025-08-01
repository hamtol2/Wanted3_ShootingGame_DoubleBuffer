#include "EnemyBullet.h"
#include "Engine.h"

EnemyBullet::EnemyBullet(const Vector2& position, float moveSpeed)
	: Actor("O", Color::Blue, position), 
	yPosition(static_cast<float>(position.y)), 
	moveSpeed(moveSpeed)
{
	sortingOrder = 5;
}

void EnemyBullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 아래로 이동 처리.
	yPosition = yPosition + moveSpeed * deltaTime;
	
	// 화면 벗어났는지 확인.
	if (yPosition >= Engine::Get().Height())
	{
		Destroy();
		return;
	}
	
	// 벗어나지 않았으면 위치 설정.
	SetPosition(Vector2(position.x, (int)yPosition));
}