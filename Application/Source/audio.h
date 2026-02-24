#pragma once
#include "..\include\miniaudio.h"
#include <string>
#include <map>

/*
--- GUIDE ON ADDING TO SCENE ---
1) include "audio.h"



--- GUIDE ON ADDING SFX ---
1) Add the sound file to SFX/... (make sure it's on GitHub and everything)

2) Initialize the sounds in the scene's Init().
   Example initialization of soundName with a file name of "sound.wav":
        AudioManager::Instance().LoadSound("soundName", "SFX/sound.wav");



--- GUIDE ON PLAYING SFX ---
1) Sounds do not automatically start from the beginning of the file. 
   Use SoundSeek on sound to return to start, or specify a value in seconds
2) Use SoundPlay to start playing a sound
3) Use SoundStop to stop playing



--- SPATIAL AUDIO INIT GUIDE ---
1) Directional attenuation is supported. Adjust the cone of attenuation with:
	SetListenerCone(innerRadius, outerRadius, attenuation);
Within the inner radius, sound plays at normal volume.
Outside the outer radius, sound is attenuated (softened) by the specified value.
In-between, the volume is attenuated gradually.
*/



class AudioManager {
public:
    // Access the singleton instance
    //  AudioManager::Instance()
    static AudioManager& Instance();

    // Initialize/shutdown engine
    bool Init();
    void Shutdown();



    bool LoadSound(const std::string& name, const std::string& path); // Load a sound file by name
    
    // Play a loaded sound (does NOT rewind automatically)
    //  Overload to seek+play in one function call
    void SoundPlay(const std::string& name); 
    void SoundPlay(const std::string& name, float startSecond);
    void SoundPlay(const std::string& name, unsigned frameStart);

    // SoundSeek will rewind to 0 by default
    void SoundSeek(const std::string& name, float startSecond = 0.f);
    void SoundSeek(const std::string& name, unsigned frameStart = 0);

    void SoundStop(const std::string& name); // Stop sound



    // Spatial audio functions
    void SetSoundPosition(const std::string& name, float x, float y, float z);

    void SetListenerPosition(float x, float y, float z);
    void SetListenerDirection(float x, float y, float z);

    void SetListenerCone(float innerRadiusRadians, float outerRadiusRadians, float attenuation);



private:
    AudioManager() = default;
    ~AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    ma_engine engine{};
    std::map<std::string, ma_sound> sounds;
};