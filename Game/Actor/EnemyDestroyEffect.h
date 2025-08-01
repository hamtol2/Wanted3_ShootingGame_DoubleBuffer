#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

/*
* 작성자: 장세윤.
* EnemyDestroyEffect 클래스.
* Enemy가 죽었을 때 죽음 효과를 재생할 때 생성하는 액터.
*/
class EnemyDestroyEffect : public Actor
{
	RTTI_DECLARATIONS(EnemyDestroyEffect, Actor)

public:

	EnemyDestroyEffect(const Vector2& position);

	virtual void Tick(float deltaTime) override;

private:

	// 시퀀스 문자열 수(배열 원소 수).
	int effectSequenceCount = 0;

	// 현재 보여지는 시퀀스 인덱스.
	int currentSequenceIndex = 0;

	// 애니메이션 재생에 사용할 타이머.
	// 시퀀스 사이에 시간 계산용.
	Timer timer;
};