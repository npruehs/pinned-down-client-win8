#pragma once

#include "pch.h"
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class UIPanelComponent : public PinnedDownCore::IEntityComponent
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