#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

/*
* ���
* - �¿� �̵� (Ű�Է� ���).
* - ź�� �߻�(���� ����) (Ű�Է� ���).
*   - �ܹ� Fire()
*   - ���� FireInterval(float deltaTime)
*/
class Player : public Actor
{
	// �߻� ���.
	// �ܹ�/����.
	enum class FireMode
	{
		None = -1,
		OneShot,
		Repeat
	};

	RTTI_DECLARATIONS(Player, Actor)
public:
	Player(float fireInterval = 0.15f, FireMode fireMode = FireMode::OneShot);

	virtual void Tick(float deltaTime) override;

private:
	void Fire();
	void FireInterval(float deltaTime);

private:
	// �߻� ���.
	FireMode fireMode = FireMode::None;

	// ���� ���� (����: ��).
	float fireInterval = 0.15f;

	// �߻� ���� ���� Ÿ�̸�.
	Timer timer;
};