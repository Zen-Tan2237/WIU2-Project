#pragma once
#include "..\include\miniaudio.h"
#include <string>
#include <map>

/*
--- GUIDE ON ADDING TO SCENE ---
1) include "audio.h"



--- GUIDE ON ADDING SFX ---
1) Add the sound file to SFX (make sure it's on GitHub and everything)

2) Declare a ma_sound for that SFX (e.g. ma_sound pop) at the top of the scene.cpp

3a) Call ma_sound_init_from_file to load the sound (see pop in SceneTester for reference); use the same ma_sound when calling
    3b) Optionally use a ma_result (like in SceneTester) to confirm the sound loads properly 

4) Call ma_sound_uninit on the ma_sound in the scene exit



--- GUIDE ON PLAYING SFX ---
1) Sounds do not automatically start from the beginning of the file. Use ma_sound_seek_to_pcm_frame(&sound, 0) on sound to return to start
2) Use ma_sound_start to start playing a sound
3) Use ma_sound_stop to stop playing



--- SPATIAL AUDIO IMPLEMENTATION GUIDE ---
1) Set the position of &sound with:
	ma_sound_set_position(&sound, x, y, z);

2) Set the player's (listener's) position with:
	ma_engine_listener_set_position(Audio_GetEngine(), 0, x, y, z);

3) Set the listener's direction with:
	ma_engine_listener_set_direction(Audio_GetEngine(), 0, x, y, z);



--- SPATIAL AUDIO INIT GUIDE ---
1) Directional attenuation is supported. Adjust the cone of attenuation with:
	ma_engine_listener_set_cone(Audio_GetEngine(), 0, innerRadius, outerRadius, attenuation);
Within the inner radius, sound plays at normal volume.
Outside the outer radius, sound is attenuated (softened) by the specified value.
In-between, the volume is attenuated gradually.
*/

//// initialize and shutdown
//bool Audio_Init();
//void Audio_Shutdown();
// 
//// access the engine
//ma_engine* Audio_GetEngine();



class AudioManager {
public:
    // Access the singleton instance
    static AudioManager& Instance();

    // Initialize/shutdown engine
    bool Init();
    void Shutdown();

    // Load a sound file by name (stores internally)
    bool LoadSound(const std::string& name, const std::string& path);

    // Play a loaded sound
    void Play(const std::string& name);

    // Optional: set 3D position for spatial audio
    void SetSoundPosition(const std::string& name, float x, float y, float z);

private:
    AudioManager() = default;
    ~AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    ma_engine engine;
    std::map<std::string, ma_sound> sounds;
};