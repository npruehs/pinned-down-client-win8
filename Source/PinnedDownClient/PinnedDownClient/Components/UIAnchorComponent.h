#pragma once

#include "pch.h"
#include "EntityManager.h"
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		enum HorizontalAnchorType
		{
			Left,
			HorizontalCenter,
			Right
		};

		enum VerticalAnchorType
		{
			Top,
			VerticalCenter,
			Bottom
		};

		struct HorizontalAnchor
		{
			HorizontalAnchorType type;
			float offset;

			HorizontalAnchor() : HorizontalAnchor::HorizontalAnchor(HorizontalAnchorType::Left, 0.0f)
			{
			}

			HorizontalAnchor(HorizontalAnchorType type, float offset)
			{
				this->type = type;
				this->offset = offset;
			}
		};

		struct VerticalAnchor
		{
			VerticalAnchorType type;
			float offset;

			VerticalAnchor() : VerticalAnchor::VerticalAnchor(VerticalAnchorType::Top, 0.0f)
			{
			}

			VerticalAnchor(VerticalAnchorType type, float offset)
			{
				this->type = type;
				this->offset = offset;
			}
		};

		class UIAnchorComponent : public PinnedDownCore::IEntityComponent
		{
		public:
			static const HashedString UIAnchorComponentType;

			const HashedString & GetComponentType() const
			{
				return UIAnchorComponentType;
			}

			Entity target = INVALID_ENTITY_ID;
			HorizontalAnchor left;
			HorizontalAnchor right;
			VerticalAnchor top;
			VerticalAnchor bottom;
		};
	}
}