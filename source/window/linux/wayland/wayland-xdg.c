#include <window/linux/window-linux.h>

internal struct xdg_wm_base* xdg_wm_base = NULL;

internal void toplevel_configure(void* data, struct xdg_toplevel* tl, int32_t width, int32_t height, struct wl_array* states) {

    (void)tl;
    (void)states;

    err_t err = CORE_SUCCESS;
    cwindow_linux_t* window = (cwindow_linux_t*)data;

    // @note handle sizing
    if (width > 0 && height > 0) {

        window->size[0] = width;
        window->size[1] = height;
        window->state |= CORE_CWINDOW_RESIZED;

        catch(&err, cgraf_handle_resize(&err, window->handle),
            throw(&err, err);
        );
        
    }

}

internal void toplevel_close(void* data, struct xdg_toplevel* tl) {

    (void)tl;

    // @note set the closed flag
    ((cwindow_linux_t*)data)->state |= CORE_CWINDOW_CLOSED;

}

internal const struct xdg_toplevel_listener toplevel_listener = {
    toplevel_configure, toplevel_close  
};

internal void xdg_surface_configure(void* data, struct xdg_surface* xs, uint32_t serial) {

    xdg_surface_ack_configure(xs, serial);

    // @note set the ready flag
    ((cwindow_linux_t*)data)->state |= CORE_CWINDOW_READY;

}

internal const struct xdg_surface_listener xdg_surface_listener = {
    xdg_surface_configure
};

internal void wm_ping(void* data, struct xdg_wm_base* wm, uint32_t serial) {

    (void)data;
    xdg_wm_base_pong(wm, serial);

}

internal const struct xdg_wm_base_listener wm_listener = {
    wm_ping
};