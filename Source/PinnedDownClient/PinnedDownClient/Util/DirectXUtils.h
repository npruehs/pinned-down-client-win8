#pragma once

#include <windows.h>

namespace PinnedDownClient
{
	namespace Util
	{
		inline void ThrowIfFailed(HRESULT hr)
		{
			if (FAILED(hr))
			{
				// Set a breakpoint on this line to catch Win32 API errors.
				throw Platform::Exception::CreateException(hr);
			}
		}

		// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
		inline float ConvertDipsToPixels(float dips, float dpi)
		{
			static const float dipsPerInch = 96.0f;
			return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
		}

	}
}