#pragma once

#include "pch.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct RenderTargetChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString RenderTargetChangedEventType;

			const HashedString & GetEventType() const
			{
				return RenderTargetChangedEventType;
			}

			Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext;

			explicit RenderTargetChangedEvent(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext)
			{
				this->d2dContext = d2dContext;
			}
		};
	}
}