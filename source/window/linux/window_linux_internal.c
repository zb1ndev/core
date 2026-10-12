internal void toplevel_configure    (void *data, struct xdg_toplevel *tl, int32_t width, int32_t height, struct wl_array *states);
internal void reg_global            (void* data, struct wl_registry* registry, uint32_t name, const char* iface, uint32_t version);
internal void xdg_surface_configure (void *data, struct xdg_surface *xs, uint32_t serial);
internal void reg_remove            (void* data, struct wl_registry* registry, uint32_t n);
internal void wm_ping               (void *data, struct xdg_wm_base *wm, uint32_t serial);
internal void toplevel_close        (void *data, struct xdg_toplevel *tl);

internal const struct xdg_toplevel_listener toplevel_listener       = { toplevel_configure, toplevel_close  };
internal const struct xdg_surface_listener  xdg_surface_listener    = { xdg_surface_configure               };
internal const struct xdg_wm_base_listener  wm_listener             = { wm_ping                             };
internal const struct wl_registry_listener  registry_listener       = { reg_global, reg_remove              };

internal void toplevel_configure(void *data, struct xdg_toplevel *tl, int32_t width, int32_t height, struct wl_array *states) {
    
    (void)tl;
    (void)states;
    
    cwindow_linux_t* window = (cwindow_linux_t*)data;
    if (width > 0 && height > 0) {

        window->size[X] = width;
        window->size[Y] = height;
        window->state |= CORE_CWINDOW_RESIZED;

        err_t err = CORE_SUCCESS;
        catch(&err, cgapi_handle_resize(&err, window->handle),
            cdbg_warnf("%d: failed to handle resize", err);
        );
        
    }

}

internal void toplevel_close(void *data, struct xdg_toplevel *tl) {
    
    (void)tl;

    cwindow_linux_t* window = (cwindow_linux_t*)data;
    window->state |= CORE_CWINDOW_CLOSED;

}

internal void xdg_surface_configure(void *data, struct xdg_surface *xs, uint32_t serial) {
    
    cwindow_linux_t* window = (cwindow_linux_t*)data;
    xdg_surface_ack_configure(xs, serial);
    window->state |= CORE_CWINDOW_READY;

}

internal void wm_ping(void *data, struct xdg_wm_base *wm, uint32_t serial) {
    
    (void)data;
    xdg_wm_base_pong(wm, serial);

}

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