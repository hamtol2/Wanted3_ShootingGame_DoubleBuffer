#pragma once

#include "Actor/Actor.h"

/*
* 플레이어가 발사하는 탄약.
* - 화면 위로 이동.
*   - 속력(빠르기, 초 단위), 위 방향
*/
class PlayerBullet : public Actor
{
	RTTI_DECLARATIONS(PlayerBullet, Actor)

public:
	PlayerBullet(const Vector2& position);

	virtual void Tick(float deltaTime) override;

private:
	// 초당 5칸.
	float moveSpeed = 30.0f;

	// 좌표가 정수이기 때문에 사이 값 계산을 위해 float 변수 선언.
	float yPosition = 0.0f;
};