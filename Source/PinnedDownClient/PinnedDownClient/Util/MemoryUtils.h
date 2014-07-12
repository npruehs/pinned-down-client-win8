#pragma once

namespace PinnedDownClient
{
	namespace Util
	{
		template <class T> void SafeRelease(T **ppT)
		{
			if (*ppT)
			{
				(*ppT)->Release();
				*ppT = NULL;
			}
		}
	}
}