internal void toplevel_configure(void* data, struct xdg_toplevel* tl, int32_t width, int32_t height, struct wl_array* states);
internal void toplevel_close(void* data, struct xdg_toplevel* tl);
internal const struct xdg_toplevel_listener toplevel_listener = {
    toplevel_configure, toplevel_close  
};

internal void xdg_surface_configure(void* data, struct xdg_surface* xs, uint32_t serial);
internal const struct xdg_surface_listener xdg_surface_listener = {
    xdg_surface_configure
};

internal void wm_ping(void* data, struct xdg_wm_base* wm, uint32_t serial);
internal const struct xdg_wm_base_listener wm_listener = {
    wm_ping
};

internal void reg_global(void* data, struct wl_registry* registry, uint32_t name, const char* iface, uint32_t version);
internal void reg_remove(void* data, struct wl_registry* registry, uint32_t n);
internal const struct wl_registry_listener registry_listener= {
    reg_global, reg_remove
};

internal void seat_capabilities(void* data, struct wl_seat* wl_seat, uint32_t capabilities);
internal void seat_name(void* data, struct wl_seat* wl_seat, const char* name);
internal const struct wl_seat_listener seat_listener= {
    seat_capabilities, seat_name
};

internal void keyboard_keymap(void* data, struct wl_keyboard* wl_keyboard, uint32_t format, int32_t fd, uint32_t size);
internal void keyboard_enter(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* surface, struct wl_array* keys);
internal void keyboard_leave(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* surface);
internal void keyboard_key(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
internal void keyboard_modifiers(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
internal void keyboard_repeat_info(void* data, struct wl_keyboard* wl_keyboard, int32_t rate, int32_t delay);
internal const struct wl_keyboard_listener keyboard_listener = {
    keyboard_keymap, keyboard_enter, keyboard_leave, keyboard_key, keyboard_modifiers, keyboard_repeat_info
};

internal void pointer_enter(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y);
internal void pointer_leave(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* surface);
internal void pointer_motion(void* data, struct wl_pointer* wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y);
internal void pointer_button(void* data, struct wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
internal void pointer_axis(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value);
internal void pointer_frame(void* data, struct wl_pointer* wl_pointer);
internal void pointer_axis_source(void* data, struct wl_pointer* wl_pointer, uint32_t axis_source);
internal void pointer_axis_stop(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis);
internal void pointer_axis_discrete(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t discrete);
internal void pointer_axis_value120(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t value120);
internal void pointer_axis_relative_direction(void *data, struct wl_pointer *wl_pointer, uint32_t axis, uint32_t direction); 
internal const struct wl_pointer_listener pointer_listener = {
    pointer_enter, pointer_leave, pointer_motion, pointer_button, pointer_axis, pointer_frame, pointer_axis_source, 
    pointer_axis_stop, pointer_axis_discrete, pointer_axis_value120, pointer_axis_relative_direction
};

internal void toplevel_configure(void* data, struct xdg_toplevel* tl, int32_t width, int32_t height, struct wl_array* states) {
    
    (void)tl;
    (void)states;
    
    cwindow_linux_t* window = (cwindow_linux_t*)data;
    if (width > 0 && height > 0) {

        window->size[X] = width;
        window->size[Y] = height;
        window->state |= CORE_CWINDOW_RESIZED;

        err_t err = CORE_SUCCESS;
        catch(&err, cgapi_handle_resize(&err, window->handle),
            throw(&err, err);
        );
        
    }

}

internal void toplevel_close(void* data, struct xdg_toplevel* tl) {
    
    (void)tl;

    cwindow_linux_t* window = (cwindow_linux_t*)data;
    window->state |= CORE_CWINDOW_CLOSED;

}

internal void xdg_surface_configure(void* data, struct xdg_surface* xs, uint32_t serial) {
    
    cwindow_linux_t* window = (cwindow_linux_t*)data;
    xdg_surface_ack_configure(xs, serial);
    window->state |= CORE_CWINDOW_READY;

}

internal void wm_ping(void* data, struct xdg_wm_base* wm, uint32_t serial) {
    
    (void)data;
    xdg_wm_base_pong(wm, serial);

}

internal void reg_global(void* data, struct wl_registry* registry, uint32_t name, const char* iface, uint32_t version) {
    
    if (strcmp(iface, wl_compositor_interface.name) == 0)
        compositor = wl_registry_bind(registry, name, &wl_compositor_interface, (version > 4 ? 4 : version));
    else if (strcmp(iface, xdg_wm_base_interface.name) == 0) {
        xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, (version > 2 ? 2 : version));
        xdg_wm_base_add_listener(xdg_wm_base, &wm_listener, NULL);
    } else if (strcmp(iface, wl_seat_interface.name) == 0) {
        if (version < 7) return; // @note assume version 7, im not implementing all that
        seat = wl_registry_bind(registry, name, &wl_seat_interface, version > 9 ? 9 : version);
        wl_seat_add_listener(seat, &seat_listener, NULL);
    }

}

internal void reg_remove(void* data, struct wl_registry* registry, uint32_t n) { 

    (void)data;
    (void)registry;
    (void)n; 

}

internal void seat_capabilities(void* data, struct wl_seat* wl_seat, uint32_t capabilities) {

    (void)data;

    if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD > 0 && keyboard == NULL) {
        keyboard = wl_seat_get_keyboard(wl_seat);
        wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
    } else if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD == 0 && keyboard != NULL) {
        wl_keyboard_release(keyboard);
        keyboard = NULL;
    }
 
    if (capabilities & WL_SEAT_CAPABILITY_POINTER > 0 && pointer == NULL) {
        pointer = wl_seat_get_pointer(wl_seat);
        wl_pointer_add_listener(pointer, &pointer_listener, NULL);
    } else if (capabilities & WL_SEAT_CAPABILITY_POINTER == 0 && pointer != NULL) {
        wl_pointer_release(pointer);
        pointer = NULL;
    }

}

internal void seat_name(void* data, struct wl_seat* wl_seat, const char *name) {

    (void)data;
    (void)wl_seat;
    (void)name;

}

internal void keyboard_keymap(void* data, struct wl_keyboard* wl_keyboard, uint32_t format, int32_t fd, uint32_t size) {}
internal void keyboard_enter(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* surface, struct wl_array* keys) {}
internal void keyboard_leave(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* surface) {}
internal void keyboard_key(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {}
internal void keyboard_modifiers(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {}
internal void keyboard_repeat_info(void* data, struct wl_keyboard* wl_keyboard, int32_t rate, int32_t delay) {}

internal void pointer_enter(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {}
internal void pointer_leave(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* surface) {}
internal void pointer_motion(void* data, struct wl_pointer* wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {}
internal void pointer_button(void* data, struct wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {}
internal void pointer_axis(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {}
internal void pointer_frame(void* data, struct wl_pointer* wl_pointer) {}
internal void pointer_axis_source(void* data, struct wl_pointer* wl_pointer, uint32_t axis_source) {}
internal void pointer_axis_stop(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis) {}
internal void pointer_axis_discrete(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t discrete) {}
internal void pointer_axis_value120(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t value120) {}
internal void pointer_axis_relative_direction(void *data, struct wl_pointer *wl_pointer, uint32_t axis, uint32_t direction) {}