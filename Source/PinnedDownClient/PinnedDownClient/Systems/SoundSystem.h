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
			void PlaySound(_In_ const std::wstring& filename);

		private:
			// Instance of the XAudio2 engine for playing sounds.
			Microsoft::WRL::ComPtr<IXAudio2> soundAudioEngine;
			Microsoft::WRL::ComPtr<IXAudio2> musicAudioEngine;

			// Encapsulates an audio device. Ultimate destination for all audio that passes through an audio graph.
			IXAudio2MasteringVoice* soundMasteringVoice;
			IXAudio2MasteringVoice* musicMasteringVoice;

			IXAudio2SourceVoice* soundSourceVoice;
			IXAudio2SourceVoice* musicSourceVoice;

			std::vector<BYTE> soundData;
			std::vector<BYTE> musicData;

			bool soundPlaying;
			bool musicPlaying;

			// Critical section to prevent thread contention with callbacks.
			std::mutex criticalSection;

			void OnEvent(Event & event);

			void InitXAudio();

			void StartVoice(
				_In_ const LPCWSTR url,
				_In_ IXAudio2* engine,
				_In_ IXAudio2MasteringVoice* masteringVoice,
				_In_ std::vector<BYTE>& resultData,
				_In_ IXAudio2SourceVoice** sourceVoice
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
