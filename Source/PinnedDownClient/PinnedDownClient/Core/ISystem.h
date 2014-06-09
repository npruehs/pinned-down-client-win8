#pragma once

#include "../Helpers/StepTimer.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class ISystem
		{
		public:
			virtual void InitSystem() = 0;
			virtual void Update(DX::StepTimer const& timer) = 0;
		};
	}
}