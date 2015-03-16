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
			class BoundsComponent : public PinnedDownCore::IEntityComponent
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
}