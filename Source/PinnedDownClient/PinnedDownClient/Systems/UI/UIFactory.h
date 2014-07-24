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

				int CreateLabel(std::wstring text);
				int CreatePanel();
				int CreateSprite(std::wstring spriteName);

				void SetAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left);
				void SetAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left, int anchorTarget);
				void SetDepth(int entityId, int depth);
				void SetPanel(int entityId, int panelId);
				void SetVisible(int entityId, bool visible);

				void FinishUIWidget(int entityId);
			private:
				std::shared_ptr<GameInfrastructure> game;
			};
		}
	}
}
