#include <gapi/linux/gl_linux_state.c>

void cgapi_opengl_hook(err_t* err, cwindow_t handle) {

    if (!gl_initialized) 
        catch(err, cgapi_opengl_initialize(err), return;); 
    
    cwindow_linux_t* window = cwindow_get_data(err, handle);



}

void cgapi_opengl_cleanup(err_t* err) {

    if (!gl_initialized) return;

}