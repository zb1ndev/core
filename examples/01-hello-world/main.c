#include <core.h>

int32_t main(int32_t argc, const char** argv) {

    err_t err = CORE_SUCCESS;
    cwindow_t window = cgapi_opengl_hook(&err, cwindow_create(NULL, "Hello World", (cvec2u_t){1920, 1080}));
    if (window == CORE_CWINDOW_INVALID) {
        return 1;
    }

    cvec2u_t window_size = {0};
    cwindow_get_size(NULL, window, &window_size);

    while(cwindow_get_closed(NULL, window) == false) {

        cwindow_update(NULL);
        cgapi_gl_draw_start(NULL, window);

        glViewport(0, 0, window_size[X], window_size[Y]);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cgapi_gl_draw_end(NULL, window);

    }

    cwindow_destroy(&err, window);
    return 0;

}
