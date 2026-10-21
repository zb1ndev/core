#include <core.h>

int32_t main(int32_t argc, const char** argv) {

    err_t err = CORE_SUCCESS;
    ivec2 window_size = (ivec2){1920, 1080};
    cwindow_t handle = CORE_CWINDOW_INVALID;
    
    catch(&err, cgraf_opengl_hook(&err, cwindow_create(&err, "simple-renderer", window_size)), 
        return 1;
    );

    while (!cwindow_get_closed(NULL, handle)) {

        if (cinput_get_button(NULL, CORE_KEYCODE_ESC))
            cwindow_close(NULL, handle);

        catch(&err, cwindow_update(&err), 
            break;
        );

        

    }    

    catch(&err, cwindow_destroy(&err, handle), 
        return 1;
    );

    return 0;

}