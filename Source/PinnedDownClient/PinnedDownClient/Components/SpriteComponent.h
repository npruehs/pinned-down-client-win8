#pragma once

#include "pch.h"
#include <string>
#include "Core\EntityComponent.h"
#include "Core\Resources\BitmapResourceHandle.h"

using namespace PinnedDownClient::Core::Resources;
using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class SpriteComponent : public PinnedDownClient::Core::EntityComponent
		{
		public:
			static const HashedString SpriteComponentType;

			const HashedString & GetComponentType() const
			{
				return SpriteComponentType;
			}

			std::shared_ptr<BitmapResourceHandle> sprite;
		};
	}
}