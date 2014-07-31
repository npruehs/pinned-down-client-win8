#pragma once

#include "EntityComponent.h"
#include "Math\Vector2F.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class BoundsComponent : public PinnedDownCore::EntityComponent
		{
		public:
			static const HashedString BoundsComponentType;

			const HashedString & GetComponentType() const
			{
				return BoundsComponentType;
			}

			PinnedDownClient::Math::Vector2F bounds;
		};
	}
}