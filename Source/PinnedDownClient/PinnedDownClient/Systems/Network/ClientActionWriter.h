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

				void WriteClientAction(ClientAction action);

			private:
				DataWriter^ dataWriter;
			};
		}
	}
}