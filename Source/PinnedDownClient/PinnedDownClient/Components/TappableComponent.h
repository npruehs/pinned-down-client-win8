#pragma once

#include "pch.h"
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class TappableComponent : public PinnedDownCore::IEntityComponent
		{
		public:
			static const HashedString TappableComponentType;

			const HashedString & GetComponentType() const
			{
				return TappableComponentType;
			}

			bool tappable = false;
		};
	}
}