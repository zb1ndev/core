#if !defined(CORE_DEBUG_H)
#define CORE_DEBUG_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <platform/platform.h>

typedef enum err_t err_t;
enum err_t {

    CORE_SUCCESS        = 0,    // Success
    CORE_PINVALID,              // Invalid Parameters
    CORE_WINMAXC,               // Max Window Count
    CORE_DISPCONREF,            // Display Connection Refused

    CORE_ERROR_COUNT            // Number of Error Types

};

#define throw(err, msg) do{if((err)!=NULL)*(err)=(msg);}while(0)
#define catch(err, call, except) do{call;if(err!=NULL&&*err!=CORE_SUCCESS)except}while(0)

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

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_DEBUG_H