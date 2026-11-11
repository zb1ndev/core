#include <window/linux/window-linux.h>

internal struct wl_shm* shm;

internal void smh_format(void *data, struct wl_shm *wl_shm, uint32_t format) {

    (void)data;
    (void)wl_shm;
    (void)format;

}

internal struct wl_shm_listener shm_listener = {
    smh_format
};