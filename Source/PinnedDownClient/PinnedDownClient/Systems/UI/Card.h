#pragma once

#include "pch.h"

#include "EntityManager.h"

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			struct Card
			{
				Entity abilityLabel = INVALID_ENTITY_ID;
				Entity backgroundSprite = INVALID_ENTITY_ID;
				Entity cardEntity = INVALID_ENTITY_ID;
				Entity cardTypeLabel = INVALID_ENTITY_ID;
				Entity nameLabel = INVALID_ENTITY_ID;
				Entity panel = INVALID_ENTITY_ID;
				Entity powerLabel = INVALID_ENTITY_ID;
				Entity powerValueLabel = INVALID_ENTITY_ID;
				Entity structureLabel = INVALID_ENTITY_ID;
				Entity structureValueLabel = INVALID_ENTITY_ID;
				Entity threatLabel = INVALID_ENTITY_ID;
			};
		}
	}
}
