#pragma once

#include "pch.h"
#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class ColorComponent : public PinnedDownClient::Core::EntityComponent
		{
		public:
			static const HashedString ColorComponentType;

			const HashedString & GetComponentType() const
			{
				return ColorComponentType;
			}

			D2D1_COLOR_F color;
		};
	}
}
