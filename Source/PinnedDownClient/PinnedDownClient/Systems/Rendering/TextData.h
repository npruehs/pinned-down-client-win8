#pragma once

#include "pch.h"
#include <string>
#include "Math\Vector2F.h"

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
		namespace Rendering
		{
			struct TextData
			{
				std::shared_ptr<ColorComponent> colorComponent;
				std::shared_ptr<FontComponent> fontComponent;
				std::shared_ptr<ScreenPositionComponent> screenPositionComponent;
				std::shared_ptr<TextComponent> textComponent;
				std::shared_ptr<TextAlignmentComponent> textAlignmentComponent;
			};
		}
	}
}
