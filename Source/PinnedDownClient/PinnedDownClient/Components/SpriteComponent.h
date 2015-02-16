#pragma once

#include "pch.h"
#include <string>
#include "IEntityComponent.h"
#include "Resources\BitmapResourceHandle.h"

using namespace PinnedDownClient::Resources;
using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class SpriteComponent : public PinnedDownCore::IEntityComponent
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