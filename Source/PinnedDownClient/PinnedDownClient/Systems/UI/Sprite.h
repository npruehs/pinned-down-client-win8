#pragma once

#include "pch.h"

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
				Entity entity;
				std::shared_ptr<SpriteComponent> spriteComponent;
				
				Entity GetEntity() { return this->entity; }
			};
		}
	}
}
