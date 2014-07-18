#pragma once

#include "Core\EntityComponent.h"
#include "Math\Vector2F.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class PositionComponent : public PinnedDownClient::Core::EntityComponent
		{
		public:
			static const HashedString PositionComponentType;

			const HashedString & GetComponentType() const
			{
				return PositionComponentType;
			}

			PinnedDownClient::Math::Vector2F position;
		};
	}
}