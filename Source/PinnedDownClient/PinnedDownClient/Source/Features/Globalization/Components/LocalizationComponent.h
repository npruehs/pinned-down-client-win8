#pragma once

#include <string>
#include "IEntityComponent.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Globalization
		{
			class LocalizationComponent : public PinnedDownCore::IEntityComponent
			{
			public:
				static const HashedString LocalizationComponentType;

				const HashedString & GetComponentType() const
				{
					return LocalizationComponentType;
				}

				std::string localizationKey;
				std::string localizationValue;
			};
		}
	}
}