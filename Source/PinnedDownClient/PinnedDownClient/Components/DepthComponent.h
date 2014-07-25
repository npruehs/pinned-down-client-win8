#pragma once

#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class DepthComponent : public PinnedDownClient::Core::EntityComponent
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