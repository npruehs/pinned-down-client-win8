#pragma once

#include "pch.h"

#include "Components\ScreenPositionComponent.h"
#include "Components\UIAnchorComponent.h"

using namespace PinnedDownClient::Components;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			struct Anchor
			{
				int entityId;
				std::shared_ptr<UIAnchorComponent> anchorComponent;
				std::shared_ptr<ScreenPositionComponent> screenPositionComponent;
			};
		}
	}
}
