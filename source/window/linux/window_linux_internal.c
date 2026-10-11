#include <window/linux/window_linux.h>

/*
    The Plan is to keep the rendering code and 
    data completely seperate from the windowing 
    system in the case I want to either add more
    graphics apis or do software rendering.
*/

internal cwindow_linux_t windows[CORE_CWINDOW_MAX]  = {0};
internal cwindow_t window_count                     = 0;
internal cwindow_t free_windows[CORE_CWINDOW_MAX]   = {0};
internal bool window_manager_initialized            = false;

internal struct wl_display*     display;
internal struct wl_registry*    registry;
internal struct wl_compositor*  compositor;
internal struct xdg_wm_base*    xdg_wm_base;

internal void toplevel_configure(void *data, struct xdg_toplevel *tl, int32_t width, int32_t height, struct wl_array *states) {
    
    (void)tl;
    (void)states;
    
    cwindow_linux_t* window = (cwindow_linux_t*)data;
    if (width > 0 && height > 0) {
        window->size[X] = width;
        window->size[Y] = height;
        window->state |= CORE_CWINDOW_RESIZED;
    }

}

internal void toplevel_close(void *data, struct xdg_toplevel *tl) {
    
    (void)tl;

    cwindow_linux_t* window = (cwindow_linux_t*)data;
    window->state |= CORE_CWINDOW_CLOSED;

}

internal const struct xdg_toplevel_listener toplevel_listener = 
    { toplevel_configure, toplevel_close };

internal void xdg_surface_configure(void *data, struct xdg_surface *xs, uint32_t serial) {
    
    cwindow_linux_t* window = (cwindow_linux_t*)data;
    xdg_surface_ack_configure(xs, serial);
    window->state |= CORE_CWINDOW_READY;

}

internal const struct xdg_surface_listener xdg_surface_listener = 
    { xdg_surface_configure };

internal void wm_ping(void *data, struct xdg_wm_base *wm, uint32_t serial) {
    
    (void)data;
    xdg_wm_base_pong(wm, serial);

}

internal const struct xdg_wm_base_listener wm_listener = 
    { wm_ping };

internal void reg_global(void* data, struct wl_registry* registry, uint32_t name, const char* iface, uint32_t version) {
    
    if (!strcmp(iface, wl_compositor_interface.name))
        compositor = wl_registry_bind(registry, name, &wl_compositor_interface, (version > 4 ? 4 : version));
    
    else if (!strcmp(iface, xdg_wm_base_interface.name)) {
        xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, (version > 2 ? 2 : version));
        xdg_wm_base_add_listener(xdg_wm_base, &wm_listener, NULL);
    }

}

internal void reg_remove(void* data, struct wl_registry* registry, uint32_t n) { 

    (void)data;
    (void)registry;
    (void)n; 

}

internal const struct wl_registry_listener registry_listener = 
    { reg_global, reg_remove };