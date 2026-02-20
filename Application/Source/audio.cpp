#define MINIAUDIO_IMPLEMENTATION
#include "..\include\miniaudio.h"
#include "audio.h"

static ma_engine gEngine;

bool Audio_Init()
{
    return ma_engine_init(NULL, &gEngine) == MA_SUCCESS;
}

void Audio_Shutdown()
{
    ma_engine_uninit(&gEngine);
}

ma_engine* Audio_GetEngine()
{
    return &gEngine;
}