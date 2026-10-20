#include <window/linux/wayland/wayland-xdg.c>
#include <window/linux/wayland/wayland-seat.c>

internal struct wl_registry*    registry        = NULL;
internal struct wl_display*     display         = NULL;
internal struct wl_compositor*  compositor      = NULL;

internal void reg_global(void* data, struct wl_registry* registry, uint32_t name, const char* iface, uint32_t version) {
    
    // @note make sure the version provided is valid
    if (version < 1)
        return;
    
    if (strcmp(iface, wl_compositor_interface.name) == 0) {

        if (version > 4) {
            cdbg_warnf("using wayland compositor version 4 instead of %d for compatibility", version);
            version = 4;
        }
        
        // @note bind the wayland compositor to our registry
        compositor = wl_registry_bind(registry, name, &wl_compositor_interface, version);
        if (compositor == NULL) {
            cdbg_err("failed to bind wayland compositor");
            return;
        }

    } else if (strcmp(iface, xdg_wm_base_interface.name) == 0) {
        
        if (version > 2) {
            cdbg_warnf("using xdg_wm_base version 2 instead of %d for compatibility", version);
            version = 2;
        }

        // @note bind the xdg window manager to our registry and add our listeners
        xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, version);
        if (xdg_wm_base == NULL) { 
            cdbg_err("failed to bind xdg_wm_base");
            return;
        }

        if (xdg_wm_base_add_listener(xdg_wm_base, &wm_listener, NULL) < 0) {
            cdbg_err("failed to add xdg_wm_base listener");
            return;
        }

    } else if (strcmp(iface, wl_seat_interface.name) == 0) {
        
        if (version > 7) {
            cdbg_warnf("using wayland seat version 7 instead of %d for compatibility", version);
            version = 7;
        }

        // @note bind a wayland seat for input management to our registry and add our listeners
        seat = wl_registry_bind(registry, name, &wl_seat_interface, version);
        if (seat == NULL) { 
            cdbg_err("failed to bind wayland seat");
            return;
        }

        if (wl_seat_add_listener(seat, &seat_listener, NULL) < 0) {
            cdbg_err("failed to add wayland seat listener");
            return;
        }

    }

}

internal void reg_remove(void* data, struct wl_registry* registry, uint32_t n) {

    (void)data;
    (void)registry;
    (void)n;

    cdbg_warnf(" -- reg_remove invoked\nregistry: %p\ncompositor: %p\nseat: %p", registry, compositor, seat);

}

internal const struct wl_registry_listener registry_listener = {
    reg_global, reg_remove
};