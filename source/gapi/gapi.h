#if !defined(CORE_GAPI_H)
#define CORE_GAPI_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <window/window.h>

/*
    All GAPI is going to do, is create a context with the desired window
    so you can just call the api your self, maybe I will add a render
    header that will abstract the api, but for now this will just
    initialize and hook the desired api to some window.
*/

cwindow_t               cgapi_opengl_hook           (err_t* err, cwindow_t handle);
void                    cgapi_opengl_cleanup        (err_t* err, cwindow_t handle);

cwindow_t               cgapi_vulkan_hook           (err_t* err, cwindow_t handle);
void                    cgapi_vulkan_cleanup        (err_t* err, cwindow_t handle);

void                    cgapi_gl_draw_start         (err_t* err, cwindow_t handle);
void                    cgapi_gl_draw_end           (err_t* err, cwindow_t handle);

void                    cgapi_handle_resize         (err_t* err, cwindow_t handle);
void                    cgapi_deinitialize          (err_t* err);

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_GAPI_H