//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <xaudio2.h>
#include <thread>
#include <mutex>
#include <roapi.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

using namespace Windows::System::Threading;
using namespace Windows::Foundation;

namespace PinnedDownClient
{
    //
    // The SoundPlayer class enables playing an effect or music.
    // 
    // Usage: 
    // 1. Create: m_player = std::shared_ptr<AudioManager>( new AudioManager() );
    // 4. Play a sound: m_player->PlaySound( filename );
    // 3. Play a music: m_player->PlayMusic( filename );
    //
    // To destroy, let go out of scope (or reset the smart pointer). Destroy on suspend,
    // recreate on resume.
    // 
    // Create only one instance of SoundPlayer during the lifetime of the game loop.
    //
    class SoundPlayer sealed : public IXAudio2VoiceCallback
    {
    public:
        SoundPlayer();
        ~SoundPlayer();

        // Public methods for playing sound effect or music.
        HRESULT PlaySound   ( _In_ const std::wstring& filename );
        HRESULT PlayMusic   ( _In_ const std::wstring& filename );

        // Public methods for app lifecycle.
        void Suspend();
        void Resume();

        // XAudio source voice callbacks.
        STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 bytesRequired);
        STDMETHOD_(void, OnVoiceProcessingPassEnd)   (THIS);
        STDMETHOD_(void, OnStreamEnd)   (THIS);
        STDMETHOD_(void, OnBufferStart) (THIS_ void* bufferContext);
        STDMETHOD_(void, OnBufferEnd)   (THIS_ void* bufferContext);
        STDMETHOD_(void, OnLoopEnd)     (THIS_ void* bufferContext);
        STDMETHOD_(void, OnVoiceError)  (THIS_ void* bufferContext, HRESULT error);

    private:
        // Callback. Activated when a voice has started.
        HRESULT StartVoice(
            _In_ const LPCWSTR url, 
            _In_ IXAudio2* engine,
            _In_ IXAudio2MasteringVoice* masteringVoice,
            _In_ std::vector<BYTE>& resultData,
            _In_ IXAudio2SourceVoice** sourceVoice
            );

        // Variables for the sound voice.
        std::vector<BYTE>                m_effectData;
        IXAudio2SourceVoice*             m_effectSourceVoice;
        IXAudio2MasteringVoice*          m_effectMasteringVoice;
        Microsoft::WRL::ComPtr<IXAudio2> m_effectAudioEngine;

        // Variables for the music voice.
        std::vector<BYTE>                m_musicData;
        IXAudio2SourceVoice*             m_musicSourceVoice;
        IXAudio2MasteringVoice*          m_musicMasteringVoice;
        Microsoft::WRL::ComPtr<IXAudio2> m_musicAudioEngine;

        // These are used to track when the voices are already playing something.
        bool m_effectBusy;
        bool m_musicBusy;

        // Critical section to prevent thread contention with callbacks.
        std::mutex                       m_criticalSection;
    };
}
