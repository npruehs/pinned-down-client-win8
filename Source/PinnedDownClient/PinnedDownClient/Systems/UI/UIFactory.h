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

				Entity CreateLabel(std::wstring text);
				Entity CreatePanel();
				Entity CreateSprite(std::string spriteName);

				void SetAnchor(Entity entity, VerticalAnchor top, HorizontalAnchor left);
				void SetAnchor(Entity entity, VerticalAnchor top, HorizontalAnchor left, Entity anchorTarget);
				void SetDepth(Entity entity, int depth);
				void SetFontSize(Entity entity, float fontSize);
				void SetPanel(Entity entity, Entity panel);
				void SetTappable(Entity entity);
				void SetVisible(Entity entity, bool visible);

				void FinishUIWidget(Entity widget);
			private:
				PinnedDownCore::Game* game;
			};
		}
	}
}
