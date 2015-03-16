#pragma once

#include "pch.h"
#include <string>
#include "Math\Vector2F.h"
#include "Systems\Rendering\IRenderable.h"

#include "Source\Features\Globalization\Components\TextComponent.h"
#include "Source\Features\Rendering\Components\ColorComponent.h"
#include "Source\Features\Rendering\Components\FontComponent.h"
#include "Source\Features\Rendering\Components\TextAlignmentComponent.h"

using namespace PinnedDownClient::Features::Globalization;
using namespace PinnedDownClient::Features::Rendering;

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

				std::shared_ptr<ColorComponent> colorComponent;
				std::shared_ptr<FontComponent> fontComponent;
				std::shared_ptr<TextComponent> textComponent;
				std::shared_ptr<TextAlignmentComponent> textAlignmentComponent;

				Entity GetEntity() { return this->entity; }
			};
		}
	}
}
