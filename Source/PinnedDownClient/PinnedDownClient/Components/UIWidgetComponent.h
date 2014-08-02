#pragma once

#include "pch.h"
#include "EntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class UIWidgetComponent : public PinnedDownCore::EntityComponent
		{
		public:
			static const HashedString UIWidgetComponentType;

			const HashedString & GetComponentType() const
			{
				return UIWidgetComponentType;
			}

			int panel = 0;
		};
	}
}