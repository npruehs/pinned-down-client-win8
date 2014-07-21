#pragma once

#include "pch.h"
#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		enum HorizontalAnchorType
		{
			Left,
			Right
		};

		enum VerticalAnchorType
		{
			Top,
			Bottom
		};

		struct HorizontalAnchor
		{
			HorizontalAnchorType type;
			float offset;

			HorizontalAnchor::HorizontalAnchor()
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

			VerticalAnchor::VerticalAnchor()
			{
			}

			VerticalAnchor::VerticalAnchor(VerticalAnchorType type, float offset)
			{
				this->type = type;
				this->offset = offset;
			}
		};

		class UIAnchorComponent : public PinnedDownClient::Core::EntityComponent
		{
		public:
			static const HashedString UIAnchorComponentType;

			const HashedString & GetComponentType() const
			{
				return UIAnchorComponentType;
			}

			HorizontalAnchor left;
			HorizontalAnchor right;
			VerticalAnchor top;
			VerticalAnchor bottom;
		};
	}
}