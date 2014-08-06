#pragma once

#include "pch.h"

#include "Components\BoundsComponent.h"
#include "Components\ScreenPositionComponent.h"

using namespace PinnedDownClient::Components;

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
