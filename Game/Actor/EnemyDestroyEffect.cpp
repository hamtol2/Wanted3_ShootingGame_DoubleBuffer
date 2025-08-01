#include "EnemyDestroyEffect.h"

// ȿ�� ����� ����� ���ڿ� ������ (������ �ִϸ��̼� ������).
static const EffectFrame sequence[] =
{
	EffectFrame("  @  ", 0.08f, Color::Red),
	EffectFrame(" @@  ", 0.08f, Color::Blue),
	EffectFrame(" @@@ ", 0.08f, Color::Green),
	EffectFrame("@@@@ ", 0.08f, Color::Red),
	EffectFrame("  +1 ", 0.5f, Color::Green)
};

EnemyDestroyEffect::EnemyDestroyEffect(const Vector2& position)
	: Actor(sequence[0].frame, Color::Red, position)
{
	// �ִϸ��̼� ������ ���� ���ϱ�.
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// ���� �ִϸ��̼Ǳ��� ����� �ð�.
	timer.SetTargetTime(sequence[0].playTime);

	// ���� ����.
	color = sequence[0].color;
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
	ChangeImage(sequence[currentSequenceIndex].frame);
	
	// ���� ����.
	color = sequence[currentSequenceIndex].color;
};