#pragma once

#include "pch.h"

#include "Source\Features\Input\Components\TappableComponent.h"
#include "Source\Features\Layout\Components\BoundsComponent.h"
#include "Source\Features\Layout\Components\ScreenPositionComponent.h"
#include "Source\Features\Rendering\Components\DepthComponent.h"

using namespace PinnedDownClient::Features::Input;
using namespace PinnedDownClient::Features::Layout;
using namespace PinnedDownClient::Features::Rendering;

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
				std::shared_ptr<TappableComponent> tappableComponent;
			};
		}
	}
}
