#include <graphics/linux/graphics-linux.h> // @note input-manager.h and window-linux.h includes core.h

internal cwindow_gl_t   gl_windows[CORE_CWINDOW_MAX]    = {0};
internal uint8_t        gl_window_count                 = 0;
internal bool           gl_initialized                  = false;

internal EGLConfig      egl_config;
internal EGLDisplay     egl_display;
internal EGLContext     egl_shared_context;

#include <window/linux/window-linux.h>

cwindow_t cgraf_opengl_hook(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get(err, handle), 
        return CORE_CWINDOW_INVALID;
    );

    if (gl_initialized == false) {

        egl_display = eglGetDisplay((EGLNativeDisplayType)cwindow_get_display());
        if (egl_display == EGL_NO_DISPLAY) {
            throw(err, CORE_EGLNODISP);
            return CORE_CWINDOW_INVALID;
        }

        if (eglInitialize(egl_display, NULL, NULL) == EGL_FALSE) {
            throw(err, CORE_EGLINIT);
            return CORE_CWINDOW_INVALID;
        }

        if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE) {
            throw(err, CORE_EGLBIND);
            return CORE_CWINDOW_INVALID;
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
            return CORE_CWINDOW_INVALID;
        }

        local_persist const EGLint context_attributes[] = {

            EGL_CONTEXT_MAJOR_VERSION, 4,
            EGL_CONTEXT_MINOR_VERSION, 6,
            EGL_NONE
        
        };

        egl_shared_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attributes);
        if (egl_shared_context == EGL_NO_CONTEXT) {
            throw(err, CORE_EGLCONTEXT);
            return CORE_CWINDOW_INVALID;
        }

        if (eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, egl_shared_context) == EGL_FALSE) {
            throw(err, CORE_EGLMAKCUR);
            return CORE_CWINDOW_INVALID;
        }

        if (gladLoadGLLoader((GLADloadproc)eglGetProcAddress) == EGL_FALSE) {
            throw(err, CORE_GLADLOAD);
            return CORE_CWINDOW_INVALID;
        }

        gl_initialized = true;

    }

    window->cgraf_cleanup = cgraf_opengl_cleanup;
    gl_windows[handle].egl_window = wl_egl_window_create(window->surface, window->size[0], window->size[1]);
    if (gl_windows[handle].egl_window == NULL) {
        throw(err, CORE_EGLCREATWIN);
        return CORE_CWINDOW_INVALID;
    }

    gl_windows[handle].egl_surface = eglCreateWindowSurface(egl_display, egl_config, (EGLNativeWindowType)gl_windows[handle].egl_window, NULL);
    if (gl_windows[handle].egl_surface == EGL_NO_SURFACE) {
        wl_egl_window_destroy(gl_windows[handle].egl_window);
        gl_windows[handle].egl_window = NULL;
        throw(err, CORE_EGLCREATSURF);
        return CORE_CWINDOW_INVALID;
    }

    gl_windows[handle].initialized = true;
    gl_window_count++;
    return handle;

}

void cgraf_opengl_cleanup(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get(err, handle), 
        return;
    );

    if (gl_initialized == false) 
        return;

    eglDestroySurface(egl_display, gl_windows[handle].egl_surface);
    wl_egl_window_destroy(gl_windows[handle].egl_window);
    gl_windows[handle].initialized = false;
    gl_window_count--;

}

cwindow_t cgraf_vulkan_hook(err_t* err, cwindow_t handle) {

    (void)err;
    (void)handle;
    cdbg_warn("not implemented");

    return CORE_CWINDOW_INVALID;

}

void cgraf_vulkan_cleanup(err_t* err, cwindow_t handle) {

    (void)err;
    (void)handle;
    cdbg_warn("not implemented");

}

void cgraf_draw_start(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get(err, handle), 
        return;
    );

    // @note handles the opengl case
    if (gl_windows[handle].initialized == true) {

        wl_display_flush(cwindow_get_display());
        eglMakeCurrent(egl_display, gl_windows[handle].egl_surface, gl_windows[handle].egl_surface, egl_shared_context);

    }


}

void cgraf_draw_end(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get(err, handle), 
        return;
    );

    // @note handles the opengl case
    if (gl_windows[handle].initialized == true) {

        eglSwapBuffers(egl_display, gl_windows[handle].egl_surface);

    }

}

void cgraf_handle_resize(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get(err, handle), 
        return;
    );

    // @note handles the opengl case
    if (gl_windows[handle].initialized == true) {

        cwindow_linux_t* window = NULL;
        catch(err, window = cwindow_get(err, handle), 
            return;
        );

        wl_egl_window_resize(gl_windows[handle].egl_window, window->size[0], window->size[1], 0, 0);

    }

}

void cgraf_deinitialize(err_t* err) {

    if (gl_initialized == true) {

        eglDestroyContext(egl_display, egl_shared_context);
        eglTerminate(egl_display);
        gl_initialized = false;
    
    }

}