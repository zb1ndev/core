#if !defined(CORE_WINDOW_LINUX_H)
#define CORE_WINDOW_LINUX_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

// @note    The following code should only be accessible by internal translation units
//          and therefor this header should only be included by implmentation files.

#include <core.h>
#include <input/input-manager.h> // @note input-manager.h includes core.h

/*  Window object holding linux-wayland specific data.
    @param handle A handle to itself.
    @param title The current title of the window.
    @param size The size of the window as an integer vector.
    @param sizef The size of the window as a float vector.
    @param surface The wayland surface.
    @param xdg_surface The xdg_surface.
    @param toplevel The xdg toplevel.
    @param state A bitmask representing the window state.
    @param cgraf_cleanup A graphics api cleanup callback.
    @param cgraf_draw_start A graphics api draw start callback.
    @param cgraf_draw_end A graphics api draw end callback.
    @note `state` options are...
    @note CORE_CWINDOW_CLOSED
    @note CORE_CWINDOW_FULLSCREEN
    @note CORE_CWINDOW_MAXIMIZED 
    @note CORE_CWINDOW_MINIMIZED 
    @note CORE_CWINDOW_RESIZED
    @note CORE_CWINDOW_READY
*/
typedef struct cwindow_linux_t cwindow_linux_t;
struct cwindow_linux_t {

    cwindow_t                   handle;
    const char*                 title;
    ivec2                       size;
    vec2                        sizef;

    struct wl_surface*          surface;
    struct xdg_surface*         xdg_surface;
    struct xdg_toplevel*        toplevel;

    cwindow_state_t             state;

    void (*cgraf_cleanup)       (err_t* err, cwindow_t handle);
    void (*cgraf_draw_start)    (err_t* err, cwindow_t handle);
    void (*cgraf_draw_end)      (err_t* err, cwindow_t handle);

};

/*  Gets the data ascociated with the provided handle.
    @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
    @param handle The handle of the window you want the data of.
    @returns A pointer to the window data.
    @note Only accessible by internal translation units.
*/
cwindow_linux_t*            cwindow_get                     (err_t* err, cwindow_t handle);

/*  Gets the data ascociated with the provided handle.
    @param err An optional (can be `NULL`) reference to an error object the function can use to throw errors.
    @param surface The surface of the window you want the data of.
    @returns A pointer to the window data.
    @note Only accessible by internal translation units.
*/
cwindow_linux_t*            cwindow_get_by_surface          (err_t* err, struct wl_surface* surface);

/*  Gets the wayland display managed internally.
    @returns A pointer to the wayland display object.
*/
struct wl_display*          cwindow_get_display             (void);

/*  Gets the wayland compositor managed internally.
    @returns A pointer to the wayland compositor object.
*/
struct wl_compositor*       cwindow_get_compositor          (void);

/*  Gets the xdg window manager base managed internally.
    @returns A pointer to the xdg window manager object.
*/
struct xdg_wm_base*         cwindow_get_xdg_wm_base         (void);

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // CORE_WINDOW_LINUX_H