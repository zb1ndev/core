#if !defined(CORE_INPUT_H)
#define CORE_INPUT_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <window/window.h>

typedef enum cinput_kc_t cinput_kc_t;
enum cinput_kc_t {

    CORE_KEYCODE_COUNT

};

typedef enum cinput_mod_t cinput_mod_t;
enum cinput_mod_t {

    CORE_MODIFIER_COUNT

};

typedef struct cinput_t cinput_t;
struct cinput_t {

    bool                modifiers[CORE_MODIFIER_COUNT];
    bool                input_states[CORE_KEYCODE_COUNT];

    #define CORE_PRESSED                            (true)
    #define CORE_RELEASED                           (false)

    cvec2i_t            mouse_position;
    int32_t             scroll_wheel;
    bool                focused;

};

void                    cinput_update               (err_t* err, cwindow_t handle);

bool                    cinput_query_modifiers      (err_t* err, cwindow_t handle, cinput_mod_t mod);
bool                    cinput_query_state          (err_t* err, cwindow_t handle, cinput_kc_t keycode);
void                    cinput_get_mouse_position   (err_t* err, cwindow_t handle, cvec2i_t* out);
int32_t                 cinput_get_scrollwheel      (err_t* err, cwindow_t handle);
bool                    cinput_is_focused           (err_t* err, cwindow_t handle);

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_INPUT_H