# core
core is a multi-platform windowing abstraction that makes it easy to hook up graphics APIs. core does not do any pipeline configuration and leaves all performance critical areas in the users control. core also provides input, cglm, and graphics API bindings via glad. Error handling is optional, and stdout logs are disabled in release. To build in release just add the following to the cflags variable in `linux-build.sh` ...
```bash
cflags="-DCORE_RELEASE"
```

# usage
The following code opens a window, initializes OpenGL, and binds it to the window surface ...

```cpp
err_t err = CORE_SUCCESS;
cwindow_t window = cwindow_create(&err, "Hello World", (ivec2){1920, 1080}); 
if (window == CORE_CWINDOW_INVALID) {
    throw(&err, CORE_WINNREADY);
    return 1;
}
catch(&err, cgraf_opengl_hook(&err, window), return 1; );
```

core provides the error handling macros `throw` and `catch` to make the use of the optional error system easier. Next is the event loop, as you can see `NULL` has been passed to the error parameter meaning that we are choosing not to handle errors right now ...

```c
while (!cwindow_get_closed(NULL, window)) {

    if (cinput_get_button(NULL, CORE_KEYCODE_ESC))
        cwindow_close(NULL, window);
    cwindow_update(NULL);

    cgraf_draw_start(NULL, window);

        // @note do your OpenGL/Vulkan draw calls in here

    cgraf_draw_end(NULL, window);

}
```

The following code will deinitialize window and any context bound to it.

```c
cwindow_destroy(&err, window);
```