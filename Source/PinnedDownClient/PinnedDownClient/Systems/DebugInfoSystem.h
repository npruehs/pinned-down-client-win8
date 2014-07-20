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
			void Update(StepTimer const& timer);

		private:
			Vector2F pointerPosition;

			int pointerPositionTextEntity;
			int fpsTextEntity;
			int versionTextEntity;

			void CreateEntities();
			int CreateTextEntity(Vector2F screenPosition);

			void OnEvent(Event & event);

			void OnPointerMoved(PointerMovedEvent pointerMovedEvent);
		};
	}
}