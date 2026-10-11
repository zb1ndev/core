#if !defined(CORE_GAPI_H)
#define CORE_GAPI_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <platform/platform.h>
#include <window/window.h>
#include <debug/debug.h>
#include <math/math.h>

/*
    All GAPI is going to do, is create a context with the desired window
    so you can just call the api your self, maybe I will add a render
    header that will abstract the api, but for now this will just
    initialize and hook the desired api to some window.
*/

void                    cgapi_opengl_hook           (err_t* err, cwindow_t handle);
void                    cgapi_vulkan_hook           (err_t* err, cwindow_t handle);

void                    cgapi_opengl_cleanup        (err_t* err);
void                    cgapi_vulkan_cleanup        (err_t* err);

// @todo add getters and setters for opengl components

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_GAPI_H