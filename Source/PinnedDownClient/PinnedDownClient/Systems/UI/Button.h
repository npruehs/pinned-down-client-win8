#pragma once

#include "pch.h"

#include "Components\BoundsComponent.h"
#include "Components\DepthComponent.h"
#include "Components\ScreenPositionComponent.h"

using namespace PinnedDownClient::Components;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			struct Button
			{
				Entity entity;
				std::shared_ptr<BoundsComponent> boundsComponent;
				std::shared_ptr<DepthComponent> depthComponent;
				std::shared_ptr<ScreenPositionComponent> screenPositionComponent;
			};
		}
	}
}
