#pragma once

#include "EntityManager.h"
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class CardUIComponent : public PinnedDownCore::IEntityComponent
		{
		public:
			static const HashedString CardUIComponentType;

			const HashedString & GetComponentType() const
			{
				return CardUIComponentType;
			}

			Entity background;
		};
	}
}
