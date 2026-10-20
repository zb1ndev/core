#include <window/linux/window-linux.h>

internal struct wl_seat*        seat            = NULL;
internal struct wl_keyboard*    keyboard        = NULL;
internal struct wl_pointer*     pointer         = NULL;

internal void keyboard_keymap(void* data, struct wl_keyboard* wl_keyboard, uint32_t format, int32_t fd, uint32_t size) {

}

internal void keyboard_enter(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* surface, struct wl_array* keys) {

}

internal void keyboard_leave(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, struct wl_surface* surface) {

}

internal void keyboard_key(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {

    if (key < CORE_KEYCODE_COUNT && key > 0)
        cim_set_state(key, state);

}

internal void keyboard_modifiers(void* data, struct wl_keyboard* wl_keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {

}

internal void keyboard_repeat_info(void* data, struct wl_keyboard* wl_keyboard, int32_t rate, int32_t delay) {

}

internal const struct wl_keyboard_listener keyboard_listener = {
    keyboard_keymap, keyboard_enter, keyboard_leave, keyboard_key, keyboard_modifiers, keyboard_repeat_info
};

internal void pointer_enter(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {

}

internal void pointer_leave(void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* surface) {

}

internal void pointer_motion(void* data, struct wl_pointer* wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {

}

internal void pointer_button(void* data, struct wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {

    if (button <= CORE_MOUSEB_MIDDLE && button >= CORE_MOUSEB_LEFT)
        cim_set_state(button, state);
    cdbg_logf("button: %u, state: %u", button, state);

}

internal void pointer_axis(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {

}

internal void pointer_frame(void* data, struct wl_pointer* wl_pointer) {

}

internal void pointer_axis_source(void* data, struct wl_pointer* wl_pointer, uint32_t axis_source) {

}

internal void pointer_axis_stop(void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis) {

}

internal void pointer_axis_discrete(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t discrete) {

}

internal void pointer_axis_value120(void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t value120) {

}

internal void pointer_axis_relative_direction(void *data, struct wl_pointer *wl_pointer, uint32_t axis, uint32_t direction) {

}

internal const struct wl_pointer_listener pointer_listener = {
    pointer_enter, pointer_leave, pointer_motion, pointer_button, pointer_axis, pointer_frame, pointer_axis_source, 
    pointer_axis_stop, pointer_axis_discrete, pointer_axis_value120, pointer_axis_relative_direction
};


internal void seat_capabilities(void* data, struct wl_seat* wl_seat, uint32_t capabilities) {

    (void)data;

    // @note handle connection and disconnection of keyboards
    if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD > 0 && keyboard == NULL) {

        keyboard = wl_seat_get_keyboard(wl_seat);
        if (wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL) < 0)
            cdbg_err("failed to add wayland keyboard listener");
    
    } else if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD == 0 && keyboard != NULL) {
    
        wl_keyboard_release(keyboard);
        keyboard = NULL;
    
    }
 
    // @note handle connection and disconnection of pointers
    if (capabilities & WL_SEAT_CAPABILITY_POINTER > 0 && pointer == NULL) {
    
        pointer = wl_seat_get_pointer(wl_seat);
        if (wl_pointer_add_listener(pointer, &pointer_listener, NULL) < 0)
            cdbg_err("failed to add wayland pointer listener");
    
    } else if (capabilities & WL_SEAT_CAPABILITY_POINTER == 0 && pointer != NULL) {
    
        wl_pointer_release(pointer);
        pointer = NULL;
    
    }

}

internal void seat_name(void* data, struct wl_seat* wl_seat, const char* name) {

    (void)data;
    (void)wl_seat;
    (void)name;

}

internal const struct wl_seat_listener seat_listener = {
    seat_capabilities, seat_name
};