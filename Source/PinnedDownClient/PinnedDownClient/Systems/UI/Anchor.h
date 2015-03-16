#pragma once

#include "pch.h"

#include "Source\Features\Layout\Components\BoundsComponent.h"
#include "Source\Features\Layout\Components\ScreenPositionComponent.h"
#include "Source\Features\UI\Components\UIAnchorComponent.h"

using namespace PinnedDownClient::Features::Layout;
using namespace PinnedDownClient::Features::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			struct Anchor
			{
				Entity entity;
				std::shared_ptr<UIAnchorComponent> anchorComponent;
				std::shared_ptr<BoundsComponent> boundsComponent;
				std::shared_ptr<ScreenPositionComponent> screenPositionComponent;
			};
		}
	}
}
