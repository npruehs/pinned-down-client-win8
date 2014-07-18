#pragma once

#include <string>
#include "Vector2F.h"

namespace PinnedDownClient
{
	namespace Math
	{
		float Max(float f, float g)
		{
			return (f > g) ? f : g;
		}

		float Min(float f, float g)
		{
			return (f < g) ? f : g);
		}
	}
}
