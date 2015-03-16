#pragma once

#include <string>
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Globalization
		{
			class TextComponent : public PinnedDownCore::IEntityComponent
			{
			public:
				static const HashedString TextComponentType;

				const HashedString & GetComponentType() const
				{
					return TextComponentType;
				}

				std::string text;
				float maxWidth = 0.3f;
			};
		}
	}
}