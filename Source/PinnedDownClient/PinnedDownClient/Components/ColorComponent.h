#pragma once

#include "pch.h"
#include "EntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class ColorComponent : public PinnedDownCore::EntityComponent
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
