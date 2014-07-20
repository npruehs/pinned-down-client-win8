#pragma once

#include "pch.h"
#include <string>
#include "Core\EntityComponent.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Components
	{
		class TextAlignmentComponent : public PinnedDownClient::Core::EntityComponent
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