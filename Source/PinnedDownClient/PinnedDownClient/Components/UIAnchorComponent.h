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
			float relativeOffset;
			float absoluteOffset;

			HorizontalAnchor() : HorizontalAnchor::HorizontalAnchor(HorizontalAnchorType::Left, 0.0f)
			{
			}

			HorizontalAnchor(HorizontalAnchorType type, float relativeOffset) : HorizontalAnchor::HorizontalAnchor(type, relativeOffset, 0.0f)
			{
			}

			HorizontalAnchor(HorizontalAnchorType type, float relativeOffset, float absoluteOffset)
			{
				this->type = type;
				this->relativeOffset = relativeOffset;
				this->absoluteOffset = absoluteOffset;
			}
		};

		struct VerticalAnchor
		{
			VerticalAnchorType type;
			float relativeOffset;
			float absoluteOffset;

			VerticalAnchor() : VerticalAnchor::VerticalAnchor(VerticalAnchorType::Top, 0.0f)
			{
			}

			VerticalAnchor(VerticalAnchorType type, float relativeOffset) : VerticalAnchor::VerticalAnchor(type, relativeOffset, 0.0f)
			{
			}

			VerticalAnchor(VerticalAnchorType type, float relativeOffset, float absoluteOffset)
			{
				this->type = type;
				this->relativeOffset = relativeOffset;
				this->absoluteOffset = absoluteOffset;
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