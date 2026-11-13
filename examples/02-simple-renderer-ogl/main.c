#include <core.h>

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

void close_file(char* data, uint32_t size) { munmap(data, size); }

#include "opengl.c"

int32_t opengl_load_scene(void) {

    vec3 vertices[] = {
        {-3.0f, -3.0f, 0.0f},
        {3.0f, -3.0f, 0.0f},
        {0.0f,  3.0f, 0.0f}
    };  

    write_to_ring_buffer(vertices, sizeof(vertices), NULL, 0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return 0;

}

int32_t opengl_draw_scene(void) {

    glUseProgram(default_program);
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0 /*last_buffer_offset * VERTEX_BUFFER_BLOCK_SIZE*/, 3);

    return 0;

}

int32_t opengl_draw(cwindow_t handle) {

    cgraf_draw_start(NULL, handle);

        glViewport(0, 0, 1920, 1080);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (opengl_draw_scene() < 0)
            return -1;

    cgraf_draw_end(NULL, handle);

    ring_buffer_fences[current_buffer_offset] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    return 0;

}

int32_t main(int32_t argc, const char** argv) {

    err_t err = CORE_SUCCESS;
    ivec2 window_size = {1920, 1080};
    cwindow_t handle = CORE_CWINDOW_INVALID;
    ccatch(&err, handle = cgraf_opengl_hook(&err, cwindow_create(&err, "simple-renderer", window_size)), 
        return 1;
    );

    if (opengl_initialize_buffers() < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    if (opengl_load_shaders() < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    if (opengl_initialize_arrays() < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    if (opengl_load_scene() < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    while (!cwindow_get_closed(NULL, handle)) {

        if (cinput_get_button(NULL, CORE_KEYCODE_ESC))
            cwindow_close(NULL, handle);
        ccatch(&err, cwindow_update(&err), break; {});
        if (opengl_draw(handle) < 0)
            cwindow_close(NULL, handle);

    }

    ccatch(&err, cwindow_destroy(&err, handle), return 1;);
    return 0;

}