#pragma once

#include "pch.h"
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

			HorizontalAnchor::HorizontalAnchor() : HorizontalAnchor::HorizontalAnchor(HorizontalAnchorType::Left, 0.0f)
			{
			}

			HorizontalAnchor::HorizontalAnchor(HorizontalAnchorType type, float offset)
			{
				this->type = type;
				this->offset = offset;
			}
		};

		struct VerticalAnchor
		{
			VerticalAnchorType type;
			float offset;

			VerticalAnchor::VerticalAnchor() : VerticalAnchor::VerticalAnchor(VerticalAnchorType::Top, 0.0f)
			{
			}

			VerticalAnchor::VerticalAnchor(VerticalAnchorType type, float offset)
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

			int target = 0;
			HorizontalAnchor left;
			HorizontalAnchor right;
			VerticalAnchor top;
			VerticalAnchor bottom;
		};
	}
}