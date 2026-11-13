#include <input/input-manager.h>

internal bool   key_states[CORE_KEYCODE_COUNT]  = {0};
internal bool   mouse_button_states[UINT8_MAX]  = {0};

internal ivec2  mouse_position                  = {0};
internal vec2   mouse_axis                      = {0};

void cinput_get_mouse_position(err_t* err, ivec2* out) {

    if (out == NULL) {
        cthrow(err, CORE_PINVALID);
        return;
    }

    (*out)[0] = mouse_position[0];
    (*out)[1] = mouse_position[1];

}

void cinput_get_mouse_axis(err_t* err, vec2* out) {

    if (out == NULL) {
        cthrow(err, CORE_PINVALID);
        return;
    }

    (*out)[0] = mouse_axis[0];
    (*out)[1] = mouse_axis[1];

}

bool cinput_get_key(err_t* err, cinput_kc_t key_code) {

    if (key_code > CORE_KEYCODE_COUNT || key_code < 0) {
        cthrow(err, CORE_PINVALID);
        return false;
    }

    return key_states[key_code];

}

bool cinput_get_mouse_button(err_t* err, uint8_t button) {

    if (button > 255) {
        cthrow(err, CORE_PINVALID);
        return false;
    }

    return mouse_button_states[button];

}

void cim_set_state(int32_t key_code, bool value) { 

    if ((key_code - CORE_MOUSEB_OFFSET) > 255 || key_code < 0)
        return;

    if (key_code >= CORE_MOUSEB_OFFSET && (key_code - CORE_MOUSEB_OFFSET) < 255) {
        mouse_button_states[key_code - CORE_MOUSEB_OFFSET] = value;
        return;
    }

    key_states[key_code] = value;

}

void cim_clear_state(void) {

    (void)memset(key_states, false, sizeof(bool) * CORE_KEYCODE_COUNT);
    (void)memset(mouse_button_states, false, sizeof(bool) * UINT8_MAX);

}

void cim_set_mouse_position(ivec2 value) { 

    mouse_position[0] = value[0]; 
    mouse_position[1] = value[1]; 

}

void cim_set_mouse_axis(vec2 value) {

    mouse_axis[0] = value[0];
    mouse_axis[1] = value[1];

}
