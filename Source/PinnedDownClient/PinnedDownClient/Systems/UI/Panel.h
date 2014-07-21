#pragma once

#include "pch.h"

#include "Components\BoundsComponent.h"

using namespace PinnedDownClient::Components;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			struct Panel
			{
				int entityId;
				std::shared_ptr<BoundsComponent> boundsComponent;
			};
		}
	}
}
