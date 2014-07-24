#pragma once

#include "pch.h"
#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class VisibilityComponent : public PinnedDownClient::Core::EntityComponent
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