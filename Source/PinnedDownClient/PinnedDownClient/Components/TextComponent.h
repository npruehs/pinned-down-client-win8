#pragma once

#include <string>
#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class TextComponent : public PinnedDownClient::Core::EntityComponent
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