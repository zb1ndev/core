#include <input/input-manager.h>

internal bool   key_states[CORE_KEYCODE_COUNT]  = {0};
internal ivec2  mouse_position                  = {0};
internal bool   mouse_button_states[3]          = {0}; // @note left | middle | right


void cinput_get_mouse_position(err_t* err, ivec2* out) {

    if (out == NULL) {
        throw(err, CORE_PINVALID);
        return;
    }

    (*out)[0] = mouse_position[0];
    (*out)[1] = mouse_position[1];

}

bool cinput_get_button(err_t* err, cinput_kc_t key_code) {

    if (key_code == CORE_MOUSEB_LEFT || key_code == CORE_MOUSEB_MIDDLE || key_code == CORE_MOUSEB_RIGHT) {
        return mouse_button_states[key_code];
    }

    if (key_code > CORE_KEYCODE_COUNT || key_code < 0) {
        throw(err, CORE_PINVALID);
        return false;
    }

    return key_states[key_code];

}

void cim_set_state(cinput_kc_t key_code, bool value) { 

    if (key_code == CORE_MOUSEB_LEFT || key_code == CORE_MOUSEB_MIDDLE || key_code == CORE_MOUSEB_RIGHT) {
        mouse_button_states[key_code] = value;
        return;
    }

    key_states[key_code] = value;

}

void cim_set_mouse_position(ivec2 value) { 

    mouse_position[0] = value[0]; 
    mouse_position[1] = value[1]; 

}