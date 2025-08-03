#pragma once

#include "Core.h"
#include "Delegate.h"
#include <unordered_map>
#include <vector>

class Engine_API Input
{
	// friend ����.
	friend class Engine;

	// Ű�Է� Ȯ���� ���� ����ü ����.
	struct KeyState
	{
		// ���� �����ӿ� Ű�� ���ȴ��� ����.
		bool isKeyDown = false;

		// ���� �����ӿ� Ű�� ���ȴ��� ����.
		bool previouseKeyDown = false;
	};

public:
	Input();

	// Ű Ȯ�� �Լ�.
	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	static Input& Get();

	template<typename T, void (T::*Method)()>
	void RegisterKeydownCallback(int keyCode, T* object)
	{
		static_assert(std::is_base_of<Actor, T>::value, "T�� Actor�� ����ϴ� ��ü���� �մϴ�.");

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
		static_assert(std::is_base_of<Actor, T>::value, "T�� Actor�� ����ϴ� ��ü���� �մϴ�.");

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
		static_assert(std::is_base_of<Actor, T>::value, "T�� Actor�� ����ϴ� ��ü���� �մϴ�.");

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

	// Ű �Է� ���� ���� ����.
	KeyState keyStates[256] = { };

	std::unordered_map<int, Delegate> keydownCallbacks;
	std::unordered_map<int, Delegate> keyupCallbacks;
	std::unordered_map<int, Delegate> keyrepeatCallbacks;

	static Input* instance;
};