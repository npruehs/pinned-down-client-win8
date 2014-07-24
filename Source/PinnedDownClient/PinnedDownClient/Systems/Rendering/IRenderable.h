#pragma once

#include "Components\DepthComponent.h"
#include "Components\ScreenPositionComponent.h"

using namespace PinnedDownClient::Components;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Rendering
		{
			class IRenderable
			{
			public:
				virtual int GetEntityId() = 0;

				std::shared_ptr<DepthComponent> depthComponent;
				std::shared_ptr<ScreenPositionComponent> screenPositionComponent;
			};
		}
	}
}