#pragma once

#ifndef H_AUDIOMANAGER
#define H_AUDIOMANAGER

#include <string>
#include <Windows.h>
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_codec.h"
#include "fmod_dsp.h"
#include "fmod_errors.h"
#include "fmod_memoryinfo.h"
#include "fmod_output.h"

#include "ResourceManager.h"

class AudioManager
{
public:
	// Destructor
	~AudioManager(void);

	void Initialize(void);	// Initialize sound components
	void Shutdown(void);	// Shutdown sound components

	// Singleton instance manip methods
	static AudioManager* GetInstance(void);
	static void DestroyInstance(void);

	// Accessors
	FMOD::System* GetSystem(void)
		{return soundSystem;}

	// Sound playing
	void Play(FMOD::Sound* sound);	// Play a sound/music with default channel
	void PlaySFX(FMOD::Sound* sound);	// Play a sound effect with custom channel
	void PlayBGM(FMOD::Sound* sound);	// Play background music with custom channel

	// Volume adjustment methods
	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume);

private:
	static AudioManager* instance;	// Singleton instance
	AudioManager(void);	 // Constructor

	FMOD::System* soundSystem;	// Sound system object
	FMOD_RESULT result;
	FMOD::Channel* bgmChannel;	// Channel for background music
	static const int numSfxChannels = 4;
	FMOD::Channel* sfxChannels[numSfxChannels];	// Channel for sound effects
};

//-------------------------------------------------------------------------------
//===============================================================================
//---------------------------Audio Resource class----------------------------
//===============================================================================
class cAudioResource : public cResource
{
public:
	inline cAudioResource()
	{
		m_ResourceID = m_Scope = 0;
		m_Type = RESOURCE_AUDIO;
	}

	~cAudioResource();
	void load();
	void unload();

	// Get access for the sound variable
	FMOD::Sound* GetSound(void){ return p_Sound;}

private:
	FMOD::Sound* p_Sound;
};
#endif