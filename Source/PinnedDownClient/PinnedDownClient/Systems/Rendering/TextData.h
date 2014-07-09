#pragma once

#include "pch.h"
#include <string>

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Rendering
		{
			struct TextData
			{
				std::wstring Text;
				float X;
				float Y;
				DWRITE_TEXT_ALIGNMENT Alignment;
			};
		}
	}
}
