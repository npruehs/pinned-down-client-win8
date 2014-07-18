#pragma once

#include "pch.h"
#include <string>
#include "Math\Vector2F.h"

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Rendering
		{
			struct TextData
			{
				std::wstring text;
				PinnedDownClient::Math::Vector2F position;
				DWRITE_TEXT_ALIGNMENT alignment;
			};
		}
	}
}
