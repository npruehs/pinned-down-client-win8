#pragma once

#include "pch.h"
#include <string>
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class FontComponent : public PinnedDownCore::IEntityComponent
		{
		public:
			static const HashedString FontComponentType;

			const HashedString & GetComponentType() const
			{
				return FontComponentType;
			}

			std::wstring fontFamilyName = L"Arial";
			DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
			DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL;
			DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL;
			float fontSize = 16.0f;
		};
	}
}