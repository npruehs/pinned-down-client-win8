#pragma once

#include <string>
#include "EntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class TextComponent : public PinnedDownCore::EntityComponent
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