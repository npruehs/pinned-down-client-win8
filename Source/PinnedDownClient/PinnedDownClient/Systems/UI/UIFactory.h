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

				Entity CreateLabel(std::wstring localizationKey);
				Entity CreatePanel();
				Entity CreateSprite(std::string spriteName);

				void SetAnchor(Entity entity, VerticalAnchor top, HorizontalAnchor left);
				void SetAnchor(Entity entity, VerticalAnchor top, HorizontalAnchor left, Entity anchorTarget);
				void SetColor(Entity entity, D2D1_COLOR_F color);
				void SetDepth(Entity entity, int depth);
				void SetFontSize(Entity entity, float fontSize);
				void SetPanel(Entity entity, Entity panel);
				void SetTappable(Entity entity, bool tappable);
				void SetText(Entity entity, std::wstring text);
				void SetVisible(Entity entity, bool visible);

				void FinishUIWidget(Entity widget);
			private:
				PinnedDownCore::Game* game;
			};
		}
	}
}
