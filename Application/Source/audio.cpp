#define MINIAUDIO_IMPLEMENTATION
#include "..\include\miniaudio.h"
#include "audio.h"
#include <iostream>

//static ma_engine gEngine;
//
//bool Audio_Init()
//{
//    return ma_engine_init(NULL, &gEngine) == MA_SUCCESS;
//}
//
//void Audio_Shutdown()
//{
//    ma_engine_uninit(&gEngine);
//}
//
//ma_engine* Audio_GetEngine()
//{
//    return &gEngine;
//}



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
        return true; // Already loaded

    ma_sound sound;
    if (ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS)
    {
        std::cerr << "Failed to load sound: " << path << std::endl;
        return false;
    }

    sounds[name] = sound;
    return true;
}

void AudioManager::Play(const std::string& name)
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

void AudioManager::SetSoundPosition(const std::string& name, float x, float y, float z)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        ma_sound_set_position(&it->second, x, y, z);
    }
}