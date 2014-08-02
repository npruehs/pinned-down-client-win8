#pragma once

#include "pch.h"
#include <string>
#include "EntityComponent.h"
#include "Core\Resources\BitmapResourceHandle.h"

using namespace PinnedDownClient::Core::Resources;
using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class SpriteComponent : public PinnedDownCore::EntityComponent
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