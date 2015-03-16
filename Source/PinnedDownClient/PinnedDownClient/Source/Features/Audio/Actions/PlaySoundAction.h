#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Features
	{
		namespace Audio
		{
			struct PlaySoundAction : public PinnedDownCore::Event
			{
				static const HashedString PlaySoundActionType;

				const HashedString & GetEventType() const
				{
					return PlaySoundActionType;
				}

				std::string soundAsset;

				explicit PlaySoundAction(std::string soundAsset)
				{
					this->soundAsset = soundAsset;
				}

				std::string ToString() const
				{
					return std::string("Play sound " + this->soundAsset);
				}
			};
		}
	}
}