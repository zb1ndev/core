#include <gapi/gapi.h>
#include <window/linux/window_linux.h>

typedef struct cwindow_gl_t cwindow_gl_t;
struct cwindow_gl_t {

    EGLSurface          egl_surface;
    struct wl_egl_window* egl_window;

};

internal cwindow_gl_t gl_windows[CORE_CWINDOW_MAX] = {0};
internal EGLDisplay egl_display;
internal EGLConfig egl_config;
internal EGLContext egl_shared_context;
internal bool gl_initialized = false;

internal void cgapi_opengl_initialize(err_t* err) {

    

    gl_initialized = true;

}

void cgapi_opengl_hook(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = cwindow_get_data(err, handle);

}

void cgapi_opengl_cleanup(err_t* err) {

    if (!gl_initialized) return;



}
