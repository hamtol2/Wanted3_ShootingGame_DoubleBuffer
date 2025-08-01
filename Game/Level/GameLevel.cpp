#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/PlayerBullet.h"
#include "Actor/EnemyBullet.h"

#include "Engine.h"

#include "Utils/Utils.h"

#include <iostream>

// 적 생성할 때 사용할 글자 값.
// 여기에서 static은 private임.
static const char* enemyType[] =
{
	";:^:;",
	"zZwZz",
	"oO@Oo",
	"<-=->",
	")qOp(",
};

GameLevel::GameLevel()
{
	// 플레이어 추가.
	Player* player = new Player();
	playerWidth = player->Width();
	AddActor(player);

	// 타이머 설정.
	enemySpawnTimer.SetTargetTime(Utils::RandomFloat(2.0f, 3.0f));
}

GameLevel::~GameLevel()
{
}

void GameLevel::BeginPlay()
{
	super::BeginPlay();
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 적 생성.
	SpawnEnemies(deltaTime);

	// 플레이어 탄약과 적의 충돌 처리.
	ProcessCollisionPlayerBulletAndEnemy();

	// 적의 탄약과 플레이어의 충돌 처리.
	ProcessCollisionPlayerAndEnemyBullet();
}

void GameLevel::SpawnEnemies(float deltaTime)
{
	// 적 생성.
	enemySpawnTimer.Tick(deltaTime);

	// 타임 아웃 확인.
	if (!enemySpawnTimer.IsTimeout())
	{
		return;
	}

	// 타이머 정리.
	enemySpawnTimer.Reset();
	enemySpawnTimer.SetTargetTime(Utils::RandomFloat(0.5f, 1.5f));

	// 적 생성 로직.
	// 배열 길이 구하기.
	static int length = sizeof(enemyType) / sizeof(enemyType[0]);

	// 배열 인덱스 랜덤으로 구하기.
	int index = Utils::Random(0, length - 1);

	// 적을 생성할 y 위치 값 랜덤으로 구하기.
	int yPosition = Utils::Random(1, 10);

	// 적 액터 생성.
	AddActor(new Enemy(enemyType[index], yPosition));
}

void GameLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// 플레이어 탄약 액터 배열.
	std::vector<PlayerBullet*> bullets;
	std::vector<Enemy*> enemies;

	// 두 타입의 액터를 검색해서 배열 채우기.
	for (Actor* const actor : actors)
	{
		PlayerBullet* bullet = actor->As<PlayerBullet>();
		if (bullet)
		{
			bullets.emplace_back(bullet);
			continue;
		}

		Enemy* enemy = actor->As<Enemy>();
		if (enemy)
		{
			enemies.emplace_back(enemy);
		}
	}

	// 예외처리 (안해도 상황 확인).
	if (bullets.size() == 0 || enemies.size() == 0)
	{
		return;
	}

	// 충돌 처리.
	for (auto* bullet : bullets)
	{
		for (auto* enemy : enemies)
		{
			// 두 액터가 서로 겹쳤는지 확인.
			if (bullet->TestIntersect(enemy))
			{
				enemy->Destroy();
				bullet->Destroy();

				// 점수를 획득했기 때문에 점수 증가 처리해야 함.
				score = score + 1;
				continue;
			}
		}
	}
}

void GameLevel::ProcessCollisionPlayerAndEnemyBullet()
{
	Player* player = nullptr;
	std::vector<EnemyBullet*> bullets;

	for (Actor* const actor : actors)
	{
		// 적 탄약인지 확인 후 배열에 추가.
		EnemyBullet* bullet = actor->As<EnemyBullet>();
		if (bullet)
		{
			bullets.emplace_back(bullet);
			continue;
		}

		// 플레이어 확인.
		if (!player)
		{
			player = actor->As<Player>();
		}
	}

	// 안해도 되는 상황 확인.
	if (bullets.size() == 0 || !player)
	{
		return;
	}

	// 충돌 확인.
	for (auto* bullet : bullets)
	{
		if (player->TestIntersect(bullet))
		{
			// 죽음 처리 (게임 종료).
			isPlayerDead = true;
			playerDeadPosition.x = player->Position().x + player->Width() / 2;
			playerDeadPosition.y = player->Position().y;

			player->Destroy();
			bullet->Destroy();
			break;
		}
	}
}

void GameLevel::ShowGameScore()
{
	// 스코어 보여주기.
	// Score: 0 이런식의 문자열 만들기.
	char buffer[20] = { };
	sprintf_s(buffer, 20, "Score: %d", score);

	// 출력.
	Engine::Get().WriteToBuffer(Vector2(1, Engine::Get().Height() - 1), buffer);
}

void GameLevel::PrintMenu()
{
	static Vector2 position(Engine::Get().Width() / 2 - 5, Engine::Get().Height() - 1);
	Engine::Get().WriteToBuffer(position, "Move: Left Right");
	Engine::Get().WriteToBuffer(Vector2(position.x + 17, position.y), "Fire: Space");
}

void GameLevel::Render()
{
	super::Render();

	//PrintMenu();

	// 게임 종료 시 처리.
	if (isPlayerDead)
	{
		// 다음에 그릴 위치가 화면 밖을 넘어가지 않도록 보정.
		// 아래 문자열 중에서 길이가 가장 긴 문자열을 기준으로 위치 보정.
		int longestStringLength = (int)strlen("Game Over!");
		int x = playerDeadPosition.x + longestStringLength > Engine::Get().Width() ?
			Engine::Get().Width() - longestStringLength : playerDeadPosition.x;
		int y = playerDeadPosition.y;
		Engine::Get().WriteToBuffer(Vector2(x, y - 3), "   .   ", Color::Red);
		Engine::Get().WriteToBuffer(Vector2(x, y - 2), " .  .  .", Color::Red);
		Engine::Get().WriteToBuffer(Vector2(x, y - 1), "..:V:..", Color::Red);
		Engine::Get().WriteToBuffer(Vector2(x, y), "Game Over!", Color::Red);

		// 스코어 보여주기.
		// Score: 0 이런식의 문자열 만들기.
		ShowGameScore();

		// 위에서 출력 요청한 글자를 바로 화면에 보이도록 함수 호출.
		Engine::Get().PresentImmediately();

		// 잠깐 정지(대략 2초).
		Sleep(2000);
		Engine::Get().Quit();
	}

	// 스코어 보여주기.
	ShowGameScore();
}