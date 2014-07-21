#pragma once

#include "pch.h"

#include "PinnedDownGame.h"

#include "Components\UIAnchorComponent.h"

using namespace PinnedDownClient::Components;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			class UIFactory
			{
			public:
				UIFactory(std::shared_ptr<GameInfrastructure> game);

				int CreateLabel(VerticalAnchor top, HorizontalAnchor left);

			private:
				std::shared_ptr<GameInfrastructure> game;
			};
		}
	}
}
