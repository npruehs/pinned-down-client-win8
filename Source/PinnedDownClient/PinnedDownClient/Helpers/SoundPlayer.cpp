//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "SoundPlayer.h"
#include "DirectXHelper.h"

using namespace Windows::Foundation;
using namespace PinnedDownClient;

// Here, we fix the sample rate at 44100. The sample rate used for mastering
// voices in your game will depend on factors such as asset sample rates and
// performance considerations.
#define SOUND_PLAYER_SAMPLE_RATE 44100

// SoundPlayer ctor.
SoundPlayer::SoundPlayer() :
    m_musicMasteringVoice(nullptr),
    m_effectMasteringVoice(nullptr),
    m_effectAudioEngine(nullptr),
    m_musicAudioEngine(nullptr),
    m_effectBusy(false),
    m_musicBusy(false)
{
    HRESULT hr = S_OK;
    UINT32 flags = 0;

    DX::ThrowIfFailed(
        XAudio2Create(&m_effectAudioEngine, flags)
        );

#if defined(_DEBUG)
    // Enable XAudio2 debug logging for the debug build.
    XAUDIO2_DEBUG_CONFIGURATION debugConfiguration = { 0 };
    debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
    debugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS;
    m_effectAudioEngine->SetDebugConfiguration(&debugConfiguration);
#endif

    // This class can play game sound effects. Note that these are categorised
    // as AudioCategory_GameEffects.
    DX::ThrowIfFailed(
        m_effectAudioEngine->CreateMasteringVoice(
            &m_effectMasteringVoice,    // Pointer to the new IXAudio2MasteringVoice object.
            XAUDIO2_DEFAULT_CHANNELS,   // Number of channels the mastering voice expects in its input audio.
            SOUND_PLAYER_SAMPLE_RATE,   // Sample rate of the input audio data of the mastering voice.
            0,                          // Must be 0.
            nullptr,                    // Use the default audio device.
            nullptr,                    // No effect chain on the mastering voice.
            AudioCategory_GameEffects)
            );
    

    DX::ThrowIfFailed(
        XAudio2Create(&m_musicAudioEngine, flags)
        );

#if defined(_DEBUG)
    // Enable XAudio2 debug logging for the debug build.
    m_musicAudioEngine->SetDebugConfiguration(&debugConfiguration);
#endif

    // This class can play the equivalent of background music, which is categorized
    // as AudioCategory_GameMedia.
    // 
    // Note to developer: You must categorize game background music properly
    // in order to meet Windows Store certification requirements. This
    // allows Windows to properly select the background audio the user chooses
    // to play when the user runs multiple background audio apps.
    DX::ThrowIfFailed(
        m_musicAudioEngine->CreateMasteringVoice(
            &m_musicMasteringVoice,     // Pointer to the new IXAudio2MasteringVoice object.
            XAUDIO2_DEFAULT_CHANNELS,   // Number of channels the mastering voice expects in its input audio.
            SOUND_PLAYER_SAMPLE_RATE,   // Sample rate of the input audio data of the mastering voice.
            0,                          // Must be 0.
            nullptr,                    // Use the default audio device.
            nullptr,                    // No effect chain on the mastering voice.
            AudioCategory_GameMedia)
            );
}

// Uninitializes the SoundPlayer by destroying the mastering voice and XAudio2 engine.
// Stops, then removes all sound files from the collection before destroying resources. 
SoundPlayer::~SoundPlayer()
{
    // Destroy sound effect engine.
    if (m_effectSourceVoice)
    {
        // All source voices must be stoped before you can stop the mastering voice.
        m_effectSourceVoice->Stop();
        m_effectSourceVoice = nullptr;
    }
    if (m_effectMasteringVoice)
    {
        m_musicMasteringVoice->DestroyVoice();
        m_musicMasteringVoice = nullptr;
    }
    if (m_effectAudioEngine)
    {
        m_effectAudioEngine->StopEngine();
        m_effectAudioEngine = nullptr;
    }

    // Destroy music engine.
    if (m_musicSourceVoice)
    {
        // All source voices must be stoped before you can stop the mastering voice.
        m_musicSourceVoice->Stop();
        m_musicSourceVoice = nullptr;
    }
    if (m_musicMasteringVoice)
    {
        m_musicMasteringVoice->DestroyVoice();
        m_musicMasteringVoice = nullptr;
    }
    if (m_musicAudioEngine)
    {
        m_musicAudioEngine->StopEngine();
        m_musicAudioEngine = nullptr;
    }
}

// Loads and plays a sound once. If a sound is already playing,
// it will be stopped and the specified file will be played.
HRESULT SoundPlayer::PlaySound(_In_ const std::wstring& file)
{
    HRESULT hr = S_OK;

    std::lock_guard<std::mutex> lock(m_criticalSection);

    // Validate parameters.
    if ((file.size() < 1))
    {
        hr = E_INVALIDARG;
    }

    if (m_effectBusy == true)
    {
        m_effectSourceVoice->Stop();
        m_effectData.clear();
    }

    if (SUCCEEDED(hr))
    {
        hr = StartVoice(
            file.c_str(), 
            m_effectAudioEngine.Get(), 
            m_effectMasteringVoice, 
            m_effectData, 
            &m_effectSourceVoice
            );
        m_effectBusy = true;
    }

    return hr;
}

// Loads and plays a music file once. If a music file is already playing,
// it will be stopped and the specified file will be played.
HRESULT SoundPlayer::PlayMusic(_In_ const std::wstring& file)
{
    HRESULT hr = S_OK;

    std::lock_guard<std::mutex> lock(m_criticalSection);

    // Validate parameters.
    if (file.size() < 1)
    {
        hr = E_INVALIDARG;
    }

    if (m_musicBusy == true)
    {
        m_musicSourceVoice->Stop();
        m_musicData.clear();
    }

    if (SUCCEEDED(hr))
    {
        hr = StartVoice(
            file.c_str(), 
            m_musicAudioEngine.Get(), 
            m_musicMasteringVoice, 
            m_musicData, 
            &m_musicSourceVoice
            );
        m_musicBusy = true;
    }

    return hr;
}


// Internal-only method. Loads an audio file and starts a new voice playing it.
HRESULT SoundPlayer::StartVoice(
    _In_ const LPCWSTR url,
    _In_ IXAudio2* engine,
    _In_ IXAudio2MasteringVoice* masteringVoice,
    _In_ std::vector<BYTE>& resultData,
    _In_ IXAudio2SourceVoice** sourceVoice
    )
{
    HRESULT hr = S_OK;

    if (sourceVoice == nullptr)
    {
        hr = E_INVALIDARG;
    }

    // Create a Media Foundation object and read/process the media file.
    Microsoft::WRL::ComPtr<IMFSourceReader> reader;
    {
        DX::ThrowIfFailed(
            MFStartup(MF_VERSION)
            );

        // Create the source reader on the url (file). This will be loaded entirely into memory,
        // so the low latency attribute is not required here; if you are attempting to stream
        // sound effects from disk, the low latency attribute should be set.
        // If the file does not exist, this will exhibit HRESULT 0x80070002: File not found.
        DX::ThrowIfFailed(
            MFCreateSourceReaderFromURL(url, nullptr, &reader) 
            );

        // Set the decoded output format as PCM
        // XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
        // When using MF, this sample always decodes into PCM.
        // NOTE: The inbox ADPCM decoder supports sample rates only up to 44.1KHz. 
        Microsoft::WRL::ComPtr<IMFMediaType> mediaType;
        DX::ThrowIfFailed(
            MFCreateMediaType(&mediaType)
            );

        DX::ThrowIfFailed(
            mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)
            );

        DX::ThrowIfFailed(
            mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)
            );

        DX::ThrowIfFailed(
            reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, mediaType.Get())
            );

        // Get the complete WAVEFORMAT from the Media Type
        Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;
        DX::ThrowIfFailed(
            reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, outputMediaType.GetAddressOf())
            );

        uint32 formatByteCount = 0;
        WAVEFORMATEX* waveFormat;
        DX::ThrowIfFailed(
            MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &formatByteCount)
            );

        // Create the source voice and start it
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

        // End of stream
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            break;
        }

        Microsoft::WRL::ComPtr<IMFMediaBuffer> mediaBuffer;
        DX::ThrowIfFailed(
            sample->ConvertToContiguousBuffer(&mediaBuffer)
            );

        // Serialize access to the buffer by calling the IMFMediaBuffer::Lock method.
        uint8* audioData = nullptr;
        DWORD sampleBufferLength = 0;
        DX::ThrowIfFailed(
            mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength)
            );

        // Append the new buffer to the running total.
        size_t lastBufferSize = resultData.size();
        resultData.resize(lastBufferSize + sampleBufferLength);
        CopyMemory(&resultData[lastBufferSize], audioData, sampleBufferLength);

        // Release the lock on the buffer.
        DX::ThrowIfFailed(
            mediaBuffer->Unlock()
            );
    }

    if (resultData.size() > XAUDIO2_MAX_BUFFER_BYTES)
    {
        hr = E_FAIL;
    }

    // Start playing the sound.
    if (SUCCEEDED(hr))
    {
        // Load the audio bytes into an XAudio2 buffer.
        XAUDIO2_BUFFER playBuffer = { 0 };
        playBuffer.AudioBytes = static_cast<UINT32>(resultData.size());
        playBuffer.pAudioData = &resultData[0];
        playBuffer.pContext = *sourceVoice;
        
        // Submit the buffer and start the voice.
        DX::ThrowIfFailed(
            (*sourceVoice)->SubmitSourceBuffer(&playBuffer)
            );

        (*sourceVoice)->Start();
    }

    return hr;
}



//
// Callback handlers for the XAudio2 source voice, assigned in the  
// CreateSourceVoice function. MediaStream implements only
// the OnStreamEnd handler for maintaining play state (m_IsPlaying).
//

// Invoked by the XAudio2 engine when a sound is finished playing.
// Also invoked by the MediaStream::Update method when it reaches
// the end of a sound file. 
void SoundPlayer::OnStreamEnd()
{
}
void SoundPlayer::OnBufferStart(void* /*bufferContext*/)
{
}

void SoundPlayer::OnBufferEnd(void* bufferContext)
{
    if (bufferContext == m_effectSourceVoice)
    {
        m_effectData.clear();
        m_effectBusy = false;
    }

    if (bufferContext == m_musicSourceVoice)
    {
        m_musicData.clear();
        m_musicBusy = false;
    }
}

void SoundPlayer::OnLoopEnd(void* /*bufferContext*/)
{
}

void SoundPlayer::OnVoiceError(void* /*bufferContext*/, HRESULT /*error*/)
{
}

void SoundPlayer::OnVoiceProcessingPassStart(UINT32 /*bytesRequired*/)
{
}

void SoundPlayer::OnVoiceProcessingPassEnd()
{
}
