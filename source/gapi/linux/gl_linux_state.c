#include <gapi/gapi.h>
#include <window/linux/window_linux.h>

typedef struct cwindow_gl_t cwindow_gl_t;
struct cwindow_gl_t {

    EGLSurface              egl_surface;
    struct wl_egl_window*   egl_window;
    bool                    initialized;

};

internal cwindow_gl_t   gl_windows[CORE_CWINDOW_MAX]    = {0};
internal uint8_t        gl_window_count                 = 0;
internal bool           gl_initialized                  = false;

internal EGLConfig      egl_config;
internal EGLDisplay     egl_display;
internal EGLContext     egl_shared_context;

internal void cgapi_opengl_initialize(err_t* err) {

    egl_display = eglGetDisplay((EGLNativeDisplayType)cwm_get_display());
    if (egl_display == EGL_NO_DISPLAY) {
        throw(err, CORE_EGLNODISP);
        return;
    }

    if (eglInitialize(egl_display, NULL, NULL) == EGL_FALSE) {
        throw(err, CORE_EGLINIT);
        return;
    }

    if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE) {
        throw(err, CORE_EGLBIND);
        return;
    }
     
    const EGLint attributes[] = {
        
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_ALPHA_SIZE,      8,
        EGL_DEPTH_SIZE,      24,
        EGL_NONE

    };

    EGLint config_count = 0;
    if (eglChooseConfig(egl_display, attributes, &egl_config, 1, &config_count) == EGL_FALSE
        || config_count == 0
    ) {
        throw(err, CORE_EGLCONFIG);
        return;
    }

    local_persist const EGLint context_attributes[] = {

        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 3,
        EGL_NONE
    
    };

    egl_shared_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attributes);
    if (egl_shared_context == EGL_NO_CONTEXT) {
        throw(err, CORE_EGLCONTEXT);
        return;
    }

    gl_initialized = true;

}
