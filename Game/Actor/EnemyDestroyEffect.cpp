#include "EnemyDestroyEffect.h"

// ȿ�� ����� ����� ���ڿ� ������ (������ �ִϸ��̼� ������).
static const char* sequence[] =
{
	"  @  ",
	" @@  ",
	" @@@ ",
	"@@@@",
	"@@@@@",
	"  +1 ",
	"  +1 ",
	"  +1 ",
	"  +1 ",
};

EnemyDestroyEffect::EnemyDestroyEffect(const Vector2& position)
	: Actor(sequence[0], Color::Red, position)
{
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	timer.SetTargetTime(0.05f);
}

void EnemyDestroyEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}

	timer.Reset();
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	++currentSequenceIndex;

	SafeDelete(image);
	size_t length = strlen(sequence[currentSequenceIndex]) + 1;
	image = new char[length];
	strcpy_s(image, length, sequence[currentSequenceIndex]);
}