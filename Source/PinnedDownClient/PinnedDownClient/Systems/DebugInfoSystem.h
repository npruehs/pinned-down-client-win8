#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"
#include "Math\Vector2F.h"
#include "Util\StepTimer.h"

#include "Components\UIAnchorComponent.h"

#include "Events\PointerMovedEvent.h"

#include "Systems\UI\UIFactory.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Systems
	{
		class DebugInfoSystem : public GameSystem, public IEventListener
		{
		public:
			DebugInfoSystem();

			void InitSystem(PinnedDownCore::Game* game);
			void Update(float dt);

		private:
			Vector2F pointerPosition;
			std::shared_ptr<UIFactory> uiFactory;
			std::shared_ptr<StepTimer> timer;

			Entity pointerPositionTextEntity = INVALID_ENTITY_ID;
			Entity fpsTextEntity = INVALID_ENTITY_ID;
			Entity versionTextEntity = INVALID_ENTITY_ID;

			void CreateEntities();

			void OnEvent(Event & event);

			void OnPointerMoved(PointerMovedEvent pointerMovedEvent);
		};
	}
}