#include <window/linux/window-linux.h>

internal cwindow_linux_t    windows_data[CORE_CWINDOW_MAX]  = {0};
internal cwindow_t          free_windows[CORE_CWINDOW_MAX]  = {0};
internal cwindow_t          window_count                    = 0;
internal bool               window_manager_initialized      = false;

#define cwindow_is_valid(handle) (handle == CORE_CWINDOW_INVALID || windows_data[handle].state & CORE_CWINDOW_READY == 0)

cwindow_linux_t* cwindow_get(err_t* err, cwindow_t handle) {
    
    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return NULL;
    }

    return &windows_data[handle];

}

// @note registry, display & compositor -> wayland-registry.c
//       seat, keyboard & pointer       -> wayland-seat.c
//       xdg_wm_base                    -> wayland-xdg.c

#include <window/linux/wayland/wayland-registry.c>

cwindow_t cwindow_create(err_t* err, const char* title, ivec2 size) {

    if (window_manager_initialized == false)  {

        for (cwindow_t w = 0; w < CORE_CWINDOW_MAX; w++)
            free_windows[w] = (CORE_CWINDOW_MAX - 1) - w;

        if ((display = wl_display_connect(NULL)) == NULL) {
            throw(err, CORE_DISPCONREF);
            return CORE_CWINDOW_INVALID;
        }

        if ((registry = wl_display_get_registry(display)) == NULL) {
            throw(err, CORE_DIPREG);
            return CORE_CWINDOW_INVALID;
        }

        if (wl_registry_add_listener(registry, &registry_listener, NULL) < 0) {
            throw(err, CORE_REGADDLIS);
            return CORE_CWINDOW_INVALID;
        }

        wl_display_roundtrip(display);
        wl_display_roundtrip(display);

        if (compositor == NULL || xdg_wm_base == NULL) {
            throw(err, CORE_PINVALID);
            return CORE_CWINDOW_INVALID;
        }

        window_manager_initialized = true;

    }
   
    if (window_count >= CORE_CWINDOW_MAX) {
        throw(err, CORE_WINMAXC);
        return CORE_CWINDOW_INVALID;
    }

    cwindow_t result = free_windows[CORE_CWINDOW_MAX - ++window_count];
    cwindow_linux_t* window = &windows_data[result];

    *window = (cwindow_linux_t) {
        .handle = result,
        .title  = title,
        .size   = {size[0], size[1]},
        .sizef  = {(float)size[0], (float)size[1]}
    };
    
    window->surface = wl_compositor_create_surface(compositor);
    if (window->surface == NULL) {
        throw(err, CORE_CREASURF);
        return CORE_CWINDOW_INVALID;
    }

    window->xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, window->surface);
    if (window->xdg_surface == NULL) {
        wl_surface_destroy(window->surface);
        throw(err, CORE_XDGGETSURF);
        return CORE_CWINDOW_INVALID;
    }

    if (xdg_surface_add_listener(window->xdg_surface, &xdg_surface_listener, window) < 0) {
        xdg_surface_destroy(window->xdg_surface);
        wl_surface_destroy(window->surface);
        throw(err, CORE_XDGSADDLIS);
        return CORE_CWINDOW_INVALID;
    }
 
    window->toplevel = xdg_surface_get_toplevel(window->xdg_surface);
    if (window->xdg_surface == NULL) {
        xdg_surface_destroy(window->xdg_surface);
        wl_surface_destroy(window->surface);
        throw(err, CORE_XDGGETTL);
        return CORE_CWINDOW_INVALID;
    }

    if (xdg_toplevel_add_listener(window->toplevel, &toplevel_listener, window) < 0) {
        xdg_toplevel_destroy(window->toplevel);
        xdg_surface_destroy(window->xdg_surface);
        wl_surface_destroy(window->surface);
        throw(err, CORE_XDGTLADDLIS);
        return CORE_CWINDOW_INVALID;
    }
    
    xdg_toplevel_set_title(window->toplevel, title);
    wl_surface_commit(window->surface);

    if (wl_display_roundtrip(display) < 0) {
        xdg_toplevel_destroy(window->toplevel);
        xdg_surface_destroy(window->xdg_surface);
        wl_surface_destroy(window->surface);
        throw(err, CORE_DISPRT);
        return CORE_CWINDOW_INVALID;
    }

    return result;

}

void cwindow_destroy(err_t* err, cwindow_t handle) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    cwindow_linux_t* window = &windows_data[handle];
    if (window->cgraf_cleanup != NULL)  {
        catch(err, window->cgraf_cleanup(err, handle),
            throw(err, *err);    
        );
    }

    xdg_toplevel_destroy(window->toplevel);
    xdg_surface_destroy(window->xdg_surface);
    wl_surface_destroy(window->surface);

    free_windows[CORE_CWINDOW_MAX - window_count] = handle;
    
    (*window) = (cwindow_linux_t) {
        .title = NULL, .size = {0,0},   .sizef = {0.0f,0.0f},
        .state = 0UL,  .surface = NULL, .toplevel = NULL, .xdg_surface = NULL
    };
 
    if (--window_count <= 0) {
        
        catch(err, cgraf_deinitialize(err), 
            throw(err, *err);
        );

        if (keyboard != NULL) {
            wl_keyboard_release(keyboard);
            keyboard = NULL;
        }

        if (pointer != NULL) {
            wl_pointer_release(pointer);
            pointer = NULL;
        }

        if (xdg_wm_base != NULL) {
            xdg_wm_base_destroy(xdg_wm_base);
            xdg_wm_base = NULL;
        }
        
        if (compositor != NULL) {
            wl_compositor_destroy(compositor);
            compositor = NULL;
        }

        if (registry != NULL) {
            wl_registry_destroy(registry);
            registry = NULL;
        }

        if (display != NULL) {
            wl_display_disconnect(display);
            display = NULL;
        }

        window_manager_initialized = false;

    }

}

void cwindow_update(err_t* err) {

    if (wl_display_dispatch_pending(cwindow_get_display()) < 0) {
        throw(err, CORE_DISPDISPAT);
        return;
    }

}

void cwindow_get_size(err_t* err, cwindow_t handle, ivec2* out) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    if (out == NULL) {
        throw(err, CORE_PINVALID);
        return;
    }    

    (*out)[0] = windows_data[handle].size[0];
    (*out)[1] = windows_data[handle].size[1];

}

void cwindow_get_sizef(err_t* err, cwindow_t handle, vec2* out) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    if (out == NULL) {
        throw(err, CORE_PINVALID);
        return;
    }    

    (*out)[0] = windows_data[handle].sizef[0];
    (*out)[1] = windows_data[handle].sizef[1];

}

const char* cwindow_get_title(err_t* err, cwindow_t handle) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return NULL;
    }

    return windows_data[handle].title;

}

bool cwindow_get_fullscreen(err_t* err, cwindow_t handle) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return false;
    }

    return windows_data[handle].state & CORE_CWINDOW_FULLSCREEN != 0;

}

bool cwindow_get_minimized(err_t* err, cwindow_t handle) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return false;
    }

    return windows_data[handle].state & CORE_CWINDOW_MINIMIZED != 0;

}

bool cwindow_get_resized(err_t* err, cwindow_t handle) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return false;
    }

    return windows_data[handle].state & CORE_CWINDOW_RESIZED != 0;

}

bool cwindow_get_closed(err_t* err, cwindow_t handle) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return false;
    }

    return windows_data[handle].state & CORE_CWINDOW_CLOSED != 0;

}

void cwindow_set_size(err_t* err, cwindow_t handle, ivec2 value) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows_data[handle].size[0] = value[0];
    windows_data[handle].sizef[0] = (float)value[0];
    windows_data[handle].size[1] = value[1];
    windows_data[handle].sizef[1] = (float)value[1];

    cwindow_set_resized(err, handle, true);

}

void cwindow_set_title(err_t* err, cwindow_t handle, const char* value) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    if (value == NULL) {
        throw(err, CORE_PINVALID);
        return;
    }    

    windows_data[handle].title = value;
    xdg_toplevel_set_title(windows_data[handle].toplevel, value);

}

void cwindow_set_icon(err_t* err, cwindow_t handle, uint8_t* data, uint64_t data_size) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    if (data == NULL || data_size == 0) {
        throw(err, CORE_PINVALID);
        return;
    }    

    cdbg_warn("not implemented");

}

void cwindow_set_fullscreen(err_t* err, cwindow_t handle, bool value) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows_data[handle].state = value 
        ? windows_data[handle].state | CORE_CWINDOW_FULLSCREEN
        : windows_data[handle].state & ~CORE_CWINDOW_FULLSCREEN;
    cwindow_set_resized(err, handle, true);

}

void cwindow_set_minimized(err_t* err, cwindow_t handle, bool value) {
    
    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows_data[handle].state = value 
        ? windows_data[handle].state | CORE_CWINDOW_MINIMIZED
        : windows_data[handle].state & ~CORE_CWINDOW_MINIMIZED;
    cwindow_set_resized(err, handle, true);
}

void cwindow_set_resized(err_t* err, cwindow_t handle, bool value) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows_data[handle].state = value 
        ? windows_data[handle].state | CORE_CWINDOW_RESIZED
        : windows_data[handle].state & ~CORE_CWINDOW_RESIZED;

}

void cwindow_close(err_t* err, cwindow_t handle) {

    if (cwindow_is_valid(handle)) {
        throw(err, CORE_WINNREADY);
        return;
    }

    windows_data[handle].state |= CORE_CWINDOW_CLOSED;

}

inline struct wl_display* cwindow_get_display(void)         { return display;       }
inline struct wl_compositor* cwindow_get_compositor(void)   { return compositor;    }
inline struct xdg_wm_base* cwindow_get_xdg_wm_base(void)    { return xdg_wm_base;   }