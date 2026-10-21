#if !defined(CORE_H)
#define CORE_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

/*


     ██████   ██████  ██████  ██████
    ▓▓    ▓▓ ▓▓    ▓▓ ▓▓   ▓▓ ▓▓    
    ▒▒       ▒▒    ▒▒ ▒▒▒▒▒▒  ▒▒▒▒▒ 
    ░░    ░░ ░░    ░░ ░░   ░░ ░░    
     ░░░░░░   ░░░░░░  ░░   ░░ ░░░░░░

    
    A minimalistic platform layer abstraction that provides input, 
    windowing and graphics API binding at *almost* no runtime cost. 

    MIT License
    Copyright (c) 2026 Joel Zbinden

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    //////////////////////////////////////////////////////////////////////////////

    AI DISCLAIMER: 
    The use of generative AI in this project was limited to 
    documentation and was not used for generating code.  

    //////////////////////////////////////////////////////////////////////////////

*/

#define internal                                    static
#define local_persist                               static

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#if defined(__x86_64__) || defined(_M_X64)
    #define CORE_ARCH_AMD64                         (1U)
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define CORE_ARCH_ARM64                         (1U)
#elif defined(__arm__) || (_M_ARM)
    #define CORE_ARCH_ARM32                         (1U)
#elif defined(__i386__) || defined(_M_IX86)
    #define CORE_ARCH_X86                           (1U)
#elif defined(__ia64__) || defined(_M_IA64)
    #define CORE_ARCH_IA64                          (1U)
#elif defined(__ppc__) || defined(__PPC__) || defined(__ppc64__) || defined(__PPC64__) || defined(_M_PPC)
    #define CORE_ARCH_POWERPC                       (1U)
#elif defined(__myriad2__)
    #define CORE_ARCH_MYRIAD2                       (1U)
#elif defined(__mips__)
    #define CORE_ARCH_MIPS                          (1U)
#else
    #define CORE_ARCH_UNRECOGNIZED                  (1U)
    #warning current architecture not recognized
#endif // CORE_ARCH_XX

#if defined(__ANDROID__)
    #define CORE_PLATFORM_ANDROID                   (1U)
#elif defined(__APPLE__)
    #define CORE_PLATFORM_APPLE                     (1U)
#elif defined(_WIN32)
    #define _CRT_SECURE_NO_WARNINGS                 (1U)
    #define WIN32_LEAN_AND_MEAN                     (1U)
    #define CORE_PLATFORM_WINDOWS                   (1U)
#elif defined(__linux__)
    #define CORE_PLATFORM_LINUX                     (1U)
#else
    #define CORE_PLATFORM_UNRECOGNIZED              (1U)
    #warning current platform not recognized
#endif // CORE_PLATFORM_XX

#if defined(__clang__)
    #define CORE_CC_CLANG                           (1U)
#elif defined(__GNUC__)
    #define CORE_CC_GCC                             (1U)
#elif defined(_MSC_VER)
    #define CORE_CC_MSCV                            (1U)
#elif defined(__EMSCRIPTEN__)
    #define CORE_CC_EMPSCRIPTEN                     (1U)
#elif defined(__asmjs__)
    #define CORE_CC_ASMJS                           (1U)
#elif defined(__wasm__)
    #define CORE_CC_WASM                            (1U)
#elif defined(__NVCC__)
    #define CORE_CC_NVCC                            (1U)
#else
    #define CORE_CC_UNRECOGNIZED                    (1U)
    #warning current compiler not recognized
#endif // CORE_CC_XX

#include <glad/glad.h>
#include <cglm/cglm.h>

#if defined(CORE_PLATFORM_LINUX)

    #include <EGL/egl.h>
    #include <wayland-egl.h>
    #include <wayland-client.h>
    #include <wayland/xdg-shell-client-protocol.h>

#endif // CORE_PLATFORM_LINUX

#if defined(CORE_PLATFORM_WINDOWS)

    #include <windows.h>

#endif // CORE_PLATFORM_WINDOWS

#pragma region CORE_DEBUG

    /*  Prints the line,function, error-type string, and formatted arguments to stderr.
        @param line The line number of the call.
        @param function The name of the calling function.
        @param etype The type of error as a string.
        @param fmt The format string, specifying how the variadic arguments will be formatted.
        @param ... Variadic Arguments
        @returns N/A
    */
    void                    _cdbg_eprint                (int64_t line, 
                                                        const char* function, 
                                                        const char* etype, 
                                                        const char* fmt, ...);

    #define cdbg_log(msg)           _cdbg_eprint(__LINE__, __func__, "LOG", msg)
    #define cdbg_logf(fmt, ...)     _cdbg_eprint(__LINE__, __func__, "LOG", fmt, __VA_ARGS__)

    #define cdbg_warn(msg)          _cdbg_eprint(__LINE__, __func__, "WARNING", msg)
    #define cdbg_warnf(fmt, ...)    _cdbg_eprint(__LINE__, __func__, "WARNING", fmt, __VA_ARGS__)

    #define cdbg_err(msg)           _cdbg_eprint(__LINE__, __func__, "ERROR", msg)
    #define cdbg_errf(fmt, ...)     _cdbg_eprint(__LINE__, __func__, "ERROR", fmt, __VA_ARGS__)

    typedef enum err_t err_t;
    enum err_t {

        CORE_SUCCESS        = 0,    // `err_t`: Success
        CORE_EXIT,                  // `err_t`: Exit Successfully
        CORE_PINVALID,              // `err_t`: Invalid Parameters
        CORE_WINMAXC,               // `err_t`: Max Window Count
        CORE_CREASURF,              // `err_t`: Failed to Create Surface
        CORE_XDGGETSURF,            // `err_t`: XDG Failed to Get Surface
        CORE_XDGSADDLIS,            // `err_t`: XDG Failed to Add Listener to Surface
        CORE_XDGGETTL,              // `err_t`: XDG Failed to Get Top-Level
        CORE_XDGTLADDLIS,           // `err_t`: XDG Failed to Add Listener to Top-Level
        CORE_DISPRT,                // `err_t`: Failed to do a Display Round Trip
        CORE_WINNREADY,             // `err_t`: Window Not Read / Initialized
        CORE_DISPCONREF,            // `err_t`: Display Connection Refused
        CORE_DIPREG,                // `err_t`: Display Registry Not Found
        CORE_REGADDLIS,             // `err_t`: Display Registry Failed to Add Listener
        CORE_DISPDISPAT,            // `err_t`: Display Dispatch Failure
        CORE_DISPFLUSH,             // `err_t`: Display Flush Failure
        CORE_EGLNODISP,             // `err_t`: No EGL Display Found
        CORE_EGLINIT,               // `err_t`: EGL Failed to Initialize
        CORE_EGLBIND,               // `err_t`: EGL Failed to Bind GAPI
        CORE_EGLCONFIG,             // `err_t`: EGL Failed to Load Config
        CORE_EGLCONTEXT,            // `err_t`: EGL Failed to Create Context
        CORE_EGLMAKCUR,             // `err_t`: EGL Failed to Make Context Current
        CORE_GLADLOAD,              // `err_t`: GLAD Failed to Load
        CORE_EGLCREATWIN,           // `err_t`: EGL Failed to Create Window
        CORE_EGLCREATSURF,          // `err_t`: EGL Failed to Create Surface

        CORE_ERROR_COUNT            // `err_t`: Number of Error Types

    };

    const char*             _cdbg_get_errmsg            (err_t code);

    /* If `err` of type `err_t*` is not equal to `NULL`, it sets `err` to `msg`, which should be of type `err_t`, and logs the error. 
        @param err The error object you are putting `msg` into.
        @param msg The error code you are throwing.
    */
    #define throw(err, msg)             do{if((err)!=NULL){*(err)=(msg);cdbg_err(_cdbg_get_errmsg(*err));}}while(0)

    /*  Performs `call` and checks if `err` of type `err_t*` is not `NULL` and that the value at `err` is not equal to `CORE_SUCCESS`.
        If so, run the code defined in except.
        @param err The error object you are pulling the error from.
        @param call The procedure you expect to fail.
        @param except The code you want to run on failure.
    */
    #define catch(err, call, except)    do{call;if(err!=NULL&&*err!=CORE_SUCCESS)except}while(0)

#pragma endregion // CORE_DEBUGGING

#pragma region CORE_WINDOW

    // The maximum number of windows core.h maintains handles for.
    #define CORE_CWINDOW_MAX                            (UINT8_MAX)
    
    // Invalid index into window data array.
    #define CORE_CWINDOW_INVALID                        (UINT8_MAX)

    /*  A Window Handle \ ID.
        @note Indexes into an internal array of window data.
    */ 
    typedef uint8_t cwindow_t;
    
    /*  A bitmask representing a window's state.
        @note CORE_CWINDOW_CLOSED
        @note CORE_CWINDOW_FULLSCREEN
        @note CORE_CWINDOW_MAXIMIZED
        @note CORE_CWINDOW_MINIMIZED
        @note CORE_CWINDOW_RESIZED
        @note CORE_CWINDOW_READY.
    */ 
    typedef uint64_t cwindow_state_t;

    #define CORE_CWINDOW_CLOSED                         (1<<0) // Enabled on window close
    #define CORE_CWINDOW_FULLSCREEN                     (1<<1) // Enabled when window is in fullscreen
    #define CORE_CWINDOW_MAXIMIZED                      (1<<2) // Enabled when window is maximized
    #define CORE_CWINDOW_MINIMIZED                      (1<<3) // Enabled when window is minimized
    #define CORE_CWINDOW_RESIZED                        (1<<4) // Enabled when window is resized (not handled yet)
    #define CORE_CWINDOW_READY                          (1<<5) // Enabled when window is ready to be used.
    #define CORE_CWINDOW_FOCUSED                        (1<<6) // Enabled when window is in focus.


    /*  Initializes a window and returns a handle to it.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param title The title of the window.
        @param size The size in pixels of the window surface.
        @returns A handle to the created window
    */
    cwindow_t               cwindow_create              (err_t* err, const char* title, ivec2 size);
    
    /*  Deinitializes the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to deinitialize.
        @returns N/A
    */
    void                    cwindow_destroy             (err_t* err, cwindow_t handle);

    /*  Updates input and events for all initialized windows.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @returns N/A
    */
    void                    cwindow_update              (err_t* err);

    /*  Gets the size, as an integer, of the window ascociated with the provided `handle` and puts it in `out`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to get the size of.
        @param out A pointer to the location you want the size to be copied.
        @returns N/A
    */
    void                    cwindow_get_size            (err_t* err, cwindow_t handle, ivec2* out);

    /*  Gets the size, as a float, of the window ascociated with the provided `handle` and puts it in `out`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to get the size of.
        @param out A pointer to the location you want the size to be copied.
        @returns N/A
    */
    void                    cwindow_get_sizef           (err_t* err, cwindow_t handle, vec2* out);

    /*  Gets the current title of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to get the title of.
        @returns The title as a `const char*`.
    */
    const char*             cwindow_get_title           (err_t* err, cwindow_t handle);

    /*  Gets whether the window ascociated with the provided `handle` has the fullscreen flag enabled.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to get the fullscreen flag of.
        @returns Whether the fullscreen flag is enabled.
    */
    bool                    cwindow_get_fullscreen      (err_t* err, cwindow_t handle);

    /*  Gets whether the window ascociated with the provided `handle` has the minimized flag enabled.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to get the minimized flag of.
        @returns Whether the minimized flag is enabled.
    */
    bool                    cwindow_get_minimized       (err_t* err, cwindow_t handle);

    /*  Gets whether the window ascociated with the provided `handle` has the resized flag enabled.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to get the resized flag of.
        @returns Whether the resized flag is enabled.
    */
    bool                    cwindow_get_resized         (err_t* err, cwindow_t handle);

    /*  Gets whether the window ascociated with the provided `handle` has the closed flag enabled.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to get the closed flag of.
        @returns Whether the closed flag is enabled.
    */
    bool                    cwindow_get_closed          (err_t* err, cwindow_t handle);

    /*  Gets whether the window ascociated with the provided `handle` has the focused flag enabled.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to get the focused flag of.
        @returns Whether the focused flag is enabled.
    */
    bool                    cwindow_get_focused         (err_t* err, cwindow_t handle);

    /*  Sets the size of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to set the size of.
        @param value The size you want to resize the window to.
        @returns N/A.
        @note Sets the `CORE_CWINDOW_RESIZED` flag.
    */
    void                    cwindow_set_size            (err_t* err, cwindow_t handle, ivec2 value);

    /*  Sets the title of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to set the title of.
        @param value The title you want to rename the window to.
        @returns N/A.
    */
    void                    cwindow_set_title           (err_t* err, cwindow_t handle, const char* value);

    /*  Sets the icon of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to set the icon of.
        @param data The raw pixel data of the icon.
        @param data_size The size of the provided `data` buffer.
        @returns N/A.
    */
    void                    cwindow_set_icon            (err_t* err, cwindow_t handle, uint8_t* data, uint64_t data_size);

    /*  Sets the fullscreen flag of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to set the fullscreen flag of.
        @param value Whether the flag should be enabled or disabled.
        @returns N/A.
        @note Sets the `CORE_CWINDOW_FULLSCREEN` flag.
    */
    void                    cwindow_set_fullscreen      (err_t* err, cwindow_t handle, bool value);

    /*  Sets the minimized flag of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to set the minimized flag of.
        @param value Whether the flag should be enabled or disabled.
        @returns N/A.
        @note Sets the `CORE_CWINDOW_MINIMIZED` flag.
    */
    void                    cwindow_set_minimized       (err_t* err, cwindow_t handle, bool value);

    /*  Sets the resized flag of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to set the resized flag of.
        @param value Whether the flag should be enabled or disabled.
        @returns N/A.
        @note Sets the `CORE_CWINDOW_RESIZED` flag.
    */
    void                    cwindow_set_resized         (err_t* err, cwindow_t handle, bool value);

    /*  Sets the closed flag of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to set the closed flag of.
        @param value Whether the flag should be enabled or disabled.
        @returns N/A
        @note Sets the `CORE_CWINDOW_CLOSED` flag.
    */
    void                    cwindow_close               (err_t* err, cwindow_t handle);

#pragma endregion // CORE_WINDOW

#pragma region CORE_INPUT

    typedef enum cinput_kc_t cinput_kc_t;
    enum cinput_kc_t {

        CORE_KEYCODE_NONE                               = -1,
        CORE_KEYKEY_RESERVED,

        /* KEYBOARD */

        CORE_KEYCODE_ESC,                   // KEYCODE 'ESC'
        CORE_KEYCODE_1,                     // KEYCODE '1'
        CORE_KEYCODE_2,                     // KEYCODE '2'
        CORE_KEYCODE_3,                     // KEYCODE '3'
        CORE_KEYCODE_4,                     // KEYCODE '4'
        CORE_KEYCODE_5,                     // KEYCODE '5'
        CORE_KEYCODE_6,                     // KEYCODE '6'
        CORE_KEYCODE_7,                     // KEYCODE '7'
        CORE_KEYCODE_8,                     // KEYCODE '8'
        CORE_KEYCODE_9,                     // KEYCODE '9'
        CORE_KEYCODE_0,                     // KEYCODE '0'
        CORE_KEYCODE_MINUS,                 // KEYCODE 'MINUS'
        CORE_KEYCODE_EQUAL,                 // KEYCODE 'EQUAL'
        CORE_KEYCODE_BACKSPACE,             // KEYCODE 'BACKSPACE'
        CORE_KEYCODE_TAB,                   // KEYCODE 'TAB'
        CORE_KEYCODE_Q,                     // KEYCODE 'Q'
        CORE_KEYCODE_W,                     // KEYCODE 'W'
        CORE_KEYCODE_E,                     // KEYCODE 'E'
        CORE_KEYCODE_R,                     // KEYCODE 'R'
        CORE_KEYCODE_T,                     // KEYCODE 'T'
        CORE_KEYCODE_Y,                     // KEYCODE 'Y'
        CORE_KEYCODE_U,                     // KEYCODE 'U'
        CORE_KEYCODE_I,                     // KEYCODE 'I'
        CORE_KEYCODE_O,                     // KEYCODE 'O'
        CORE_KEYCODE_P,                     // KEYCODE 'P'
        CORE_KEYCODE_LEFTBRACE,             // KEYCODE 'LEFTBRACE'
        CORE_KEYCODE_RIGHTBRACE,            // KEYCODE 'RIGHTBRACE'
        CORE_KEYCODE_ENTER,                 // KEYCODE 'ENTER'
        CORE_KEYCODE_LEFTCTRL,              // KEYCODE 'LEFTCTRL'
        CORE_KEYCODE_A,                     // KEYCODE 'A'
        CORE_KEYCODE_S,                     // KEYCODE 'S'
        CORE_KEYCODE_D,                     // KEYCODE 'D'
        CORE_KEYCODE_F,                     // KEYCODE 'F'
        CORE_KEYCODE_G,                     // KEYCODE 'G'
        CORE_KEYCODE_H,                     // KEYCODE 'H'
        CORE_KEYCODE_J,                     // KEYCODE 'J'
        CORE_KEYCODE_K,                     // KEYCODE 'K'
        CORE_KEYCODE_L,                     // KEYCODE 'L'
        CORE_KEYCODE_SEMICOLON,             // KEYCODE 'SEMICOLON'
        CORE_KEYCODE_APOSTROPHE,            // KEYCODE 'APOSTROPHE'
        CORE_KEYCODE_GRAVE,                 // KEYCODE 'GRAVE'
        CORE_KEYCODE_LEFTSHIFT,             // KEYCODE 'LEFTSHIFT'
        CORE_KEYCODE_BACKSLASH,             // KEYCODE 'BACKSLASH'
        CORE_KEYCODE_Z,                     // KEYCODE 'Z'
        CORE_KEYCODE_X,                     // KEYCODE 'X'
        CORE_KEYCODE_C,                     // KEYCODE 'C'
        CORE_KEYCODE_V,                     // KEYCODE 'V'
        CORE_KEYCODE_B,                     // KEYCODE 'B'
        CORE_KEYCODE_N,                     // KEYCODE 'N'
        CORE_KEYCODE_M,                     // KEYCODE 'M'
        CORE_KEYCODE_COMMA,                 // KEYCODE 'COMMA'
        CORE_KEYCODE_DOT,                   // KEYCODE 'DOT'
        CORE_KEYCODE_SLASH,                 // KEYCODE 'SLASH'
        CORE_KEYCODE_RIGHTSHIFT,            // KEYCODE 'RIGHTSHIFT'
        CORE_KEYCODE_KPASTERISK,            // KEYCODE 'KPASTERISK'
        CORE_KEYCODE_LEFTALT,               // KEYCODE 'LEFTALT'
        CORE_KEYCODE_SPACE,                 // KEYCODE 'SPACE'
        CORE_KEYCODE_CAPSLOCK,              // KEYCODE 'CAPSLOCK'
        CORE_KEYCODE_F1,                    // KEYCODE 'F1'
        CORE_KEYCODE_F2,                    // KEYCODE 'F2'
        CORE_KEYCODE_F3,                    // KEYCODE 'F3'
        CORE_KEYCODE_F4,                    // KEYCODE 'F4'
        CORE_KEYCODE_F5,                    // KEYCODE 'F5'
        CORE_KEYCODE_F6,                    // KEYCODE 'F6'
        CORE_KEYCODE_F7,                    // KEYCODE 'F7'
        CORE_KEYCODE_F8,                    // KEYCODE 'F8'
        CORE_KEYCODE_F9,                    // KEYCODE 'F9'
        CORE_KEYCODE_F10,                   // KEYCODE 'F10'
        CORE_KEYCODE_NUMLOCK,               // KEYCODE 'NUMLOCK'
        CORE_KEYCODE_SCROLLLOCK,            // KEYCODE 'SCROLLLOCK'

        /* KEYPAD */

        CORE_KEYCODE_KP7,                   // KEYCODE `KP7`
        CORE_KEYCODE_KP8,                   // KEYCODE `KP8`
        CORE_KEYCODE_KP9,                   // KEYCODE `KP9`
        CORE_KEYCODE_KPMINUS,               // KEYCODE `KPMINUS`
        CORE_KEYCODE_KP4,                   // KEYCODE `KP4`
        CORE_KEYCODE_KP5,                   // KEYCODE `KP5`
        CORE_KEYCODE_KP6,                   // KEYCODE `KP6`
        CORE_KEYCODE_KPPLUS,                // KEYCODE `KPPLUS`
        CORE_KEYCODE_KP1,                   // KEYCODE `KP1`
        CORE_KEYCODE_KP2,                   // KEYCODE `KP2`
        CORE_KEYCODE_KP3,                   // KEYCODE `KP3`
        CORE_KEYCODE_KP0,                   // KEYCODE `KP0`
        CORE_KEYCODE_KPDOT,                 // KEYCODE `KPDOT`

        /* RANDOM SHIT AND ARROW KEYS IDK */
            
        CORE_KEYCODE_ZENKAKUHANKAKU,        // KEYCODE `ZENKAKUHANKAKU`
        CORE_KEYCODE_102ND,                 // KEYCODE `102ND`
        CORE_KEYCODE_F11,                   // KEYCODE `F11`
        CORE_KEYCODE_F12,                   // KEYCODE `F12`
        CORE_KEYCODE_RO,                    // KEYCODE `RO`
        CORE_KEYCODE_KATAKANA,              // KEYCODE `KATAKANA`
        CORE_KEYCODE_HIRAGANA,              // KEYCODE `HIRAGANA`
        CORE_KEYCODE_HENKAN,                // KEYCODE `HENKAN`
        CORE_KEYCODE_KATAKANAHIRAGANA,      // KEYCODE `KATAKANAHIRAGANA`
        CORE_KEYCODE_MUHENKAN,              // KEYCODE `MUHENKAN`
        CORE_KEYCODE_KPJPCOMMA,             // KEYCODE `KPJPCOMMA`
        CORE_KEYCODE_KPENTER,               // KEYCODE `KPENTER`
        CORE_KEYCODE_RIGHTCTRL,             // KEYCODE `RIGHTCTRL`
        CORE_KEYCODE_KPSLASH,               // KEYCODE `KPSLASH`
        CORE_KEYCODE_SYSRQ,                 // KEYCODE `SYSRQ`
        CORE_KEYCODE_RIGHTALT,              // KEYCODE `RIGHTALT`
        CORE_KEYCODE_LINEFEED,              // KEYCODE `LINEFEED`
        CORE_KEYCODE_HOME,                  // KEYCODE `HOME`
        CORE_KEYCODE_UP,                    // KEYCODE `UP`
        CORE_KEYCODE_PAGEUP,                // KEYCODE `PAGEUP`
        CORE_KEYCODE_LEFT,                  // KEYCODE `LEFT`
        CORE_KEYCODE_RIGHT,                 // KEYCODE `RIGHT`
        CORE_KEYCODE_END,                   // KEYCODE `END`
        CORE_KEYCODE_DOWN,                  // KEYCODE `DOWN`
        CORE_KEYCODE_PAGEDOWN,              // KEYCODE `PAGEDOWN`
        CORE_KEYCODE_INSERT,                // KEYCODE `INSERT`
        CORE_KEYCODE_DELETE,                // KEYCODE `DELETE`
        CORE_KEYCODE_MACRO,                 // KEYCODE `MACRO`
        CORE_KEYCODE_MUTE,                  // KEYCODE `MUTE`
        CORE_KEYCODE_VOLUMEDOWN,            // KEYCODE `VOLUMEDOWN`
        CORE_KEYCODE_VOLUMEUP,              // KEYCODE `VOLUMEUP`
        CORE_KEYCODE_POWER,                 // KEYCODE `POWER`
        CORE_KEYCODE_KPEQUAL,               // KEYCODE `KPEQUAL`
        CORE_KEYCODE_KPPLUSMINUS,           // KEYCODE `KPPLUSMINUS`
        CORE_KEYCODE_PAUSE,                 // KEYCODE `PAUSE`
        CORE_KEYCODE_SCALE,                 // KEYCODE `SCALE`

        CORE_KEYCODE_COUNT,                 // Number of Defined Keycodes

        CORE_MOUSEB_LEFT             = 272, // MOUSE BUTTON `LEFT`
        CORE_MOUSEB_RIGHT,                  // MOUSE BUTTON `RIGHT`
        CORE_MOUSEB_MIDDLE,                 // MOUSE BUTTON `MIDDLE`
        
    };

    /*  Gets the mouse position relative to the current focused window.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param out A pointer to the location you want the position to be copied.
        @returns N/A.
    */
    void                    cinput_get_mouse_position   (err_t* err, ivec2* out);

    /*  Gets the whether the provided keycode is pressed relative to the current focused window.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param keycode The keycode you want to check for.
        @returns Whether button ascociated with that keycode is pressed or not.
    */
    bool                    cinput_get_button           (err_t* err, cinput_kc_t key_code);

#pragma endregion // CORE_INPUT

#pragma region CORE_GRAPHICS

    /*  Initializes OpenGL and hooks it up to the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to hook OpenGL up to.
        @returns The same handle you provided it.
    */
    cwindow_t               cgraf_opengl_hook           (err_t* err, cwindow_t handle);

    /*  Deinitializes OpenGL objects related to the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to unhook OpenGL from.
        @returns N/A.
        @note Do not call this unless you are switching APIs at runtime, it will automatically get called by the
        window destructor.
    */
    void                    cgraf_opengl_cleanup        (err_t* err, cwindow_t handle);

    /*  Initializes Vulkan and hooks it up to the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to hook Vulkan up to.
        @returns The same handle you provided it.
    */
    cwindow_t               cgraf_vulkan_hook           (err_t* err, cwindow_t handle);

    /*  Deinitializes Vulkan objects related to the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to unhook Vulkan from.
        @returns N/A.
        @note Do not call this unless you are switching APIs at runtime, it will automatically get called by the
        window destructor.
    */
    void                    cgraf_vulkan_cleanup        (err_t* err, cwindow_t handle);

    /*  Switches draw context to the to the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want to draw to.
        @returns N/A
        @note Automatically determines the correct API to call.
    */
    void                    cgraf_draw_start            (err_t* err, cwindow_t handle);

    /*  Swaps buffers connected to the surface of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window you want end drawing to.
        @returns N/A
        @note Automatically determines the correct API to call.
    */
    void                    cgraf_draw_end              (err_t* err, cwindow_t handle);

    /*  Handles resizing of the window ascociated with the provided `handle`.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @param handle The window that needs to handle resizing.
        @returns N/A
        @note Do not call this, it will automatically get called by `cwindwow_update`.
        @note Automatically determines the correct API to call.
    */
    void                    cgraf_handle_resize         (err_t* err, cwindow_t handle);

    /*  Deinitializes and frees all data related to graphics APIs.
        @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
        @returns N/A
        @note Do not call this, it will automatically get called by `cwindwow_close` on last window close.
    */
    void                    cgraf_deinitialize          (err_t* err);

#pragma endregion // CORE_GRAPHICS

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_H