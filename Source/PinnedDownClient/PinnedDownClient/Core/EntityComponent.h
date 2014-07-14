#pragma once

#include "Util\HashedString.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Core
	{
		class EntityComponent
		{
		public:
			virtual const HashedString & GetComponentType() const = 0;
		};
	}
}