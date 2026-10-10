#if !defined(CORE_PLATFORM_H)
#define CORE_PLATFORM_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#if defined(__linux__)

    #include <EGL/egl.h>
    #include <wayland-egl.h>
    #include <wayland-client.h>
    #include <wayland/xdg-shell-client-protocol.h>

    #define CORE_PLATFORM_LINUX                     1

#endif // __linux__

#if defined(_WIN32) || defined(_WIN64)

    #define WIN32_LEAN_AND_MEAN                     1
    #include <windows.h>

    #define CORE_PLATFORM_WINDOWS                   1

#endif // _WIN32 || _WIN64

#endif // CORE_PLATFORM_H