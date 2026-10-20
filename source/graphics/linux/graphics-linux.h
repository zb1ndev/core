#if !defined(CORE_GRAPHICS_LINUX_H)
#define CORE_GRAPHICS_LINUX_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// @note    The following code should only be accessible by internal translation units
//          and therefor this header should only be included by implmentation files.

#include <core.h>

/*  OpenGL Window object holding OpenGL specific data.
    @param egl_surface The surface asociated with the window.
    @param egl_window The wayland egl window instance.
    @param initialized Whether OpenGL has been initialized for this window.
*/
typedef struct cwindow_gl_t cwindow_gl_t;
struct cwindow_gl_t {

    EGLSurface              egl_surface;
    struct wl_egl_window*   egl_window;
    bool                    initialized;

};

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_GRAPHICS_LINUX_H