#define MINIAUDIO_IMPLEMENTATION
#include "..\include\miniaudio.h"
#include "audio.h"
#include <iostream>



AudioManager& AudioManager::Instance()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::Init()
{
    return ma_engine_init(NULL, &engine) == MA_SUCCESS;
}

void AudioManager::Shutdown()
{
    // Uninit all loaded sounds
    for (auto& pair : sounds)
    {
        ma_sound_uninit(&pair.second);
    }
    sounds.clear();

    ma_engine_uninit(&engine);
}

bool AudioManager::LoadSound(const std::string& name, const std::string& path)
{
    if (sounds.find(name) != sounds.end())
        return true;

    // Create entry in map first
    auto result = sounds.emplace(name, ma_sound{});
    auto it = result.first;
    bool inserted = result.second;

    if (!inserted)
        return false;

    ma_result resultSound = ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &it->second); // initialize directly in map
        

    if (resultSound != MA_SUCCESS)
    {
        sounds.erase(it);
        std::cout << "Failed to load sound: " << name << "\nError code " << resultSound << std::endl;
        return false;
    }

    return true;
}

bool AudioManager::SetSoundVolume(const std::string& name, float volume)
{
    auto it = sounds.find(name);

    if (it == sounds.end())
    {
        std::cout << "SetSoundVolume failed: sound not found: " << name << std::endl;
        return false;
    }

    ma_sound_set_volume(&it->second, volume);

    return true;
}

void AudioManager::SoundSeek(const std::string& name, float startSecond)
{
    auto it = sounds.find(name);

    if (it != sounds.end())
    {
        ma_sound_seek_to_second(&it->second, startSecond);
    }
}
void AudioManager::SoundSeek(const std::string& name, unsigned frameStart)
{
    auto it = sounds.find(name);

    if (it != sounds.end())
    {
        ma_sound_seek_to_pcm_frame(&it->second, frameStart);
    }
}

void AudioManager::SoundPlay(const std::string& name)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        ma_sound_start(&it->second);
    }
    else
    {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}
void AudioManager::SoundPlay(const std::string& name, float msStart) 
{
    SoundSeek(name, msStart);
    SoundPlay(name);
}
void AudioManager::SoundPlay(const std::string& name, unsigned frameStart) 
{
    SoundSeek(name, frameStart);
    SoundPlay(name);
}

void AudioManager::SoundStop(const std::string& name)
{
    auto it = sounds.find(name);

    if (it != sounds.end())
    {
        ma_sound_stop(&it->second);
    }
}

void AudioManager::SetLooping(const std::string& name, bool loop)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        ma_sound_set_looping(&it->second, loop ? MA_TRUE : MA_FALSE);
    }
}

void AudioManager::SetSoundPosition(const std::string& name, float x, float y, float z)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        ma_sound_set_position(&it->second, x, y, z);
    }
}
void AudioManager::SetListenerPosition(float x, float y, float z) {
    ma_engine_listener_set_position(&engine, 0, x, y, z);
}
void AudioManager::SetListenerDirection(float x, float y, float z) {
    ma_engine_listener_set_direction(&engine, 0, x, y, z);
}

void AudioManager::SetListenerCone(float innerRadiusRadians, float outerRadiusRadians, float attenuation)
{
    ma_engine_listener_set_cone(&engine, 0, innerRadiusRadians, outerRadiusRadians, attenuation);
}