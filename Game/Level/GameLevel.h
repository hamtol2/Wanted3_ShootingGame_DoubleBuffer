#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"

/* ECS - 서로 모름.
* 게임 레벨의 책임.
* [x] 적생성: 약간의 랜덤성을 가미해서 일정 시간마다 적 생성.
* [x] 충돌처리: 플레이어-적 탄약 사이의 충돌/적-플레이어 탄약의 충돌 처리(AABB).
* [x] 점수 관리: 플레이어가 적을 없애면 1점씩 획득.
* [x] 게임 판정: 플레이어가 죽으면 게임 종료.
*/

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;
	
private:
	void SpawnEnemies(float deltaTime);
	
	void ProcessCollisionPlayerBulletAndEnemy();
	void ProcessCollisionPlayerAndEnemyBullet();

	void PrintMenu();

private:
	// 적 생성 시 시간 계산을 위한 타이머.
	Timer enemySpawnTimer;

	// 점수 변수.
	int score = 0;

	// 플레이어의 죽음 처리를 위한 변수.
	bool isPlayerDead = false;

	// 플레이어 죽은 위치.
	Vector2 playerDeadPosition;
};