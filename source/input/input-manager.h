#if !defined(CORE_INPUT_MANAGER_H)
#define CORE_INPUT_MANAGER_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// @note    The following code should only be accessible by internal translation units
//          and therefor this header should only be included by implmentation files.

#include <core.h>

/*  Sets the input state of the provided `key_code` to `value`.
    @param key_code The key or mouse button you want to set the state of.
    @param value The value you want to set `key_code` to.
    @returns N/A
    @note This is an internal function for updating input via windowing callbacks.
    @note `cinput_kc_t` also includes mouse buttons.
*/
void                 cim_set_state               (cinput_kc_t key_code, bool value);

/*  Clears all keyboard and mouse input state ( excludes position ).
    @returns N/A
    @note This is an internal function for updating input via windowing callbacks.
*/
void                 cim_clear_state               (void);

/*  Sets the internal mouse position data to `value`.
    @param value The value you want to set the mouse position to.
    @returns N/A
    @note This is an internal function for updating input via windowing callbacks.
*/
void                 cim_set_mouse_position      (ivec2 value);



#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_INPUT_MANAGER_H