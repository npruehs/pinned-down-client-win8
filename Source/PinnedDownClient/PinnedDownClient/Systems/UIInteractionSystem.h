#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "EntityInitializedEvent.h"
#include "EntityRemovedEvent.h"
#include "Events\PointerPressedEvent.h"
#include "Events\PointerReleasedEvent.h"

#include "Systems\UI\Button.h"


using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UIInteractionSystem : public GameSystem, public IEventListener
		{
		public:
			UIInteractionSystem();

			void InitSystem(PinnedDownCore::Game* game);
			void Update(float dt);

		private:
			std::list<Button> buttons;

			UI::Button* currentButton = nullptr;
			float elapsedTapTime = 0.0f;
			float hoverThreshold = 1.0f;

			void OnEvent(Event & event);

			void OnEntityInitialized(EntityInitializedEvent& entityInitializedEvent);
			void OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent);
			void OnPointerPressed(PointerPressedEvent& pointerPressedEvent);
			void OnPointerReleased(PointerReleasedEvent& pointerReleasedEvent);
			
			UI::Button* HitTest(Vector2F position);
		};
	}
}