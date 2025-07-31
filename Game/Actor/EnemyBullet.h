#pragma once

/*
* EnemyBullet.
* �ۼ���: �弼��. 2025.07.31.
* - ���� �߻��ϴ� ź��.
* - ������ �Ʒ� �������� �̵�.
* - �̵� �����⸦ ������, �÷��̾�� �浹�ϰų� ȭ���� ����� ���ŵ�.
*/

#include "Actor/Actor.h"

class EnemyBullet : public Actor
{
	RTTI_DECLARATIONS(EnemyBullet, Actor)
public:
	EnemyBullet(const Vector2& position, float moveSpeed = 15.0f);

	virtual void Tick(float deltaTime) override;

private:
	// �̵� ó���� ���� ����.
	float moveSpeed = 0.0f;

	// �ε� �Ҽ������� �̵� ��ġ�� ����� �� ����� ����.
	float yPosition = 0.0f;
};