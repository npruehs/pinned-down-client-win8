#pragma once

#include "pch.h"
#include "EntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class UIPanelComponent : public PinnedDownCore::EntityComponent
		{
		public:
			static const HashedString UIPanelComponentType;

			const HashedString & GetComponentType() const
			{
				return UIPanelComponentType;
			}
		};
	}
}