#pragma once

#include "Systems\Screens\Screen.h"

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

				ScreenName GetScreenName();

			private:
				int panel1 = INVALID_ENTITY_ID;
				int window1 = INVALID_ENTITY_ID;
				int button1 = INVALID_ENTITY_ID;
				int panel2 = INVALID_ENTITY_ID;
				int window2 = INVALID_ENTITY_ID;
				int button2 = INVALID_ENTITY_ID;
				int longLabel = INVALID_ENTITY_ID;
			};
		}
	}
}