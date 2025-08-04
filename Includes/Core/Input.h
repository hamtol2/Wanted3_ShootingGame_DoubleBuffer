#pragma once

#include "Core.h"
#include "Delegate.h"
#include <unordered_map>
#include <Windows.h>
#include <vector>

class Engine_API Input
{
	// friend 선언.
	friend class Engine;

	// 키입력 확인을 위한 구조체 선언.
	struct KeyState
	{
		// 현재 프레임에 키가 눌렸는지 여부.
		bool isKeyDown = false;

		// 이전 프레임에 키가 눌렸는지 여부.
		bool previouseKeyDown = false;
	};

public:
	Input();

	// 키 확인 함수.
	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	static Input& Get();

	// 키 눌림 이벤트에 리스너 함수를 등록할 때 사용할 함수.
	template<typename T, void (T::*Method)()>
	void RegisterKeydownEvent(int keyCode, T* object)
	{
		// 델리게이트를 전달한 객체가 Actor의 자손인지 확인.
		if (!object)
		{
			// Visual Studio 출력(Output) 창에 오류 메시지 출력.
			OutputDebugStringA("일반 함수는 메소드 등록이 불가능합니다.");

			// Debug 모드에서 일시정지시킬 때 사용하는 함수.
			__debugbreak();
		}

		// 델리게이트 객체 생성.
		Delegate delegate(object, [](void* instance)
			{
				// 형변환(Down-Casting) 후 메소드 호출.
				T* derived = static_cast<T*>(instance);
				(derived->*Method)();
			});

		// 콜백에 델리게이트 추가.
		keydownCallbacks[keyCode].emplace_back(delegate);
	}

	// 키 눌림 해제 이벤트에 리스너 함수를 등록할 때 사용할 함수.
	template<typename T, void (T::* Method)()>
	void RegisterKeyupEvent(int keyCode, T* object)
	{
		// 델리게이트를 전달한 객체가 Actor의 자손인지 확인.
		if (!object)
		{
			// Visual Studio 출력(Output) 창에 오류 메시지 출력.
			OutputDebugStringA("일반 함수는 메소드 등록이 불가능합니다.");

			// Debug 모드에서 일시정지시킬 때 사용하는 함수.
			__debugbreak();
		}

		// 델리게이트 객체 생성.
		Delegate delegate(object, [](void* instance)
			{
				// 형변환(Down-Casting) 후 메소드 호출.
				T* derived = static_cast<T*>(instance);
				(derived->*Method)();
			});

		// 콜백에 델리게이트 추가.
		keyupCallbacks[keyCode].emplace_back(delegate);
	}

	// 키 눌림 반복 이벤트에 리스너 함수를 등록할 때 사용할 함수.
	template<typename T, void (T::* Method)()>
	void RegisterKeyrepeatEvent(int keyCode, T* object)
	{
		// 델리게이트를 전달한 객체가 Actor의 자손인지 확인.
		if (!object)
		{
			// Visual Studio 출력(Output) 창에 오류 메시지 출력.
			OutputDebugStringA("일반 함수는 메소드 등록이 불가능합니다.");

			// Debug 모드에서 일시정지시킬 때 사용하는 함수.
			__debugbreak();
		}

		// 델리게이트 객체 생성.
		Delegate delegate(object, [](void* instance)
			{
				// 형변환(Down-Casting) 후 메소드 호출.
				T* derived = static_cast<T*>(instance);
				(derived->*Method)();
			});

		// 콜백에 델리게이트 추가.
		keyrepeatCallbacks[keyCode].emplace_back(delegate);
	}

private:
	void ProcessInput();
	void DispatchCallbacks();
	void SavePreviouseKeyStates();

private:

	// 키 입력 정보 관리 변수.
	KeyState keyStates[256] = { };

	// 키입력 관련 이벤트.
	std::unordered_map<int, std::vector<Delegate>> keydownCallbacks;
	std::unordered_map<int, std::vector<Delegate>> keyupCallbacks;
	std::unordered_map<int, std::vector<Delegate>> keyrepeatCallbacks;

	static Input* instance;
};