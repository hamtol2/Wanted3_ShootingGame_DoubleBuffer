#include "EnemyDestroyEffect.h"

// ȿ�� ����� ����� ���ڿ� ������ (������ �ִϸ��̼� ������).
static const EffectFrame sequence[] =
{
	EffectFrame("  @  ", 0.08f),
	EffectFrame(" @@  ", 0.08f),
	EffectFrame(" @@@ ", 0.08f),
	EffectFrame("@@@@ ", 0.08f),
	EffectFrame("  +1 ", 1.0f)
};

EnemyDestroyEffect::EnemyDestroyEffect(const Vector2& position)
	: Actor(sequence[0].frame, Color::Red, position)
{
	// �ִϸ��̼� ������ ���� ���ϱ�.
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// ���� �ִϸ��̼Ǳ��� ����� �ð�.
	timer.SetTargetTime(sequence[0].playTime);
}

void EnemyDestroyEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �ִϸ��̼� ����� ���� Ÿ�̸� ������Ʈ.
	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}
	
	// �ִϸ��̼� ��� �������� Ȯ��.
	// �������� ����.
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	// ������ ������Ʈ.
	++currentSequenceIndex;
	timer.Reset();

	// ���� ���������� ����� �ð����� Ÿ�̸� �缳��.
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// ������ ���� �̹��� ����.
	SafeDelete(image);

	// �ִϸ��̼� �����ӿ� ����� ���ڿ��� ���Ϳ� ����.
	size_t length = strlen(sequence[currentSequenceIndex].frame) + 1;
	image = new char[length];
	strcpy_s(image, length, sequence[currentSequenceIndex].frame);
}