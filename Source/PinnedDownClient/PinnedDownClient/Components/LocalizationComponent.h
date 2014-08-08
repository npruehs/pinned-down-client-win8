#pragma once

#include <string>
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Components
	{
		class LocalizationComponent : public PinnedDownCore::IEntityComponent
		{
		public:
			static const HashedString LocalizationComponentType;

			const HashedString & GetComponentType() const
			{
				return LocalizationComponentType;
			}

			std::wstring localizationKey;
			std::wstring localizationValue;
		};
	}
}