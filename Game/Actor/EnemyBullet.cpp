#include "EnemyBullet.h"
#include "Engine.h"

EnemyBullet::EnemyBullet(const Vector2& position, float moveSpeed)
	: Actor("O", Color::Blue, position), 
	yPosition(static_cast<float>(position.y)), 
	moveSpeed(moveSpeed)
{
}

void EnemyBullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �Ʒ��� �̵� ó��.
	yPosition = yPosition + moveSpeed * deltaTime;

	// ȭ�� ������� Ȯ��.
	if (yPosition >= Engine::Get().Height())
	{
		Destroy();
		return;
	}

	// ����� �ʾ����� ��ġ ����.
	SetPosition(Vector2(position.x, (int)yPosition));
}