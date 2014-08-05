#pragma once

#include "Event.h"
#include "PinnedDownNet.h"

using namespace Windows::Storage::Streams;

using namespace PinnedDownCore;

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

				void WriteClientAction(Event& clientAction);

			private:
				DataWriter^ dataWriter;
			};
		}
	}
}