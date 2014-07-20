#pragma once

#include <thread>
#include <mutex>

#include "GameInfrastructure.h"
#include "Core\IEventListener.h"
#include "Core\GameSystem.h"

using namespace Microsoft::WRL;
using namespace PinnedDownClient::Core;

namespace PinnedDownClient
{
	namespace Systems
	{
		class SoundSystem : public Core::GameSystem, public Core::IEventListener, public IXAudio2VoiceCallback
		{
		public:
			SoundSystem();
			~SoundSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

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
			bool soundPlaying;
			bool musicPlaying;

			// Critical section to prevent thread contention with callbacks.
			std::mutex criticalSection;

			void OnEvent(Event & event);

			void InitXAudio();
			void LoadResources();
			void UnloadResources();

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
