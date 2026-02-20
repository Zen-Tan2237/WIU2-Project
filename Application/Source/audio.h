#pragma once
#include "..\include\miniaudio.h"

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
*/

// initialize and shutdown
bool Audio_Init();
void Audio_Shutdown();

// access the engine
ma_engine* Audio_GetEngine();