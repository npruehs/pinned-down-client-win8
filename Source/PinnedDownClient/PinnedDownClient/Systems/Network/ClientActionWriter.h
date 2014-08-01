#pragma once

#include "PinnedDownNet.h"

using namespace Windows::Storage::Streams;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Network
		{
			class ClientActionWriter
			{
			public:
				ClientActionWriter(DataWriter^ dataWriter);

				ClientAction WriteClientAction(ClientAction action);

			private:
				DataWriter^ dataWriter;
			};
		}
	}
}