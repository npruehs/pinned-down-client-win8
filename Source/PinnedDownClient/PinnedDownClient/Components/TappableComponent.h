#pragma once

#include "pch.h"
#include "EntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class TappableComponent : public PinnedDownCore::EntityComponent
		{
		public:
			static const HashedString TappableComponentType;

			const HashedString & GetComponentType() const
			{
				return TappableComponentType;
			}
		};
	}
}