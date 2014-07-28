#pragma once

#include "Systems\Screens\Screen.h"

using namespace PinnedDownClient::Core;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			class GameScreen : public Screen
			{
			public:
				GameScreen();

				void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext);
				void UnloadResources();

				void LoadUI();
				void UnloadUI();

			private:
				int panel1;
				int window1;
				int button1;
				int panel2;
				int window2;
				int button2;
				int longLabel;
			};
		}
	}
}