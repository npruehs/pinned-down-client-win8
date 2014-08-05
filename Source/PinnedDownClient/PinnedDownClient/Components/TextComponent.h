#pragma once

#include <string>
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class TextComponent : public PinnedDownCore::IEntityComponent
		{
		public:
			static const HashedString TextComponentType;

			const HashedString & GetComponentType() const
			{
				return TextComponentType;
			}

			std::wstring text;
		};
	}
}