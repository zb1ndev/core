#include <core.h>

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <sys/mman.h>
#include <sys/stat.h>

char* read_file(const char* file_path, uint32_t* size_out) {

    FILE* file = fopen(file_path, "rb+");
    if (file == NULL) {
        cdbg_errf("failed to open %s", file_path);
        return NULL;
    }

    struct stat fd_stat;
    int fd = fileno(file);
    if (fstat(fd, &fd_stat) < 0) {
        cdbg_errf("failed to get file size %s", file_path);
        fclose(file);
        return NULL;
    }

    *size_out = fd_stat.st_size;
    char* result = mmap(NULL, fd_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (result == MAP_FAILED) {
        cdbg_err("failed to read shader file");
        return NULL;
    }

    fclose(file);
    return result;

}

void close_file(char* data, uint32_t size) {

    munmap(data, size);

}

#include "opengl.c"

internal void tiny_obj_get_file_data(void* ctx, const char* file_name, const int is_mtl, const char* obj_file_name, char** data, size_t* len) {

    (void)ctx;

    if (file_name == NULL) {
        cdbg_err("file_name was NULL");
        return;
    }

    uint32_t data_size = 0;
    *data = read_file(file_name, &data_size);
    *len = (size_t)data_size;
    
}

// @note we will need textures with object space normals
int32_t opengl_load_scene(int32_t argc, const char** argv) {

    

    return 0;

}

int32_t opengl_draw(cwindow_t handle) {

    cgraf_draw_start(NULL, handle);

        // @todo do stuff here, currently crashing because of something opengl related.
        // it is something shader related.

        glViewport(0, 0, 1920, 1080);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cgraf_draw_end(NULL, handle);

    ring_buffer_fences[current_buffer_offset] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    return 0;

}

int32_t main(int32_t argc, const char** argv) {

    if (argc < 4 || argv == NULL) {
        cdbg_err("invalid arguments, usage: <model as .obj> <material as .mtl> <object-space normals as .png>");
        return 1;
    }

    cdbg_logf("rendering the following model, material, and normals\n\tmodel: %s\n\tmaterial: %s\n\tnormal: %s\n",
        argv[1], argv[2], argv[3]
    );

    err_t err = CORE_SUCCESS;
    ivec2 window_size = {1920, 1080};
    cwindow_t handle = CORE_CWINDOW_INVALID;
    ccatch(&err, handle = cgraf_opengl_hook(&err, cwindow_create(&err, "simple-renderer", window_size)), 
        return 1;
    );

    if (opengl_create_buffers() < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    if (opengl_load_shaders() < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    if (opengl_load_scene(argc, argv) < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    bool lock_state = false;
    while (!cwindow_get_closed(NULL, handle)) {

        if (cinput_get_button(NULL, CORE_KEYCODE_C)) {
            lock_state = !lock_state;
            cwindow_set_cursor(NULL,  handle, false, lock_state);
        }

        vec2 axis = {0};
        cinput_get_mouse_axis(NULL, &axis);
        cdbg_logf("axis: %f:%f", axis[0], axis[1]);

        if (cinput_get_button(NULL, CORE_KEYCODE_ESC))
            cwindow_close(NULL, handle);
        ccatch(&err, cwindow_update(&err), break; {});
        if (opengl_draw(handle) < 0)
            cwindow_close(NULL, handle);

    }

    ccatch(&err, cwindow_destroy(&err, handle), return 1;);
    return 0;

}