#pragma once

#include "EntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class DepthComponent : public PinnedDownCore::EntityComponent
		{
		public:
			static const HashedString DepthComponentType;

			const HashedString & GetComponentType() const
			{
				return DepthComponentType;
			}

			int depth = 0;
		};
	}
}