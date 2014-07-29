
#include "pch.h"

#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

#include "Core\Event.h"
#include "Systems\SoundSystem.h"
#include "Util\DirectXUtils.h"
#include "Events\PointerPressedEvent.h"
#include "Core\Resources\AudioResourceHandle.h"

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Core::Resources;
using namespace PinnedDownClient::Util;

// Sample rate used for mastering voices the your game. Depends on factors such as asset sample rates and performance considerations.
#define SOUND_SYSTEM_SAMPLE_RATE 44100

SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{
	this->UnloadResources();

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

void SoundSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, PointerPressedEvent::PointerPressedEventType);

	// Create devices.
	this->InitXAudio();

	this->LoadResources();
}

void SoundSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == PointerPressedEvent::PointerPressedEventType)
	{
		this->PlaySound(L"Assets/chord.wav");
	}
}

void SoundSystem::InitXAudio()
{
	// Create instances of the XAudio2 engine.
	ThrowIfFailed(XAudio2Create(&this->soundAudioEngine, 0, XAUDIO2_DEFAULT_PROCESSOR));
	ThrowIfFailed(XAudio2Create(&this->musicAudioEngine, 0, XAUDIO2_DEFAULT_PROCESSOR));

	// Create mastering voices.
	ThrowIfFailed(
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
	ThrowIfFailed(
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

void SoundSystem::LoadResources()
{
	this->game->resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/chord.wav"
		);
}

void SoundSystem::UnloadResources()
{
	this->game->resourceManager->UnloadResource(L"Assets/chord.wav");
}

void SoundSystem::PlaySound(_In_ const std::wstring& file)
{
	std::lock_guard<std::mutex> lock(this->criticalSection);

	if (this->soundPlaying)
	{
		this->soundSourceVoice->Stop();
	}

	this->StartVoice(
		file.c_str(),
		this->soundAudioEngine.Get(),
		this->soundMasteringVoice,
		&this->soundSourceVoice
		);

	this->soundPlaying = true;
}

void SoundSystem::PlayMusic(_In_ const std::wstring& file)
{
	std::lock_guard<std::mutex> lock(this->criticalSection);

	if (this->musicPlaying)
	{
		this->musicSourceVoice->Stop();
	}

	this->StartVoice(
		file.c_str(),
		this->musicAudioEngine.Get(),
		this->musicMasteringVoice,
		&this->musicSourceVoice
		);

	this->musicPlaying = true;
}

void SoundSystem::StartVoice(
	_In_ const LPCWSTR url,
	_In_ IXAudio2* engine,
	_In_ IXAudio2MasteringVoice* masteringVoice,
	_In_ IXAudio2SourceVoice** sourceVoice,
	float volume,
	bool loop
	)
{
	// Check arguments.
	if (sourceVoice == nullptr)
	{
		ThrowIfFailed(E_INVALIDARG);
	}

	// Get audio resource.
	std::shared_ptr<AudioResourceHandle> audioResource = this->game->resourceManager->GetResource<AudioResourceHandle>(url);

	// Create the source voice and start it.
	ThrowIfFailed(
		engine->CreateSourceVoice(
		sourceVoice,
		audioResource->waveFormat,
		0,
		1.0f,
		this,
		nullptr,
		nullptr)
		);

	// Load the audio bytes into an XAudio2 buffer.
	std::vector<BYTE>& audioBuffer = *audioResource->audioBuffer;

	XAUDIO2_BUFFER playBuffer = { 0 };
	playBuffer.AudioBytes = static_cast<UINT32>(audioBuffer.size());	// Buffer containing audio data.
	playBuffer.pAudioData = &audioBuffer[0];							// Size of the audio buffer in bytes.
	playBuffer.pContext = *sourceVoice;
	playBuffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	// Submit the buffer and start the voice.
	ThrowIfFailed((*sourceVoice)->SubmitSourceBuffer(&playBuffer));
	(*sourceVoice)->SetVolume(volume);
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
		this->soundPlaying = false;
	}
	else if (bufferContext == this->musicSourceVoice)
	{
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
