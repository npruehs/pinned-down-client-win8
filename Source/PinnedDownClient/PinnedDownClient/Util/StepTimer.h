#pragma once

#include <wrl.h>

namespace PinnedDownClient
{
	namespace Util
	{
		// Helper class for animation and simulation timing.
		class StepTimer
		{
		public:
			StepTimer() :
				elapsedTicks(0),
				totalTicks(0),
				frameCount(0),
				framesPerSecond(0),
				framesThisSecond(0),
				qpcSecondCounter(0)
			{
				if (!QueryPerformanceFrequency(&qpcFrequency))
				{
					throw ref new Platform::FailureException();
				}

				if (!QueryPerformanceCounter(&qpcLastTime))
				{
					throw ref new Platform::FailureException();
				}

				// Initialize max delta to 1/10 of a second.
				qpcMaxDelta = qpcFrequency.QuadPart / 10;
			}

			// Gets the elapsed time since the previous Update call, in ticks.
			uint64 GetElapsedTicks() const { return elapsedTicks; }

			// Gets the elapsed time since the previous Update call, in seconds.
			double GetElapsedSeconds() const { return TicksToSeconds(elapsedTicks); }

			// Gets total time since the construction of this timer, in ticks.
			uint64 GetTotalTicks() const { return totalTicks; }

			// Gets total time since the construction of this timer, in seconds.
			double GetTotalSeconds() const { return TicksToSeconds(totalTicks); }

			// Gets total number of updates since the construction of this timer.
			uint32 GetFrameCount() const { return frameCount; }

			// Gets the current framerate.
			uint32 GetFramesPerSecond() const { return framesPerSecond; }

			// Integer format represents time using 10,000,000 ticks per second.
			static const uint64 TicksPerSecond = 10000000;

			// Converts the specified number of ticks to seconds.
			static double TicksToSeconds(uint64 ticks) { return static_cast<double>(ticks) / TicksPerSecond; }

			// Converts the specified number of seconds to ticks.
			static uint64 SecondsToTicks(double seconds) { return static_cast<uint64>(seconds * TicksPerSecond); }

			// Update timer state.
			void Update()
			{
				// Query the current time.
				LARGE_INTEGER currentTime;

				if (!QueryPerformanceCounter(&currentTime))
				{
					throw ref new Platform::FailureException();
				}

				uint64 timeDelta = currentTime.QuadPart - qpcLastTime.QuadPart;

				qpcLastTime = currentTime;
				qpcSecondCounter += timeDelta;

				// Clamp excessively large time deltas (e.g. after paused in the debugger).
				if (timeDelta > qpcMaxDelta)
				{
					timeDelta = qpcMaxDelta;
				}

				// Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
				timeDelta *= TicksPerSecond;
				timeDelta /= qpcFrequency.QuadPart;

				uint32 lastFrameCount = frameCount;

				// Variable timestep update logic.
				elapsedTicks = timeDelta;
				totalTicks += timeDelta;
				frameCount++;

				// Track the current framerate.
				if (frameCount != lastFrameCount)
				{
					framesThisSecond++;
				}

				if (qpcSecondCounter >= static_cast<uint64>(qpcFrequency.QuadPart))
				{
					framesPerSecond = framesThisSecond;
					framesThisSecond = 0;
					qpcSecondCounter %= qpcFrequency.QuadPart;
				}
			}

		private:
			// QueryPerformanceCounter source frequency.
			LARGE_INTEGER qpcFrequency;

			// Last value returned by QueryPerformanceCounter.
			LARGE_INTEGER qpcLastTime;

			// Maximum delta between two queries to QueryPerformanceCounter (e.g. for pausing in the debugger).
			uint64 qpcMaxDelta;

			// Aggregated QueryPerformanceCounter time delta. Used to determine when a full second has passed.
			uint64 qpcSecondCounter;

			// Canonical ticks since the last update.
			uint64 elapsedTicks;

			// Total ticks since the construction of this timer.
			uint64 totalTicks;

			// Total frames since the construction of this timer.
			uint32 frameCount;

			// Number of updates in the past second.
			uint32 framesPerSecond;

			// Number of updates in the current second.
			uint32 framesThisSecond;
		};
	}
}
