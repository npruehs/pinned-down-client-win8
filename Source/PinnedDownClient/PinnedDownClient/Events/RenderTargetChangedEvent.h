#pragma once

#include "pch.h"
#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct RenderTargetChangedEvent : public PinnedDownClient::Core::Event
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