#include "Enemy.h"
#include "Utils/Utils.h"
#include "Engine.h"
#include "Level/Level.h"
#include "Actor/EnemyBullet.h"
#include "Actor/EnemyDestroyEffect.h"

Enemy::Enemy(const char* image, int yPosition)
	: Actor(image)
{
	// 이동 방향을 정하기 위한 난수.
	int random = Utils::Random(1, 10);

	// 짝홀 비교.
	if (random % 2 == 0)
	{
		// 화면 오른쪽 끝에서 생성되도록 위치 / 이동 방향 설정.
		direction = MoveDirection::Left;
		xPosition = static_cast<float>(Engine::Get().Width()) - width - 1;
	}
	else
	{
		// 화면 왼쪽 끝에서 생성되도록 위치 / 이동 방향 설정.
		direction = MoveDirection::Right;
		xPosition = 0.0f;
	}

	// 액터의 위치 설정.
	position.x = static_cast<int>(xPosition);
	position.y = yPosition;

	// 발사 간격 시간 설정.
	timer.SetTargetTime(Utils::RandomFloat(1.0f, 3.0f));
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 이동 (좌우).
	float dir = direction == MoveDirection::Left ? -1.0f : 1.0f;

	// 이동 처리(동속도 운동, 이동거리 = 속도 x 시간; 속도 -> 빠르기 x 방향.
	xPosition = xPosition + moveSpeed * dir * deltaTime;

	// 화면 밖에 벗어났는지 확인.
	if (xPosition < 0.0f || (int)xPosition > Engine::Get().Width() - width)
	{
		// 화면에서 벗어나면 액터 제거.
		Destroy();
		return;
	}

	// 안 벗어났으면 위치 설정.
	SetPosition(Vector2((int)xPosition, position.y));

	// 시간 잰 후에 탄약 발사.
	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}

	// 발사.
	// 적 탄약 생성 요청.
	owner->AddActor(new EnemyBullet(
		Vector2(position.x + width / 2, position.y),
		Utils::RandomFloat(10.0f, 20.0f))
	);

	// 타이머 변수 정리.
	timer.Reset();
	timer.SetTargetTime(Utils::RandomFloat(1.0f, 3.0f));
}

void Enemy::OnDestroy()
{
	super::OnDestroy();

	owner->AddActor(new EnemyDestroyEffect(position));
}