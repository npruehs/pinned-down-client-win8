#pragma once

#include "pch.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AudioEngineChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString AudioEngineChangedEventType;

			const HashedString & GetEventType() const
			{
				return AudioEngineChangedEventType;
			}

			Microsoft::WRL::ComPtr<IXAudio2> audioEngine;

			explicit AudioEngineChangedEvent(Microsoft::WRL::ComPtr<IXAudio2> audioEngine)
			{
				this->audioEngine = audioEngine;
			}
		};
	}
}