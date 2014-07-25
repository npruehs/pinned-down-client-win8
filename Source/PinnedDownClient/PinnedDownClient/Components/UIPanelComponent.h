#pragma once

#include "pch.h"
#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class UIPanelComponent : public PinnedDownClient::Core::EntityComponent
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