#pragma once

#include "..\Core\ISystem.h"

using namespace PinnedDownClient::Core;

namespace PinnedDownClient
{
	namespace Systems
	{
		class RenderSystem : public Core::ISystem
		{
		public:
			RenderSystem();

			void Update(DX::StepTimer const& timer);
		};
	}
}