#pragma once

#include "pch.h"
#include <string>
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Rendering
		{
			class TextAlignmentComponent : public PinnedDownCore::IEntityComponent
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
}