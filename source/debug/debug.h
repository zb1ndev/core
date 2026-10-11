#if !defined(CORE_DEBUG_H)
#define CORE_DEBUG_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <platform/platform.h>

typedef int8_t err_t;

void                    _cdbg_eprint                (int64_t line, 
                                                    const char* function, 
                                                    const char* etype, 
                                                    const char* fmt, ...);

#define cdbg_log(msg)       _cdbg_eprint(__LINE__, __func__, "LOG", msg)
#define cdbg_logf(fmt, ...) _cdbg_eprint(__LINE__, __func__, "LOG", fmt, __VA_ARGS__)

#define cdbg_warn(msg)       _cdbg_eprint(__LINE__, __func__, "WARNING", msg)
#define cdbg_warnf(fmt, ...) _cdbg_eprint(__LINE__, __func__, "WARNING", fmt, __VA_ARGS__)

#define cdbg_err(msg)       _cdbg_eprint(__LINE__, __func__, "ERROR", msg)
#define cdbg_errf(fmt, ...) _cdbg_eprint(__LINE__, __func__, "ERROR", fmt, __VA_ARGS__)

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_DEBUG_H