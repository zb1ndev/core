#include <core.h>

int32_t main(int32_t argc, const char** argv) {

    err_t err = CORE_SUCCESS;
    cwindow_t window = cgapi_opengl_hook(&err, cwindow_create(NULL, "Hello World", (cvec2u_t){1920, 1080}));
    if (window == CORE_CWINDOW_INVALID) {
        return 1;
    }

    cgapi_opengl_test(window);
    cwindow_destroy(&err, window);
    return 0;

}
