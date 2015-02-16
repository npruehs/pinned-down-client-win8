#pragma once

#include <memory>

#include "PinnedDownNet.h"
#include "Event.h"

using namespace PinnedDownCore;
using namespace Windows::Storage::Streams;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Network
		{
			class ServerEventReader
			{
			public:
				ServerEventReader(DataReader^ dataReader);

				std::shared_ptr<Event> ReadServerEvent(int packetSize);

			private:
				DataReader^ dataReader;
			};
		}
	}
}