#include <gapi/linux/gl_linux_state.c>

cwindow_t cgapi_opengl_hook(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return CORE_CWINDOW_INVALID;
    );

    if (gl_initialized == false) catch(err, cgapi_opengl_initialize(err), 
        return CORE_CWINDOW_INVALID;
    ); 
    
    window->gapi_cleanup = cgapi_opengl_cleanup;
    gl_windows[handle].egl_window = wl_egl_window_create(window->surface, window->size[X], window->size[Y]);
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

void cgapi_opengl_cleanup(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return;
    );

    if (gl_initialized == false) return;

    eglDestroySurface(egl_display, gl_windows[handle].egl_surface);
    wl_egl_window_destroy(gl_windows[handle].egl_window);
    gl_windows[handle].initialized = false;
    gl_window_count--;

}

cwindow_t cgapi_vulkan_hook(err_t* err, cwindow_t handle) {
    
    cdbg_err("vulkan is not implemented");
    return CORE_CWINDOW_INVALID;

}

void cgapi_vulkan_cleanup(err_t* err, cwindow_t handle) {
    
    cdbg_err("vulkan is not implemented");
    return;

}

void cgapi_gl_draw_start(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return;
    );

    wl_display_flush(cwm_get_display());
    eglMakeCurrent(egl_display, gl_windows[handle].egl_surface, gl_windows[handle].egl_surface, egl_shared_context);


}

void cgapi_gl_draw_end(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return;
    );
    
    eglSwapBuffers(egl_display, gl_windows[handle].egl_surface);

}

// @note api agnostic
void cgapi_handle_resize(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return;
    );

    // @note handles the opengl case
    if (gl_windows[handle].initialized == true) {

        cwindow_linux_t* window = NULL;
        catch(err, window = cwindow_get_data(err, handle), 
            return;
        );

        wl_egl_window_resize(gl_windows[handle].egl_window, window->size[X], window->size[Y], 0, 0);

    }
   
}

// @note api agnostic
void cgapi_deinitialize(err_t* err) {

    if (gl_initialized == true) {
        eglDestroyContext(egl_display, egl_shared_context);
        eglTerminate(egl_display);
        gl_initialized = false;
    }

}
