#include "Enemy.h"
#include "Utils/Utils.h"
#include "Engine.h"
#include "Level/Level.h"
#include "Actor/EnemyBullet.h"
#include "Actor/EnemyDestroyEffect.h"

Enemy::Enemy(const char* image, int yPosition)
	: Actor(image)
{
	// �̵� ������ ���ϱ� ���� ����.
	int random = Utils::Random(1, 10);

	// ¦Ȧ ��.
	if (random % 2 == 0)
	{
		// ȭ�� ������ ������ �����ǵ��� ��ġ / �̵� ���� ����.
		direction = MoveDirection::Left;
		xPosition = static_cast<float>(Engine::Get().Width()) - width - 1;
	}
	else
	{
		// ȭ�� ���� ������ �����ǵ��� ��ġ / �̵� ���� ����.
		direction = MoveDirection::Right;
		xPosition = 0.0f;
	}

	// ������ ��ġ ����.
	position.x = static_cast<int>(xPosition);
	position.y = yPosition;

	// �߻� ���� �ð� ����.
	timer.SetTargetTime(Utils::RandomFloat(1.0f, 3.0f));
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �̵� (�¿�).
	float dir = direction == MoveDirection::Left ? -1.0f : 1.0f;

	// �̵� ó��(���ӵ� �, �̵��Ÿ� = �ӵ� x �ð�; �ӵ� -> ������ x ����.
	xPosition = xPosition + moveSpeed * dir * deltaTime;

	// ȭ�� �ۿ� ������� Ȯ��.
	//if (xPosition + width - 1 < 0.0f || (int)xPosition > Engine::Get().Width() - width)
	if ((int)xPosition + width < 0
		|| (int)xPosition > Engine::Get().Width() - 1)
	{
		// ȭ�鿡�� ����� ���� ����.
		Destroy();
		return;
	}

	// �� ������� ��ġ ����.
	SetPosition(Vector2((int)xPosition, position.y));

	// �ð� �� �Ŀ� ź�� �߻�.
	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}

	// �߻�.
	// �� ź�� ���� ��û.
	owner->AddActor(new EnemyBullet(
		Vector2(position.x + width / 2, position.y),
		Utils::RandomFloat(10.0f, 20.0f))
	);

	// Ÿ�̸� ���� ����.
	timer.Reset();
	//timer.SetTargetTime(Utils::RandomFloat(1.0f, 3.0f));
	timer.SetTargetTime(10000.0f);
}

void Enemy::OnDestroy()
{
	super::OnDestroy();

	owner->AddActor(new EnemyDestroyEffect(position));
}