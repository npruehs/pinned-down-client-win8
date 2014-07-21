#pragma once

#include "Core\EntityComponent.h"
#include "Math\Vector2F.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class BoundsComponent : public PinnedDownClient::Core::EntityComponent
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