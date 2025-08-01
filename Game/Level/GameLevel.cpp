#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/PlayerBullet.h"
#include "Actor/EnemyBullet.h"

#include "Engine.h"

#include "Utils/Utils.h"

#include <iostream>

// �� ������ �� ����� ���� ��.
// ���⿡�� static�� private��.
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
	// �÷��̾� �߰�.
	Player* player = new Player();
	playerWidth = player->Width();
	AddActor(player);

	// Ÿ�̸� ����.
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

	// �� ����.
	SpawnEnemies(deltaTime);

	// �÷��̾� ź��� ���� �浹 ó��.
	ProcessCollisionPlayerBulletAndEnemy();

	// ���� ź��� �÷��̾��� �浹 ó��.
	ProcessCollisionPlayerAndEnemyBullet();
}

void GameLevel::SpawnEnemies(float deltaTime)
{
	// �� ����.
	enemySpawnTimer.Tick(deltaTime);

	// Ÿ�� �ƿ� Ȯ��.
	if (!enemySpawnTimer.IsTimeout())
	{
		return;
	}

	// Ÿ�̸� ����.
	enemySpawnTimer.Reset();
	enemySpawnTimer.SetTargetTime(Utils::RandomFloat(0.5f, 1.5f));

	// �� ���� ����.
	// �迭 ���� ���ϱ�.
	static int length = sizeof(enemyType) / sizeof(enemyType[0]);

	// �迭 �ε��� �������� ���ϱ�.
	int index = Utils::Random(0, length - 1);

	// ���� ������ y ��ġ �� �������� ���ϱ�.
	int yPosition = Utils::Random(1, 10);

	// �� ���� ����.
	AddActor(new Enemy(enemyType[index], yPosition));
}

void GameLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// �÷��̾� ź�� ���� �迭.
	std::vector<PlayerBullet*> bullets;
	std::vector<Enemy*> enemies;

	// �� Ÿ���� ���͸� �˻��ؼ� �迭 ä���.
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

	// ����ó�� (���ص� ��Ȳ Ȯ��).
	if (bullets.size() == 0 || enemies.size() == 0)
	{
		return;
	}

	// �浹 ó��.
	for (auto* bullet : bullets)
	{
		for (auto* enemy : enemies)
		{
			// �� ���Ͱ� ���� ���ƴ��� Ȯ��.
			if (bullet->TestIntersect(enemy))
			{
				enemy->Destroy();
				bullet->Destroy();

				// ������ ȹ���߱� ������ ���� ���� ó���ؾ� ��.
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
		// �� ź������ Ȯ�� �� �迭�� �߰�.
		EnemyBullet* bullet = actor->As<EnemyBullet>();
		if (bullet)
		{
			bullets.emplace_back(bullet);
			continue;
		}

		// �÷��̾� Ȯ��.
		if (!player)
		{
			player = actor->As<Player>();
		}
	}

	// ���ص� �Ǵ� ��Ȳ Ȯ��.
	if (bullets.size() == 0 || !player)
	{
		return;
	}

	// �浹 Ȯ��.
	for (auto* bullet : bullets)
	{
		if (player->TestIntersect(bullet))
		{
			// ���� ó�� (���� ����).
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
	// ���ھ� �����ֱ�.
	// Score: 0 �̷����� ���ڿ� �����.
	char buffer[20] = { };
	sprintf_s(buffer, 20, "Score: %d", score);

	// ���.
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

	// ���� ���� �� ó��.
	if (isPlayerDead)
	{
		// ������ �׸� ��ġ�� ȭ�� ���� �Ѿ�� �ʵ��� ����.
		// �Ʒ� ���ڿ� �߿��� ���̰� ���� �� ���ڿ��� �������� ��ġ ����.
		int longestStringLength = (int)strlen("Game Over!");
		int x = playerDeadPosition.x + longestStringLength > Engine::Get().Width() ?
			Engine::Get().Width() - longestStringLength : playerDeadPosition.x;
		int y = playerDeadPosition.y;
		Engine::Get().WriteToBuffer(Vector2(x, y - 3), "   .   ", Color::Red);
		Engine::Get().WriteToBuffer(Vector2(x, y - 2), " .  .  .", Color::Red);
		Engine::Get().WriteToBuffer(Vector2(x, y - 1), "..:V:..", Color::Red);
		Engine::Get().WriteToBuffer(Vector2(x, y), "Game Over!", Color::Red);

		// ���ھ� �����ֱ�.
		// Score: 0 �̷����� ���ڿ� �����.
		ShowGameScore();

		// ������ ��� ��û�� ���ڸ� �ٷ� ȭ�鿡 ���̵��� �Լ� ȣ��.
		Engine::Get().PresentImmediately();

		// ��� ����(�뷫 2��).
		Sleep(2000);
		Engine::Get().Quit();
	}

	// ���ھ� �����ֱ�.
	ShowGameScore();
}