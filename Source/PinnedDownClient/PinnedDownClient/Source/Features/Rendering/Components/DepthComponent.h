#pragma once

#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Rendering
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
}