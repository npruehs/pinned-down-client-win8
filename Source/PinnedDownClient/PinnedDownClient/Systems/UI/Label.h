#pragma once

#include "pch.h"
#include <string>
#include "Math\Vector2F.h"
#include "Systems\Rendering\IRenderable.h"

#include "Components\BoundsComponent.h"
#include "Components\ColorComponent.h"
#include "Components\FontComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"

using namespace PinnedDownClient::Components;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			class Label : public IRenderable
			{
			public:
				Entity entity;
				std::shared_ptr<BoundsComponent> boundsComponent;
				std::shared_ptr<ColorComponent> colorComponent;
				std::shared_ptr<FontComponent> fontComponent;
				std::shared_ptr<TextComponent> textComponent;
				std::shared_ptr<TextAlignmentComponent> textAlignmentComponent;

				Entity GetEntity() { return this->entity; }
			};
		}
	}
}
