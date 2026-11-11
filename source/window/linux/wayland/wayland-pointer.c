#include <window/linux/window-linux.h>

internal struct zwp_pointer_constraints_v1*         pointer_constraints         = NULL;
internal struct zwp_relative_pointer_manager_v1*    relative_pointer_manager    =  NULL;

internal struct zwp_locked_pointer_v1*              locked_pointer              = NULL;
internal struct zwp_relative_pointer_v1*            relative_pointer            = NULL;

internal void locked_pointer_handle_locked(void* data, struct zwp_locked_pointer_v1* locked_pointer) {

    cwindow_linux_t* window = (cwindow_linux_t*)data;
    if (window->state & CORE_CWINDOW_MOUSE_HIDDEN > 0)
        wl_pointer_set_cursor(pointer, last_pointer_serial, NULL, 0, 0);

}

internal void locked_pointer_handle_unlocked(void* data, struct zwp_locked_pointer_v1* locked_pointer) {

    (void)data;
    (void)locked_pointer;

}

internal const struct zwp_locked_pointer_v1_listener lock_pointer_listener = {
    locked_pointer_handle_locked, locked_pointer_handle_unlocked 
};