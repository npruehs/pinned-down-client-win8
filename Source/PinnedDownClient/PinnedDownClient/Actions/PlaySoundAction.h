#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct PlaySoundAction : public PinnedDownCore::Event
		{
			static const HashedString PlaySoundActionType;

			const HashedString & GetEventType() const
			{
				return PlaySoundActionType;
			}

			std::wstring soundAsset;

			explicit PlaySoundAction(std::wstring soundAsset)
			{
				this->soundAsset = soundAsset;
			}

			std::wstring ToString()
			{
				return std::wstring(L"Play sound " + this->soundAsset);
			}
		};
	}
}