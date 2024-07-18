/**
 * Compile on Windows/MacOS without changing code.
 */
#pragma once

#define SystemInit()

#ifdef _WIN32
#include <windows.h>
#define Delay_Ms(milliseconds) Sleep(milliseconds)
#elif defined(__APPLE__)
#define Delay_Ms(milliseconds) usleep(milliseconds * 1000)
#endif
