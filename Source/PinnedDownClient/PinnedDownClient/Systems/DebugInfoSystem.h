#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"
#include "Math\Vector2F.h"
#include "Util\StepTimer.h"

#include "Components\UIAnchorComponent.h"

#include "Events\CardCameraCreatedEvent.h"
#include "Events\PointerMovedEvent.h"
#include "Events\PositionChangedEvent.h"

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
			Entity cardCamera = INVALID_ENTITY_ID;
			Vector2F pointerPosition;
			std::shared_ptr<StepTimer> timer;
			std::shared_ptr<UIFactory> uiFactory;
			

			Entity cameraPositionTextEntity = INVALID_ENTITY_ID;
			Entity pointerPositionTextEntity = INVALID_ENTITY_ID;
			Entity fpsTextEntity = INVALID_ENTITY_ID;
			Entity versionTextEntity = INVALID_ENTITY_ID;

			const Vector2F cameraPositionTextPosition = Vector2F(0.0125f, -0.110f);
			const Vector2F pointerPositionTextPosition = Vector2F(0.0125f, -0.088f);
			const Vector2F fpsTextPosition = Vector2F(0.0125f, - 0.067f);
			const Vector2F versionTextPosition = Vector2F(0.0125f, -0.044f);

			void CreateEntities();

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(CardCameraCreatedEvent);
			EVENT_HANDLER_DECLARATION(PointerMovedEvent);
			EVENT_HANDLER_DECLARATION(PositionChangedEvent);
		};
	}
}