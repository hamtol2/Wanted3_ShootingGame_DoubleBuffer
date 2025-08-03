#pragma once

#include "Core.h"
#include "Delegate.h"
#include <unordered_map>
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

	template<typename T, void (T::*Method)()>
	void RegisterKeydownCallback(int keyCode, T* object)
	{
		static_assert(std::is_base_of<Actor, T>::value, "T는 Actor를 상속하는 객체여야 합니다.");

		Delegate delegate;
		delegate.instance = object;
		delegate.function = [](Actor* actor)
			{
				T* derived = static_cast<T*>(actor);
				(derived->*Method)();
			};

		keydownCallbacks.insert({ keyCode, delegate });
	}

	template<typename T, void (T::* Method)()>
	void RegisterKeyupCallback(int keyCode, T* object)
	{
		static_assert(std::is_base_of<Actor, T>::value, "T는 Actor를 상속하는 객체여야 합니다.");

		Delegate delegate;
		delegate.instance = object;
		delegate.function = [](Actor* actor)
			{
				T* derived = static_cast<T*>(actor);
				(derived->*Method)();
			};

		keyupCallbacks.insert({ keyCode, delegate });
	}

	template<typename T, void (T::* Method)()>
	void RegisterKeyrepeatCallback(int keyCode, T* object)
	{
		static_assert(std::is_base_of<Actor, T>::value, "T는 Actor를 상속하는 객체여야 합니다.");

		Delegate delegate;
		delegate.instance = object;
		delegate.function = [](Actor* actor)
			{
				T* derived = static_cast<T*>(actor);
				(derived->*Method)();
			};

		keyrepeatCallbacks.insert({ keyCode, delegate });
	}

private:
	void ProcessInput();
	void DispatchCallbacks();
	void SavePreviouseKeyStates();

private:

	// 키 입력 정보 관리 변수.
	KeyState keyStates[256] = { };

	std::unordered_map<int, Delegate> keydownCallbacks;
	std::unordered_map<int, Delegate> keyupCallbacks;
	std::unordered_map<int, Delegate> keyrepeatCallbacks;

	static Input* instance;
};