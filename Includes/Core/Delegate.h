#pragma once

/*
* Delegate 클래스.
* - 클래스(또는 구조체) 메소드를 등록해 콜백 처리가 가능하도록 해주는 클래스.
* - 일반 함수는 메소드 등록 불가.
* - 일반 함수와/클래스(구조체)의 메소드를 모두 활용할 수 있게 하려면 구조가 복잡해져 제한을 둠.
*/

#include "Core.h"

// 델리게이트에서 사용할 함수 포인터 타입 선언.
using Function = void(*)(void*);

// 델리게이트 클래스.
struct Engine_API Delegate
{
	// 델리게이트에서 참조할 인스턴스.
	void* instance = nullptr;

	// 실행할 메소드 포인터.
	Function function = nullptr;

	Delegate() = default;

	Delegate(void* const instance, Function function)
		: instance(instance), function(function)
	{
	}

	// 델리게이트 호출 함수.
	void Invoke()
	{
		// 함수 호출 가능 여부 확인.
		if (!instance || !function)
		{
			return;
		}

		// 콜백 함수 호출.
		function(instance);
	}

	// 호출 연산자 오버로딩.
	void operator()()
	{
		Invoke();
	}
};