#pragma once

#include "IEntityComponent.h"
#include "Math\Vector2F.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class ScreenPositionComponent : public PinnedDownCore::IEntityComponent
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