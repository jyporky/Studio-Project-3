/**
 CSoundController
 @brief A class which manages the sound objects
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "SingletonTemplate.h"

// Include GLEW
#include <includes/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

// Include string
#include <string>
#include "MyMath.h"
// Include map storage
#include <map>

using namespace std;

// Include SoundInfo class; it stores the sound and other information
#include "SoundInfo.h"

struct ButtonData
{
	std::string fileName;
	unsigned textureID;
};

class CSoundController : public CSingletonTemplate<CSoundController>
{
	friend CSingletonTemplate<CSoundController>;
public:

	// Initialise this class instance
	bool Init(void);

	// Load a sound
	bool LoadSound(	string filename, 
					const int ID,
					const bool bIsLooped = false,
					CSoundInfo::MUSICTYPE eMusicType = CSoundInfo::MUSICTYPE::NOT_MUSIC,
					const bool bPreload = true,
					CSoundInfo::SOUNDTYPE eSoundType = CSoundInfo::SOUNDTYPE::_2D,
					vec3df vec3dfSoundPos = vec3df(0.0f, 0.0f, 0.0f));

	// Play a sound by its ID
	void PlaySoundByID(const int ID);

	void StopPlayByID(const int ID);

	void SetMasterVolume(double volume);

	double GetMasterVolume(void);

	void SetMusicVolume(double volume);

	double GetMusicVolume(void);

	void SetSFXVolume(double volume);

	double GetSFXVolume(void);

	// Increase Master volume
	bool MasterVolumeIncrease(void);
	// Decrease Master volume
	bool MasterVolumeDecrease(void);

	// Increase volume of a ISoundSource
	bool VolumeIncrease(const int ID);
	// Decrease volume of a ISoundSource
	bool VolumeDecrease(const int ID);

	void UpdateSounds(double dt);

	void SoundFadeout(const int ID, double time2fade);

	void SoundFadeIn(const int ID, double time2fade);

	void ChangeMusic(unsigned ID);

	// For 3D sounds only
	// Set Listener position
	void SetListenerPosition(const float x, const float y, const float z);
	// Set Listener direction
	void SetListenerDirection(const float x, const float y, const float z);

protected:
	// Constructor
	CSoundController(void);

	unsigned CurrMusic;

	// Get an sound from this map
	CSoundInfo* GetSound(const int ID);
	// Destructor
	virtual ~CSoundController(void);

	// Remove an sound from this map
	bool RemoveSound(const int ID);
	// Get the number of sounds in this map
	int GetNumOfSounds(void) const;

	// The handler to the irrklang Sound Engine
	ISoundEngine* cSoundEngine;

	float MusicVolume = 1;
	float SFXVolume = 1;

	// The map of all the entity created
	std::map<int, CSoundInfo*> soundMap;

	// For 3D sound only: Listener position
	vec3df vec3dfListenerPos;
	// For 3D sound only: Listender view direction
	vec3df vec3dfListenerDir;

	
};

