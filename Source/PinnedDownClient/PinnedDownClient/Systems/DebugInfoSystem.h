#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"
#include "Math\Vector2F.h"

#include "Events\PointerMovedEvent.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class DebugInfoSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			DebugInfoSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

		private:
			Vector2F pointerPosition;

			int pointerPositionTextEntity;

			void CreateEntities();

			void OnEvent(Event & event);

			void OnPointerMoved(PointerMovedEvent pointerMovedEvent);
		};
	}
}