#pragma once

#include "IEntityComponent.h"
#include "Math\Vector2F.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Layout
		{
			class PositionComponent : public PinnedDownCore::IEntityComponent
			{
			public:
				static const HashedString PositionComponentType;

				const HashedString & GetComponentType() const
				{
					return PositionComponentType;
				}

				PinnedDownClient::Math::Vector2F position = PinnedDownClient::Math::Vector2F();
			};
		}
	}
}