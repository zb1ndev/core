#include "linux/window_wayland.c"

/*
    The Plan is to keep the rendering code and 
    data completely seperate from the windowing 
    system in the case I want to either add more
    graphics apis or do software rendering.
*/

internal struct internal_window_t {

    cvec2u_t            size;
    cvec2f_t            sizef;

    const char*         title;

    struct wl_surface*  surface;
    struct xdg_surface* xdg_surface;
    struct xdg_toplevel*toplevel;

} windows[CORE_CWINDOW_MAX]                         = {0};

internal cwindow_t window_count                     = 0;
internal cwindow_t free_windows[CORE_CWINDOW_MAX]   = {0};
internal bool window_manager_initialized            = false;

internal void cwindow_manager_initialize(err_t* err) {
    
    for (cwindow_t w = 0; w < CORE_CWINDOW_MAX; w++)
        free_windows[w] = (CORE_CWINDOW_MAX - 1) - w;

    display = wl_display_connect(NULL);
    if (display == NULL) {
        throw(err, CORE_DISPCONREF);
        return;
    }

    registry = wl_display_get_registry(display);
    if (registry == NULL) {
        throw(err, CORE_PINVALID);
        return;
    }

    if (wl_registry_add_listener(registry, &registry_listener, NULL) < 0) {
        throw(err, CORE_PINVALID);
        return;
    }

    wl_display_roundtrip(display);
    wl_display_roundtrip(display);

    if (compositor == NULL || xdg_wm_base == NULL) {
        throw(err,CORE_PINVALID);
        return;
    }

    window_manager_initialized = true;

}

internal void cwindow_manager_deinitialize(err_t* err) {

    // xdg_wm_base_destroy(runtime->xdg_wm_base);
    // wl_compositor_destroy(runtime->compositor);
    // wl_registry_destroy(runtime->registry);
    // wl_display_disconnect(runtime->display);

    window_manager_initialized = false;

}

cwindow_t cwindow_create(err_t* err, const char* title, cvec2u_t size) {

    if (window_manager_initialized == false)
        catch(err, cwindow_manager_initialize(err), return;);

    if (window_count >= CORE_CWINDOW_MAX) {
        throw(err, CORE_WINMAXC);
        return CORE_CWINDOW_INVALID;
    }

    cwindow_t result = free_windows[CORE_CWINDOW_MAX - ++window_count];
    struct internal_window_t* window = &windows[result];

    *window = (struct internal_window_t) {
        .title = title,
        .size = {size[X], size[Y]},
        .sizef = {(float)size[X], (float)size[Y]}
    };
 
    // out->surface = wl_compositor_create_surface(runtime->compositor);
 
    // out->xdg_surface = xdg_wm_base_get_xdg_surface(runtime->xdg_wm_base, out->surface);
    // xdg_surface_add_listener(out->xdg_surface, &xdg_surface_listener, out);
 
    // out->toplevel = xdg_surface_get_toplevel(out->xdg_surface);
    // xdg_toplevel_add_listener(out->toplevel, &toplevel_listener, out);
    // xdg_toplevel_set_title(out->toplevel, title);
 
    // wl_surface_commit(out->surface);
    // wl_display_roundtrip(runtime->display);

    return result;

}

void cwindow_destroy(err_t* err, cwindow_t handle) {

    // xdg_toplevel_destroy(window->toplevel);
    // xdg_surface_destroy(window->xdg_surface);
    // wl_surface_destroy(window->surface);

}

void cwindow_get_size(err_t* err, cwindow_t handle, cvec2u_t* out) {

}

void cwindow_get_sizef(err_t* err, cwindow_t handle, cvec2f_t* out) {

}

char* cwindow_get_title(err_t* err, cwindow_t handle) {

}

bool cwindow_get_fullscreen(err_t* err, cwindow_t handle) {

}

bool cwindow_get_minimized(err_t* err, cwindow_t handle) {

}

bool cwindow_get_resized(err_t* err, cwindow_t handle) {

}

bool cwindow_should_close(err_t* err, cwindow_t handle) {

}

void cwindow_set_size(err_t* err, cwindow_t handle, cvec2u_t value) {

}

void cwindow_set_title(err_t* err, cwindow_t handle, const char* value) {

}

void cwindow_set_icon(err_t* err, cwindow_t handle, uint8_t* data, uint64_t data_size) {

}

void cwindow_set_fullscreen(err_t* err, cwindow_t handle, bool value) {

}

void cwindow_set_minimized(err_t* err, cwindow_t handle, bool value) {

}

void cwindow_set_resized(err_t* err, cwindow_t handle, bool value) {

}

void cwindow_close(err_t* err, cwindow_t handle) {

}