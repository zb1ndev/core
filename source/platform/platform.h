#if !defined(CORE_PLATFORM_H)
#define CORE_PLATFORM_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#define internal                                    static
#define local_persist                               static

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

typedef uint8_t cplatform_t;

#define CORE_HAS_STDATOMIC                          (0U)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #if !defined(__STDC_NO_ATOMICS__)
        #include <stdatomic.h>
        #undef CORE_HAS_STDATOMIC
        #define CORE_HAS_STDATOMIC                  (1<<4)
    #endif // __STDC_NO_ATOMICS__
#endif // __STDC_VERSION__

#define CORE_ARCH_64BIT                             (0U)
#define CORE_ARCH_32BIT                             (0U)
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__)
    #undef CORE_ARCH_64BIT
    #define CORE_ARCH_64BIT                         (1<<3)
#else
    #undef CORE_ARCH_32BIT
    #define CORE_ARCH_32BIT                         (1<<2)
#endif // _WIN64 || __x86_64__ || __ppc64__ || __aarch64__

#define CORE_PLATFORM_LINUX                         (0U)
#define CORE_PLATFORM_WINDOWS                       (0U)
#if defined(__linux__)

    #include <EGL/egl.h>
    #include <wayland-egl.h>
    #include <wayland-client.h>
    #include <wayland/xdg-shell-client-protocol.h>

    #undef CORE_PLATFORM_LINUX
    #define CORE_PLATFORM_LINUX                     (1<<1)

#elif defined(_WIN32) || defined(_WIN64)

    #define WIN32_LEAN_AND_MEAN                     1
    #include <windows.h>

    #undef CORE_PLATFORM_WINDOWS
    #define CORE_PLATFORM_WINDOWS                   (1<<0)

#endif // _WIN32 || _WIN64 || __linux__

internal inline uint8_t         core_get_platform_mask       (void) {
    return  CORE_HAS_STDATOMIC | CORE_ARCH_64BIT | 
            CORE_ARCH_32BIT | CORE_PLATFORM_LINUX | 
            CORE_PLATFORM_WINDOWS;
}

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_PLATFORM_H