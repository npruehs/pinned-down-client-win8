#pragma once

#include "Source\Features\Layout\Components\BoundsComponent.h"
#include "Source\Features\Layout\Components\ScreenPositionComponent.h"
#include "Source\Features\Rendering\Components\DepthComponent.h"
#include "Source\Features\Rendering\Components\VisibilityComponent.h"

using namespace PinnedDownClient::Features::Layout;
using namespace PinnedDownClient::Features::Rendering;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Rendering
		{
			class IRenderable
			{
			public:
				virtual Entity GetEntity() = 0;

				std::shared_ptr<BoundsComponent> boundsComponent;
				std::shared_ptr<DepthComponent> depthComponent;
				std::shared_ptr<ScreenPositionComponent> screenPositionComponent;
				std::shared_ptr<VisibilityComponent> visibilityComponent;

				std::shared_ptr<DepthComponent> panelDepthComponent;
				std::shared_ptr<VisibilityComponent> panelVisibilityComponent;
			};
		}
	}
}