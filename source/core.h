#if !defined(CORE_H)
#define CORE_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <platform/platform.h>
#include <window/window.h>
#include <debug/debug.h>
#include <gapi/gapi.h>
#include <math/math.h>

int32_t                 main                        (int32_t argc, const char** argv);

void                    startup                     (err_t* err, int32_t argc, const char** argv);
void                    update                      (err_t* err);
void                    shutdown                    (err_t* err);

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_H