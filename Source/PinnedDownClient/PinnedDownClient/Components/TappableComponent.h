#pragma once

#include "pch.h"
#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class TappableComponent : public PinnedDownClient::Core::EntityComponent
		{
		public:
			static const HashedString TappableComponentType;

			const HashedString & GetComponentType() const
			{
				return TappableComponentType;
			}
		};
	}
}