#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"
#include "Math\Vector2F.h"

#include "Components\UIAnchorComponent.h"

#include "Events\PointerMovedEvent.h"

#include "Systems\UI\UIFactory.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Systems::UI;

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
			std::shared_ptr<UIFactory> uiFactory;

			int pointerPositionTextEntity;
			int fpsTextEntity;
			int versionTextEntity;

			void CreateEntities();

			void OnEvent(Event & event);

			void OnPointerMoved(PointerMovedEvent pointerMovedEvent);
		};
	}
}