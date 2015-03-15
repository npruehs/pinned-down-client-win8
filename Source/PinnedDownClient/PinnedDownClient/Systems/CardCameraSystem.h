#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\PlayerAddedEvent.h"

#include "Events\PointerDraggedEvent.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;


namespace PinnedDownClient
{
	namespace Systems
	{
		class CardCameraSystem : public GameSystem, public IEventListener
		{
		public:
			CardCameraSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			const float designWidth = 1920.0f;

			Entity camera;
			float cardCameraPositionX;
			float cardCameraPositionXMax;
			int players;

			void CreateCamera();

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(PlayerAddedEvent);
			EVENT_HANDLER_DECLARATION(PointerDraggedEvent);
		};
	}
}
