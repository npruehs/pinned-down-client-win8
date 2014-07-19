
#include "pch.h"

#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

#include "Core\Event.h"
#include "Systems\SoundSystem.h"
#include "Helpers\DirectXHelper.h"
#include "Events\PointerPressedEvent.h"

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Events;

// Sample rate used for mastering voices the your game. Depends on factors such as asset sample rates and performance considerations.
#define SOUND_SYSTEM_SAMPLE_RATE 44100

SoundSystem::SoundSystem()
{
}

void SoundSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), PointerPressedEvent::PointerPressedEventType);

	// Create devices.
	this->InitXAudio();
}

void SoundSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == PointerPressedEvent::PointerPressedEventType)
	{
		this->PlaySound(L"assets/chord.wav");
	}
}

void SoundSystem::InitXAudio()
{
	// Create instances of the XAudio2 engine.
	DX::ThrowIfFailed(XAudio2Create(&this->soundAudioEngine, 0, XAUDIO2_DEFAULT_PROCESSOR));
	DX::ThrowIfFailed(XAudio2Create(&this->musicAudioEngine, 0, XAUDIO2_DEFAULT_PROCESSOR));

	// Create mastering voices.
	DX::ThrowIfFailed(
		this->soundAudioEngine->CreateMasteringVoice(
		&this->soundMasteringVoice,	// Pointer to the new IXAudio2MasteringVoice object.
		XAUDIO2_DEFAULT_CHANNELS,   // Number of channels the mastering voice expects in its input audio.
		SOUND_SYSTEM_SAMPLE_RATE,   // Sample rate of the input audio data of the mastering voice.
		0,                          // Must be 0.
		nullptr,                    // Use the default audio device.
		nullptr,                    // No effect chain on the mastering voice.
		AudioCategory_GameEffects)
		);

	// Categorize game background music properly in order to meet Windows Store certification requirements.
	// Allows Windows to properly select the background audio the user chooses to play when the user runs multiple background audio apps.
	DX::ThrowIfFailed(
		this->musicAudioEngine->CreateMasteringVoice(
		&this->musicMasteringVoice,	// Pointer to the new IXAudio2MasteringVoice object.
		XAUDIO2_DEFAULT_CHANNELS,   // Number of channels the mastering voice expects in its input audio.
		SOUND_SYSTEM_SAMPLE_RATE,   // Sample rate of the input audio data of the mastering voice.
		0,                          // Must be 0.
		nullptr,                    // Use the default audio device.
		nullptr,                    // No effect chain on the mastering voice.
		AudioCategory_GameMedia)
		);
}

SoundSystem::~SoundSystem()
{
	// Destroy sound effect engine.
	if (this->soundSourceVoice)
	{
		// All source voices must be stoped before you can stop the mastering voice.
		this->soundSourceVoice->Stop();
		this->soundSourceVoice = nullptr;
	}

	if (this->soundMasteringVoice)
	{
		this->soundMasteringVoice->DestroyVoice();
		this->soundMasteringVoice = nullptr;
	}

	if (this->soundAudioEngine)
	{
		this->soundAudioEngine->StopEngine();
		this->soundAudioEngine = nullptr;
	}

	// Destroy music engine.
	if (this->musicSourceVoice)
	{
		// All source voices must be stoped before you can stop the mastering voice.
		this->musicSourceVoice->Stop();
		this->musicSourceVoice = nullptr;
	}

	if (this->musicMasteringVoice)
	{
		this->musicMasteringVoice->DestroyVoice();
		this->musicMasteringVoice = nullptr;
	}

	if (this->musicAudioEngine)
	{
		this->musicAudioEngine->StopEngine();
		this->musicAudioEngine = nullptr;
	}
}

void SoundSystem::PlaySound(_In_ const std::wstring& file)
{
	std::lock_guard<std::mutex> lock(this->criticalSection);

	// Validate parameters.
	if ((file.size() < 1))
	{
		DX::ThrowIfFailed(E_INVALIDARG);
	}

	if (this->soundPlaying)
	{
		this->soundSourceVoice->Stop();
		this->soundData.clear();
	}

	this->StartVoice(
		file.c_str(),
		this->soundAudioEngine.Get(),
		this->soundMasteringVoice,
		this->soundData,
		&this->soundSourceVoice
		);

	this->soundPlaying = true;
}

void SoundSystem::PlayMusic(_In_ const std::wstring& file)
{
	std::lock_guard<std::mutex> lock(this->criticalSection);

	// Validate parameters.
	if ((file.size() < 1))
	{
		DX::ThrowIfFailed(E_INVALIDARG);
	}

	if (this->musicPlaying)
	{
		this->musicSourceVoice->Stop();
		this->musicData.clear();
	}

	this->StartVoice(
		file.c_str(),
		this->musicAudioEngine.Get(),
		this->musicMasteringVoice,
		this->musicData,
		&this->musicSourceVoice
		);

	this->musicPlaying = true;
}

void SoundSystem::StartVoice(
	_In_ const LPCWSTR url,
	_In_ IXAudio2* engine,
	_In_ IXAudio2MasteringVoice* masteringVoice,
	_In_ std::vector<BYTE>& resultData,
	_In_ IXAudio2SourceVoice** sourceVoice
	)
{
	// Check arguments.
	if (sourceVoice == nullptr)
	{
		DX::ThrowIfFailed(E_INVALIDARG);
	}

	// Create a Media Foundation object and process the media file.
	Microsoft::WRL::ComPtr<IMFSourceReader> reader;
	{
		// Initialize Microsoft Media Foundation.
		DX::ThrowIfFailed(MFStartup(MF_VERSION));

		// Create the source reader on the specified url. This will file be loaded entirely into memory,
		// so the low latency attribute is not required here; if you are attempting to stream
		// sound effects from disk, the low latency attribute should be set.
		DX::ThrowIfFailed(MFCreateSourceReaderFromURL(url, nullptr, &reader));

		// Set the decoded output format as PCM.
		// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
		// When using MF, this sample always decodes into PCM.
		// Note the inbox ADPCM decoder supports sample rates only up to 44.1KHz. 
		Microsoft::WRL::ComPtr<IMFMediaType> mediaType;
		DX::ThrowIfFailed(MFCreateMediaType(&mediaType));
		DX::ThrowIfFailed(mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio));
		DX::ThrowIfFailed(mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM));
		DX::ThrowIfFailed(reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, mediaType.Get()));

		// Get the complete WAVEFORMAT from the Media Type.
		Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;
		DX::ThrowIfFailed(reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, outputMediaType.GetAddressOf()));

		uint32 formatByteCount = 0;
		WAVEFORMATEX* waveFormat;
		DX::ThrowIfFailed(MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &formatByteCount));

		// Create the source voice and start it.
		DX::ThrowIfFailed(
			engine->CreateSourceVoice(
			sourceVoice,
			waveFormat,
			0,
			1.0f,
			this,
			nullptr,
			nullptr)
			);
		CoTaskMemFree(waveFormat);
	}

	// Buffer the sound data in-memory.
	for (;;)
	{
		DWORD flags = 0;
		Microsoft::WRL::ComPtr<IMFSample> sample;
		DX::ThrowIfFailed(
			reader->ReadSample(
			MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			0,
			nullptr,
			&flags,
			nullptr,
			&sample
			)
			);

		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			// End of stream.
			break;
		}

		Microsoft::WRL::ComPtr<IMFMediaBuffer> mediaBuffer;
		DX::ThrowIfFailed(
			sample->ConvertToContiguousBuffer(&mediaBuffer)
			);

		// Serialize access to the buffer by calling the IMFMediaBuffer::Lock method.
		uint8* audioData = nullptr;
		DWORD sampleBufferLength = 0;
		DX::ThrowIfFailed(mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength));

		// Append the new buffer to the running total.
		size_t lastBufferSize = resultData.size();
		resultData.resize(lastBufferSize + sampleBufferLength);
		CopyMemory(&resultData[lastBufferSize], audioData, sampleBufferLength);

		// Release the lock on the buffer.
		DX::ThrowIfFailed(mediaBuffer->Unlock());
	}

	if (resultData.size() > XAUDIO2_MAX_BUFFER_BYTES)
	{
		DX::ThrowIfFailed(E_FAIL);
	}

	// Load the audio bytes into an XAudio2 buffer.
	XAUDIO2_BUFFER playBuffer = { 0 };
	playBuffer.AudioBytes = static_cast<UINT32>(resultData.size()); // Buffer containing audio data.
	playBuffer.pAudioData = &resultData[0];							// Size of the audio buffer in bytes.
	playBuffer.pContext = *sourceVoice;

	// Submit the buffer and start the voice.
	DX::ThrowIfFailed((*sourceVoice)->SubmitSourceBuffer(&playBuffer));
	(*sourceVoice)->Start();
}

void SoundSystem::OnStreamEnd()
{
}
void SoundSystem::OnBufferStart(void* bufferContext)
{
}

void SoundSystem::OnBufferEnd(void* bufferContext)
{
	if (bufferContext == this->soundSourceVoice)
	{
		this->soundData.clear();
		this->soundPlaying = false;
	}
	else if (bufferContext == this->musicSourceVoice)
	{
		this->musicData.clear();
		this->musicPlaying = false;
	}
}

void SoundSystem::OnLoopEnd(void* bufferContext)
{
}

void SoundSystem::OnVoiceError(void* bufferContext, HRESULT error)
{
}

void SoundSystem::OnVoiceProcessingPassStart(UINT32 bytesRequired)
{
}

void SoundSystem::OnVoiceProcessingPassEnd()
{
}
