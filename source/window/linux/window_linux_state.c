#include <window/linux/window_linux.h>

internal cwindow_linux_t    windows[CORE_CWINDOW_MAX]       = {0};
internal cwindow_t          free_windows[CORE_CWINDOW_MAX]  = {0};
internal cwindow_t          window_count                    = 0;
internal bool               window_manager_initialized      = false;

internal struct wl_display*     display         = NULL;
internal struct wl_registry*    registry        = NULL;
internal struct wl_compositor*  compositor      = NULL;
internal struct xdg_wm_base*    xdg_wm_base     = NULL;

#include <window/linux/window_linux_internal.c>

internal void cwindow_manager_initialize(err_t* err) {
    
    for (cwindow_t w = 0; w < CORE_CWINDOW_MAX; w++)
        free_windows[w] = (CORE_CWINDOW_MAX - 1) - w;

    display = wl_display_connect(NULL);
    if (display == NULL) {
        throw(err, CORE_DISPCONREF);
        return;
    }

    registry = wl_display_get_registry(display);
    if (registry == NULL) {
        throw(err, CORE_DIPREG);
        return;
    }

    if (wl_registry_add_listener(registry, &registry_listener, NULL) < 0) {
        throw(err, CORE_REGADDLIS);
        return;
    }

    wl_display_roundtrip(display);
    wl_display_roundtrip(display);

    if (compositor == NULL || xdg_wm_base == NULL) {
        throw(err, CORE_PINVALID);
        return;
    }

    window_manager_initialized = true;

}

internal void cwindow_manager_deinitialize(err_t* err) {

    catch(err, cgapi_deinitialize(err), 
        cdbg_warnf("%d: failed to deinitialize gapi", err!=NULL?*err:-1);
    );

    xdg_wm_base_destroy(xdg_wm_base);
    wl_compositor_destroy(compositor);
    wl_registry_destroy(registry);
    wl_display_disconnect(display);

    window_manager_initialized = false;

}
