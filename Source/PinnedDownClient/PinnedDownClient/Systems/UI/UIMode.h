#pragma once

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace UI
		{
			enum UIMode
			{
				DefaultUIMode,
				SelectCardTarget
			};

			inline std::string UIModeToString(UIMode mode)
			{
				switch (mode)
				{
				case UIMode::DefaultUIMode:
					return "DefaultUIMode";
					break;

				case UIMode::SelectCardTarget:
					return "SelectCardTarget";
					break;

				default:
					return "Unknown";
					break;
				}
			}
		}
	}
}