#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

/*
* �ۼ���: �弼��.
* EnemyDestroyEffect Ŭ����.
* Enemy�� �׾��� �� ���� ȿ���� ����� �� �����ϴ� ����.
*/
class EnemyDestroyEffect : public Actor
{
	RTTI_DECLARATIONS(EnemyDestroyEffect, Actor)

public:

	EnemyDestroyEffect(const Vector2& position);

	virtual void Tick(float deltaTime) override;

private:

	// ������ ���ڿ� ��(�迭 ���� ��).
	int effectSequenceCount = 0;

	// ���� �������� ������ �ε���.
	int currentSequenceIndex = 0;

	// �ִϸ��̼� ����� ����� Ÿ�̸�.
	// ������ ���̿� �ð� ����.
	Timer timer;
};