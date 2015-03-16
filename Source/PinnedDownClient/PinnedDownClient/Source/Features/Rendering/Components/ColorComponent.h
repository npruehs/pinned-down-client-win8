#pragma once

#include "pch.h"
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Rendering
		{
			class ColorComponent : public PinnedDownCore::IEntityComponent
			{
			public:
				static const HashedString ColorComponentType;

				const HashedString & GetComponentType() const
				{
					return ColorComponentType;
				}

				D2D1_COLOR_F color;
			};
		}
	}
}
