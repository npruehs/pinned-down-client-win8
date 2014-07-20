#pragma once

#include "Core\EntityComponent.h"
#include "Math\Vector2F.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class ScreenPositionComponent : public PinnedDownClient::Core::EntityComponent
		{
		public:
			static const HashedString ScreenPositionComponentType;

			const HashedString & GetComponentType() const
			{
				return ScreenPositionComponentType;
			}

			PinnedDownClient::Math::Vector2F position;
		};
	}
}