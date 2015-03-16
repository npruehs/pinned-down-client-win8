#pragma once

#include "EntityManager.h"
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Cards
		{
			class CardBackgroundComponent : public PinnedDownCore::IEntityComponent
			{
			public:
				static const HashedString CardBackgroundComponentType;

				const HashedString & GetComponentType() const
				{
					return CardBackgroundComponentType;
				}

				Entity background;
			};
		}
	}
}
