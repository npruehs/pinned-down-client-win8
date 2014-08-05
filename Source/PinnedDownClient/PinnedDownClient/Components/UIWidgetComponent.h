#pragma once

#include "pch.h"
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class UIWidgetComponent : public PinnedDownCore::IEntityComponent
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