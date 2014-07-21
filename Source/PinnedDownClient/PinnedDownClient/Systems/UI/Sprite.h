#pragma once

#include "pch.h"

#include "Components\ScreenPositionComponent.h"
#include "Components\SpriteComponent.h"

using namespace PinnedDownClient::Components;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			struct Sprite
			{
				int entityId;
				std::shared_ptr<ScreenPositionComponent> screenPositionComponent;
				std::shared_ptr<SpriteComponent> spriteComponent;
			};
		}
	}
}
