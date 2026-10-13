#include <window/linux/window_linux.h>

// internal FILE* dev_input; // need a stream from dev/input to handle gamepad controls

void cinput_update(err_t* err, cwindow_t handle) {

    catch(err, if (cinput_is_focused(err, handle) == false) return, 
        return;
    );

    // keyboard and mouse events will be handled through wayland callbacks,
    // though we still want to handle gamepad input so we need to look into
    // and poll dev/input for devices and data

    // we poll the input and only update the windows that are focused
    // this function will be called by cwindow_update and will be passed each window 
    // for it to update the state

}

bool cinput_query_modifiers(err_t* err, cwindow_t handle, cinput_mod_t mod) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return false;
    );

    if (mod < 0 || mod >= CORE_MODIFIER_COUNT) {
        throw(err, CORE_PINVALID);
        return false;
    }

    return window->input.modifiers[mod];

}

bool cinput_query_state(err_t* err, cwindow_t handle, cinput_kc_t keycode) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return false;
    );

    if (keycode < 0 || keycode >= CORE_KEYCODE_COUNT) {
        throw(err, CORE_PINVALID);
        return false;
    }

    return window->input.modifiers[keycode];

}

void cinput_get_mouse_position(err_t* err, cwindow_t handle, cvec2i_t* out) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return;
    );

    if (out == NULL) {
        throw(err, CORE_PINVALID);
        return;
    }

    *out[X] = window->input.mouse_position[X];
    *out[Y] = window->input.mouse_position[Y];

}

int32_t cinput_get_scrollwheel(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return UINT32_MAX;
    );

    return window->input.scroll_wheel;

}

bool cinput_is_focused(err_t* err, cwindow_t handle) {

    cwindow_linux_t* window = NULL;
    catch(err, window = cwindow_get_data(err, handle), 
        return false;
    );

    return window->input.focused;

}