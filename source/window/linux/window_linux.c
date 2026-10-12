#include <window/linux/window_linux_state.c>

cwindow_t cwindow_create(err_t* err, const char* title, cvec2u_t size) {

    if (window_manager_initialized == false)
        catch(err, cwindow_manager_initialize(err), 
            return CORE_CWINDOW_INVALID;
        );

    if (window_count >= CORE_CWINDOW_MAX) {
        throw(err, CORE_WINMAXC);
        return CORE_CWINDOW_INVALID;
    }

    cwindow_t result = free_windows[CORE_CWINDOW_MAX - ++window_count];
    cwindow_linux_t* window = &windows[result];

    *window = (cwindow_linux_t) {
        .title = title,
        .size = {size[X], size[Y]},
        .sizef = {(float)size[X], (float)size[Y]}
    };
 
    window->surface = wl_compositor_create_surface(compositor);
 
    window->xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, window->surface);
    xdg_surface_add_listener(window->xdg_surface, &xdg_surface_listener, window);
 
    window->toplevel = xdg_surface_get_toplevel(window->xdg_surface);
    xdg_toplevel_add_listener(window->toplevel, &toplevel_listener, window);
    xdg_toplevel_set_title(window->toplevel, title);
 
    wl_surface_commit(window->surface);
    wl_display_roundtrip(display);

    return result;

}

void cwindow_destroy(err_t* err, cwindow_t handle) {

    xdg_toplevel_destroy(windows[handle].toplevel);
    xdg_surface_destroy(windows[handle].xdg_surface);
    wl_surface_destroy(windows[handle].surface);

    free_windows[CORE_CWINDOW_MAX - window_count] = handle;
    
    windows[handle] = (cwindow_linux_t) {
        .title = NULL,
        .size = {0,0},
        .sizef = {0.0f,0.0f},
        .state = 0UL,
        .surface = NULL,
        .toplevel = NULL,
        .xdg_surface = NULL
    };
 
    if (--window_count <= 0)
        catch(err, cwindow_manager_deinitialize(err), 
            return;
        );
    
}

void cwindow_update(err_t* err) {

    // Check all window state flags and handle them

    return;

}

void cwindow_get_size(err_t* err, cwindow_t handle, cvec2u_t* out) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return;
    }

    (*out)[X] = windows[handle].size[X];
    (*out)[Y] = windows[handle].size[Y];

}

void cwindow_get_sizef(err_t* err, cwindow_t handle, cvec2f_t* out) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return;
    }

    (*out)[X] = windows[handle].sizef[X];
    (*out)[Y] = windows[handle].sizef[Y];

}

const char* cwindow_get_title(err_t* err, cwindow_t handle) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return false;
    }

    return windows[handle].title;

}

bool cwindow_get_fullscreen(err_t* err, cwindow_t handle) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return false;
    }

    return windows[handle].state & CORE_CWINDOW_FULLSCREEN == 1;

}

bool cwindow_get_minimized(err_t* err, cwindow_t handle) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return false;
    }

    return windows[handle].state & CORE_CWINDOW_MINIMIZED == 1;

}

bool cwindow_get_resized(err_t* err, cwindow_t handle) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return false;
    }

    return windows[handle].state & CORE_CWINDOW_RESIZED == 1;

}

void cwindow_set_size(err_t* err, cwindow_t handle, cvec2u_t value) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows[handle].size[X] = value[X];
    windows[handle].sizef[X] = (float)value[X];
    windows[handle].size[Y] = value[Y];
    windows[handle].sizef[Y] = (float)value[Y];

    cwindow_set_resized(err, handle, true);

}

void cwindow_set_title(err_t* err, cwindow_t handle, const char* value) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows[handle].title = value;
    xdg_toplevel_set_title(windows[handle].toplevel, value);

}

void cwindow_set_icon(err_t* err, cwindow_t handle, uint8_t* data, uint64_t data_size) {

    (void)err;
    (void)handle;
    (void)data;
    (void)data_size;

    cdbg_warn("not implemented");

}

void cwindow_set_fullscreen(err_t* err, cwindow_t handle, bool value) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows[handle].state = value 
        ? windows[handle].state | CORE_CWINDOW_FULLSCREEN
        : windows[handle].state & ~CORE_CWINDOW_FULLSCREEN;
    cwindow_set_resized(err, handle, true);

}

void cwindow_set_minimized(err_t* err, cwindow_t handle, bool value) {
    
    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows[handle].state = value 
        ? windows[handle].state | CORE_CWINDOW_MINIMIZED
        : windows[handle].state & ~CORE_CWINDOW_MINIMIZED;
    cwindow_set_resized(err, handle, true);
}

void cwindow_set_resized(err_t* err, cwindow_t handle, bool value) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows[handle].state = value 
        ? windows[handle].state | CORE_CWINDOW_RESIZED
        : windows[handle].state & ~CORE_CWINDOW_RESIZED;

}

void cwindow_close(err_t* err, cwindow_t handle) {

    windows[handle].state |= CORE_CWINDOW_CLOSED;

}

cwindow_linux_t* cwindow_get_data(err_t* err, cwindow_t handle) {

    if (windows[handle].state & CORE_CWINDOW_READY == 0) {
        throw(err, CORE_WINNREADY);
        return NULL;
    }

    return &windows[handle];

}

struct wl_display* cwm_get_display(void)        { return display;       }
struct wl_registry* cwm_get_registry(void)      { return registry;      }
struct wl_compositor* cwm_get_compositor(void)  { return compositor;    }
struct xdg_wm_base* cwm_get_xdg_wm_base(void)   { return xdg_wm_base;   }
