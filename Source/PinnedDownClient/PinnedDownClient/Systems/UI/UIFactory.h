#pragma once

#include "pch.h"

#include "Game.h"
#include "PinnedDownGame.h"

#include "Components\UIAnchorComponent.h"

using namespace PinnedDownCore;
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
				UIFactory(PinnedDownCore::Game* game);

				int CreateLabel(std::wstring text);
				int CreatePanel();
				int CreateSprite(std::string spriteName);

				void SetAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left);
				void SetAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left, int anchorTarget);
				void SetDepth(int entityId, int depth);
				void SetFontSize(int entityId, float fontSize);
				void SetPanel(int entityId, int panelId);
				void SetTappable(int entityId);
				void SetVisible(int entityId, bool visible);

				void FinishUIWidget(int entityId);
			private:
				PinnedDownCore::Game* game;
			};
		}
	}
}
