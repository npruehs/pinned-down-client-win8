#pragma once

#include "pch.h"

#include "PinnedDownGame.h"

using namespace PinnedDownClient;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			class IScreen
			{
			public:
				virtual void InitScreen(std::shared_ptr<GameInfrastructure> game) = 0;

				virtual void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext) = 0;
				virtual void UnloadResources() = 0;

				virtual void LoadUI() = 0;
				virtual void UnloadUI() = 0;
			};
		}
	}
}