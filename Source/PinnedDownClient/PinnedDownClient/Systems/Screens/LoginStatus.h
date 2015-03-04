#pragma once

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			enum LoginStatus
			{
				Default,
				Connecting,
				VerifyingClientVersion,
				WaitingForPlayers,
				ConnectionError
			};
		}
	}
}