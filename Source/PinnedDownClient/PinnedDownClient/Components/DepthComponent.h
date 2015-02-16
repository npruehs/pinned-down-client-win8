#pragma once

#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class DepthComponent : public PinnedDownCore::IEntityComponent
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