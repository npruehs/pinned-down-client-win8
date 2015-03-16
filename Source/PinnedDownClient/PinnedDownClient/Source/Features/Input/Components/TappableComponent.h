#pragma once

#include "pch.h"
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Input
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
}