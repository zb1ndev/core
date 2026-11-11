#include <window/linux/window-linux.h>

internal struct zwp_pointer_constraints_v1*         pointer_constraints         = NULL;
internal struct zwp_relative_pointer_manager_v1*    relative_pointer_manager    =  NULL;

internal struct zwp_locked_pointer_v1*              locked_pointer              = NULL;
internal struct zwp_relative_pointer_v1*            relative_pointer            = NULL;

internal void locked_pointer_locked(void* data, struct zwp_locked_pointer_v1* locked_pointer) {

    cwindow_linux_t* window = (cwindow_linux_t*)data;
    if (window->state & CORE_CWINDOW_MOUSE_HIDDEN > 0)
        wl_pointer_set_cursor(pointer, last_pointer_serial, NULL, 0, 0);

}

internal void locked_pointer_unlocked(void* data, struct zwp_locked_pointer_v1* locked_pointer) {

    (void)data;
    (void)locked_pointer;

}

internal const struct zwp_locked_pointer_v1_listener lock_pointer_listener = {
    locked_pointer_locked, locked_pointer_unlocked 
};

internal void relative_pointer_motion(

    void *data, 
    struct zwp_relative_pointer_v1 *zwp_relative_pointer_v1, 
    uint32_t utime_hi, 
    uint32_t utime_lo, 
    wl_fixed_t dx, 
    wl_fixed_t dy, 
    wl_fixed_t dx_unaccel, 
    wl_fixed_t dy_unaccel

) {

    (void)data;
    (void)zwp_relative_pointer_v1;
    (void)utime_hi;
    (void)utime_lo;
    (void)dx_unaccel;
    (void)dy_unaccel;

    // @todo find a way to zero the axis when we are not moving
    cim_set_mouse_axis((vec2){(float)wl_fixed_to_double(dx), (float)wl_fixed_to_double(dy)});

}

internal const struct zwp_relative_pointer_v1_listener relative_pointer_listener = {
    relative_pointer_motion
};