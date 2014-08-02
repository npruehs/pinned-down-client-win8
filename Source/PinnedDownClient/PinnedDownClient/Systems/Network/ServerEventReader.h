#pragma once

#include "PinnedDownNet.h"

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

				ServerEvent ReadServerEvent();

			private:
				DataReader^ dataReader;
			};
		}
	}
}