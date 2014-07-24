#pragma once

#include "pch.h"

#include "Components\ScreenPositionComponent.h"
#include "Components\SpriteComponent.h"
#include "Systems\Rendering\IRenderable.h"

using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Systems::Rendering;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			class Sprite : public IRenderable
			{
			public:
				int entityId;
				std::shared_ptr<SpriteComponent> spriteComponent;

				int GetEntityId() { return this->entityId; }
			};
		}
	}
}
