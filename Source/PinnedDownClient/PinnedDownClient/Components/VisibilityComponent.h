#pragma once

#include "pch.h"
#include "EntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class VisibilityComponent : public PinnedDownCore::EntityComponent
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