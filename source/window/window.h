#if !defined(CORE_WINDOW_H)
#define CORE_WINDOW_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <platform/platform.h>
#include <debug/debug.h>
#include <math/math.h>

#define CORE_CWINDOW_MAX                            UINT8_MAX
#define CORE_CWINDOW_INVALID                        UINT8_MAX

typedef uint8_t cwindow_t;

#define CORE_CWINDOW_CLOSED                         (1<<0)
#define CORE_CWINDOW_FULLSCREEN                     (1<<1)
#define CORE_CWINDOW_MINIMIZED                      (1<<2)
#define CORE_CWINDOW_RESIZED                        (1<<3)
#define CORE_CWINDOW_READY                          (1<<4)

cwindow_t               cwindow_create              (err_t* err, const char* title, cvec2u_t size);
void                    cwindow_destroy             (err_t* err, cwindow_t handle);

void                    cwindow_update              (err_t* err);

void                    cwindow_get_size            (err_t* err, cwindow_t handle, cvec2u_t* out);
void                    cwindow_get_sizef           (err_t* err, cwindow_t handle, cvec2f_t* out);
const char*             cwindow_get_title           (err_t* err, cwindow_t handle);

bool                    cwindow_get_fullscreen      (err_t* err, cwindow_t handle);
bool                    cwindow_get_minimized       (err_t* err, cwindow_t handle);
bool                    cwindow_get_resized         (err_t* err, cwindow_t handle);

void                    cwindow_set_size            (err_t* err, cwindow_t handle, cvec2u_t value);
void                    cwindow_set_title           (err_t* err, cwindow_t handle, const char* value);
void                    cwindow_set_icon            (err_t* err, cwindow_t handle, uint8_t* data, uint64_t data_size);

void                    cwindow_set_fullscreen      (err_t* err, cwindow_t handle, bool value);
void                    cwindow_set_minimized       (err_t* err, cwindow_t handle, bool value);
void                    cwindow_set_resized         (err_t* err, cwindow_t handle, bool value);

void                    cwindow_close               (err_t* err, cwindow_t handle);

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_WINDOW_H