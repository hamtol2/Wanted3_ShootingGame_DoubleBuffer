#include "Actor.h"
#include "Utils/Utils.h"
#include "Engine.h"
#include "Level/Level.h"

#include <Windows.h>
#include <iostream>

Actor::Actor(const char* image, Color color, const Vector2& position)
	: color(color), position(position)
{
	// 문자열 길이.
	width = (int)strlen(image);

	// 메모리 할당.
	this->image = new char[width + 1];

	// 문자열 복사.
	strcpy_s(this->image, width + 1, image);
}

Actor::~Actor()
{
	// 메모리 해제.
	SafeDeleteArray(image);
}

// 객체 생애주기(Lifetime)에 1번만 호출됨 (초기화 목적).
void Actor::BeginPlay()
{
	hasBeganPlay = true;
}

// 프레임 마다 호출 (반복성 작업/지속성이 필요한 작업).
void Actor::Tick(float deltaTime)
{
}

// 그리기 함수.
void Actor::Render()
{
	// 커서 이동.
	//Utils::SetConsolePosition(position);

	// 색상 설정.
	//Utils::SetConsoleTextColor(color);

	// 그리기.
	//std::cout << image;

	// 엔진이 관리하는 이미지 버퍼에 액터의 문자열/색상 기록.
	Engine::Get().WriteToBuffer(position, image, color);
}

void Actor::SetPosition(const Vector2& newPosition)
{
	// 예외처리 (화면 벗어났는지 확인).

	// 왼쪽 가장자리가 화면 왼쪽을 벗어났는지.
	//if (newPosition.x < 0)
	//{
	//	return;
	//}

	//// 오른쪽 가장자리가 화면 오른쪽을 벗어났는지.
	//if (newPosition.x + width > Engine::Get().Width())
	//{
	//	return;
	//}

	//// 위쪽 가장자리가 화면의 위를 벗어났는지
	//if (newPosition.y < 0)
	//{
	//	return;
	//}

	//// 화면 아래를 벗어났는지
	//if (newPosition.y - 1 > Engine::Get().Height())
	//{
	//	return;
	//}

	// 같으면 업데이트 안함.
	if (position == newPosition)
	{
		return;
	}

	//// 지울 위치 확인.
	//Vector2 direction = newPosition - position;
	//position.x = direction.x >= 0 ? position.x : position.x + width - 1;

	//// 커서 이동.
	//Utils::SetConsolePosition(position);

	//// 문자열 길이 고려해서 지울 위치 확인해야 함.
	//std::cout << ' ';

	position = newPosition;
}

Vector2 Actor::Position() const
{
	return position;
}

int Actor::Width() const
{
	return width;
}

void Actor::SetSortingOrder(unsigned int sortingOrder)
{
	this->sortingOrder = sortingOrder;
}

void Actor::SetOwner(Level* newOwner)
{
	owner = newOwner;
}

Level* Actor::GetOwner()
{
	return owner;
}

bool Actor::TestIntersect(const Actor* const other)
{
	// AABB(Axis Aligned Bounding Box).
	// Note: 현재 액터 구조 상 세로는 크기가 없음(크기가 1).
	//       따라서 가로의 최소/최대 위치만 더 고려하면 됨.

	// 이 액터의 x 좌표 정보.
	int xMin = position.x;
	int xMax = position.x + width - 1;

	// 충돌 비교할 다른 액터의 x 좌표 정보.
	int otherXMin = other->position.x;
	int otherXMax = other->position.x + other->width - 1;

	// 안겹치는 조건 확인.

	// 다른 액터의 왼쪽 좌표가 내 오른쪽 좌표보다 더 오른쪽에 있으면 안겹침.
	if (otherXMin > xMax)
	{
		return false;
	}

	// 다른 액터의 오른쪽 좌표가 내 왼쪽 좌표보다 더 왼쪽에 있으면 안겹침.
	if (otherXMax < xMin)
	{
		return false;
	}

	// y 좌표가 같은지 최종 확인.
	return position.y == other->position.y;
}

void Actor::ChangeImage(const char* newImage)
{
	// 기존 이미지 메모리 해제.
	SafeDelete(image);

	// 새로운 문자열 복사.
	size_t length = strlen(newImage) + 1;
	image = new char[length];
	strcpy_s(image, length, newImage);
}

void Actor::Destroy()
{
	// 중복 삭제 방지 처리.
	if (isExpired)
	{
		return;
	}

	// 삭제 요청 되었다고 설정.
	isExpired = true;

	// 레벨에 삭제 요청.
	owner->DestroyActor(this);

	// 죽음 요청됐다고 알림.
	OnDestroy();
}

void Actor::OnDestroy()
{

}

void Actor::SetLifetime(float newLifetime)
{
	// 입력값 확인.
	if (newLifetime <= 0.0f)
	{
		return;
	}

	// 수명 주기 설정.
	lifetime = newLifetime;

	// 자동 제거 옵션 활성화.
	autoDestroy = true;
}

void Actor::QuitGame()
{
	Engine::Get().Quit();
}