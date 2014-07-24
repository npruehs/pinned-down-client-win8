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
				int CreateLabel(VerticalAnchor top, HorizontalAnchor left, int anchorTarget);
				int CreateLabel(VerticalAnchor top, HorizontalAnchor left, int anchorTarget, std::wstring text);
				int CreateLabel(VerticalAnchor top, HorizontalAnchor left, int anchorTarget, std::wstring text, int depth);
				int CreatePanel();
				int CreateSprite(std::wstring spriteName);
				int CreateSprite(std::wstring spriteName, VerticalAnchor top, HorizontalAnchor left);
				int CreateSprite(std::wstring spriteName, VerticalAnchor top, HorizontalAnchor left, int anchorTarget);
				int CreateSprite(std::wstring spriteName, VerticalAnchor top, HorizontalAnchor left, int anchorTarget, int depth);

			private:
				std::shared_ptr<GameInfrastructure> game;

				void AddAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left);
				void AddAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left, int anchorTarget);
				void AddSprite(int entityId, std::wstring spriteName);
			};
		}
	}
}
