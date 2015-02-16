#pragma once

#include <thread>
#include <mutex>

#include "IEventListener.h"
#include "GameSystem.h"

#include "Actions\PlaySoundAction.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace Microsoft::WRL;

namespace PinnedDownClient
{
	namespace Systems
	{
		class SoundSystem : public GameSystem, public IEventListener, public IXAudio2VoiceCallback
		{
		public:
			SoundSystem();
			~SoundSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			// Instance of the XAudio2 engine for playing sounds.
			ComPtr<IXAudio2> soundAudioEngine;
			ComPtr<IXAudio2> musicAudioEngine;

			// Encapsulates an audio device. Ultimate destination for all audio that passes through an audio graph.
			IXAudio2MasteringVoice* soundMasteringVoice;
			IXAudio2MasteringVoice* musicMasteringVoice;

			// Source voice for passing audio through the audio graph.
			IXAudio2SourceVoice* soundSourceVoice;
			IXAudio2SourceVoice* musicSourceVoice;

			// Audio data buffer.
			std::vector<BYTE> soundData;
			std::vector<BYTE> musicData;

			// Whether audio is currently playing, or not.
			bool soundPlaying = false;
			bool musicPlaying = false;

			// Critical section to prevent thread contention with callbacks.
			std::mutex criticalSection;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(PlaySoundAction);

			void InitXAudio();

			void PlaySound(_In_ const std::wstring& filename);
			void PlayMusic(_In_ const std::wstring& filename);

			void StartVoice(
				_In_ const LPCWSTR url,
				_In_ IXAudio2* engine,
				_In_ IXAudio2MasteringVoice* masteringVoice,
				_In_ IXAudio2SourceVoice** sourceVoice,
				float volume = 1.0f,
				bool loop = false
				);

			STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 bytesRequired);
			STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS);
			STDMETHOD_(void, OnStreamEnd) (THIS);
			STDMETHOD_(void, OnBufferStart) (THIS_ void* bufferContext);
			STDMETHOD_(void, OnBufferEnd) (THIS_ void* bufferContext);
			STDMETHOD_(void, OnLoopEnd) (THIS_ void* bufferContext);
			STDMETHOD_(void, OnVoiceError) (THIS_ void* bufferContext, HRESULT error);
		};
	}
}
