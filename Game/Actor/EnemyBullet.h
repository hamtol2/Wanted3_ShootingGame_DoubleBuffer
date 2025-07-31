#pragma once

/*
* EnemyBullet.
* 작성자: 장세윤. 2025.07.31.
* - 적이 발사하는 탄약.
* - 위에서 아래 방향으로 이동.
* - 이동 빠르기를 가지며, 플레이어와 충돌하거나 화면을 벗어나면 제거됨.
*/

#include "Actor/Actor.h"

class EnemyBullet : public Actor
{
	RTTI_DECLARATIONS(EnemyBullet, Actor)
public:
	EnemyBullet(const Vector2& position, float moveSpeed = 15.0f);

	virtual void Tick(float deltaTime) override;

private:
	// 이동 처리를 위한 변수.
	float moveSpeed = 0.0f;

	// 부동 소수점으로 이동 위치를 계산할 때 사용할 변수.
	float yPosition = 0.0f;
};