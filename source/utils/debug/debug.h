#if !defined(CORE_DEBUG_H)
#define CORE_DEBUG_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <platform/platform.h>

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

    CORE_SUCCESS        = 0,    // Success
    CORE_EXIT,                  // Exit Successfully
    CORE_PINVALID,              // Invalid Parameters
    CORE_WINMAXC,               // Max Window Count
    CORE_CREASURF,              // Failed to Create Surface
    CORE_XDGGETSURF,            // XDG Failed to Get Surface
    CORE_XDGSADDLIS,            // XDG Failed to Add Listener to Surface
    CORE_XDGGETTL,              // XDG Failed to Get Top-Level
    CORE_XDGTLADDLIS,           // XDG Failed to Add Listener to Top-Level
    CORE_DISPRT,                // Failed to do a Display Round Trip
    CORE_WINNREADY,             // Window Not Read / Initialized
    CORE_DISPCONREF,            // Display Connection Refused
    CORE_DIPREG,                // Display Registry Not Found
    CORE_REGADDLIS,             // Display Registry Failed to Add Listener
    CORE_DISPDISPAT,            // Display Dispatch Failure
    CORE_DISPFLUSH,             // Display Flush Failure
    CORE_EGLNODISP,             // No EGL Display Found
    CORE_EGLINIT,               // EGL Failed to Initialize
    CORE_EGLBIND,               // EGL Failed to Bind GAPI
    CORE_EGLCONFIG,             // EGL Failed to Load Config
    CORE_EGLCONTEXT,            // EGL Failed to Create Context
    CORE_EGLMAKCUR,             // EGL Failed to Make Context Current
    CORE_GLADLOAD,              // GLAD Failed to Load
    CORE_EGLCREATWIN,           // EGL Failed to Create Window
    CORE_EGLCREATSURF,          // EGL Failed to Create Surface

    CORE_ERROR_COUNT            // Number of Error Types

};

const char*             _cdbg_get_errmsg            (err_t code);

#define throw(err, msg)             do{if((err)!=NULL){*(err)=(msg);cdbg_err(_cdbg_get_errmsg(*err));}}while(0)
#define catch(err, call, except)    do{call;if(err!=NULL&&*err!=CORE_SUCCESS)except}while(0)

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_DEBUG_H