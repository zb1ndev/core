#if !defined(CORE_WINDOW_LINUX_H)
#define CORE_WINDOW_LINUX_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <window/window.h>

typedef struct cwindow_linux_t cwindow_linux_t;
struct cwindow_linux_t {

    cwindow_t           handle;
    cinput_t            input;
    const char*         title;

    cvec2u_t            size;
    cvec2f_t            sizef;

    struct wl_surface*  surface;
    struct xdg_surface* xdg_surface;
    struct xdg_toplevel*toplevel;

    uint64_t            state;

    void (*gapi_cleanup)(err_t* err, cwindow_t handle);

};

cwindow_linux_t*        cwindow_get_data            (err_t* err, cwindow_t handle);

struct wl_display*      cwm_get_display             (void);
struct wl_registry*     cwm_get_registry            (void);
struct wl_compositor*   cwm_get_compositor          (void);
struct xdg_wm_base*     cwm_get_xdg_wm_base         (void);

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_WINDOW_LINUX_H