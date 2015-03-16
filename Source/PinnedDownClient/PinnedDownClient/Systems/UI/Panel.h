#pragma once

#include "pch.h"

#include "Source\Features\Layout\Components\BoundsComponent.h"
#include "Source\Features\Layout\Components\ScreenPositionComponent.h"

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			struct Panel
			{
				Entity entity;
				std::shared_ptr<BoundsComponent> boundsComponent;
				std::shared_ptr<ScreenPositionComponent> screenPositionComponent;
			};
		}
	}
}
