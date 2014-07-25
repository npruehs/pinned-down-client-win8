#pragma once

#include "pch.h"
#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class UIWidgetComponent : public PinnedDownClient::Core::EntityComponent
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