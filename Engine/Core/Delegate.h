#pragma once

#include "Core.h"

class Actor;

using Function = void(*)(Actor*);
struct Engine_API Delegate
{
	Actor* instance = nullptr;
	Function function = nullptr;

	void Invoke()
	{
		if (!instance || !function)
		{
			return;
		}

		function(instance);
	}
};