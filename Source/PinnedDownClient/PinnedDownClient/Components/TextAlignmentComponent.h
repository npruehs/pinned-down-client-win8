#pragma once

#include "pch.h"
#include <string>
#include "EntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class TextAlignmentComponent : public PinnedDownCore::EntityComponent
		{
		public:
			static const HashedString TextAlignmentComponentType;

			const HashedString & GetComponentType() const
			{
				return TextAlignmentComponentType;
			}

			DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		};
	}
}