#pragma once
#include "include\miniaudio.h"

// initialize and shutdown
bool Audio_Init();
void Audio_Shutdown();

// access the engine
ma_engine* Audio_GetEngine();