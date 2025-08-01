#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

/*
* �ۼ���: �弼��.
* EnemyDestroyEffect Ŭ����.
* Enemy�� �׾��� �� ���� ȿ���� ����� �� �����ϴ� ����.
*/

// �ִϸ��̼� ����Ʈ ������ ����ü.
struct EffectFrame
{
	EffectFrame(const char* frame, float playTime = 0.05f, Color color = Color::Red)
		: playTime(playTime), color(color)
	{
		// ���ڿ� ����.
		size_t length = strlen(frame) + 1;
		this->frame = new char[length];
		strcpy_s(this->frame, length, frame);
	}

	~EffectFrame()
	{
		SafeDelete(frame);
	}

	// ���ڿ� ���� (ȭ�鿡 ������ ���ڿ�).
	char* frame = nullptr;

	// ��� �ð�.
	float playTime = 0.0f;

	// ����.
	Color color = Color::White;
};

class EnemyDestroyEffect : public Actor
{
	RTTI_DECLARATIONS(EnemyDestroyEffect, Actor)

public:

	EnemyDestroyEffect(const Vector2& position);

	virtual void Tick(float deltaTime) override;

private:

	// ������ ���ڿ� ��(�迭 ���� ��).
	int effectSequenceCount = 0;

	// ���� �������� ������ �ε���.
	int currentSequenceIndex = 0;

	// �ִϸ��̼� ����� ����� Ÿ�̸�.
	// ������ ���̿� �ð� ����.
	Timer timer;
};