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
			class VisibilityComponent : public PinnedDownCore::IEntityComponent
			{
			public:
				static const HashedString VisibilityComponentType;

				const HashedString & GetComponentType() const
				{
					return VisibilityComponentType;
				}

				bool visible = true;
			};
		}
	}
}