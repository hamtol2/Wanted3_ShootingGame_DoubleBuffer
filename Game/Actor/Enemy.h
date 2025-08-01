#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

/*
* �̵� ���� (�¿�).
* - �����ʿ��� �¾���� �������� �̵�.
* - ���ʿ��� �¾���� ���������� �̵�.
*/
class Enemy : public Actor
{
	// �̵� ���� ������.
	enum class MoveDirection
	{
		None = -1,
		Left,
		Right
	};

	RTTI_DECLARATIONS(Enemy, Actor)

public:
	// �� ĳ���Ͱ� ������ �̹���(���ڿ�)�� ���� y��ġ.
	Enemy(const char* image = "<-=->", int yPosition = 5);

	virtual void Tick(float deltaTime) override;

	virtual void OnDestroy() override;

	// �÷��̾� ź�࿡ �¾��� �� ȣ���� �Լ�.
	void OnDamaged();

private:

	// �̵� ����.
	MoveDirection direction = MoveDirection::None;

	// �ӷ��� ����� �̵� ��ġ ����� ���� float ����.
	float xPosition = 0.0f;

	// �̵� ������.
	float moveSpeed = 5.0f;

	// Ÿ�̸�.
	Timer timer;

	// Ÿ�̸� ����� ���� ����.
	// ��� �ð� ��� ����.
	//float elapsedTime = 0.0f;

	// ��ǥ �ð� ��. (setTime, targetTime).
	//float targetTime = 0.0f;
};