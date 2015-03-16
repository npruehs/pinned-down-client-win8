#pragma once

#include "pch.h"

#include "Source\Features\Rendering\Components\SpriteComponent.h"

#include "Systems\Rendering\IRenderable.h"

using namespace PinnedDownClient::Features::Rendering;
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
