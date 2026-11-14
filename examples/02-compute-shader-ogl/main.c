#include <core.h>

#include <sys/mman.h>
#include <sys/stat.h>

#define COMPUTE_SHADER_PATH                         "./examples/02-compute-shader-ogl/renderer.glsl"

#define WINDOW_WIDTH                                (1920U)
#define WINDOW_HEIGHT                               (1080U)

#define RENDER_TEXTURE_WIDTH                        (1280U)
#define RENDER_TEXTURE_HEIGHT                       (720U)
#define WORKGROUP_SIZE                              (8U) // @note must match local_size_x/y in fast-rt.comp

internal GLuint render_texture      = 0;
internal GLuint blit_fbo            = 0;
internal GLuint raytrace_program    = 0;

char* read_file(const char* file_path, uint32_t* size_out) {

    FILE* file = fopen(file_path, "rb");
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
    fclose(file);

    if (result == MAP_FAILED) {
        cdbg_err("failed to read shader file");
        return NULL;
    }

    return result;

}

void close_file(char* data, uint32_t size) { munmap(data, size); }

int32_t opengl_initialize_render_target(void) {

    glCreateTextures(GL_TEXTURE_2D, 1, &render_texture);
    glTextureStorage2D(render_texture, 1, GL_RGBA16F, RENDER_TEXTURE_WIDTH, RENDER_TEXTURE_HEIGHT);
    glBindImageTexture(0, render_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

    glCreateFramebuffers(1, &blit_fbo);
    glNamedFramebufferTexture(blit_fbo, GL_COLOR_ATTACHMENT0, render_texture, 0);

    if (glCheckNamedFramebufferStatus(blit_fbo, GL_READ_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cdbg_err("blit framebuffer is incomplete");
        return -1;
    }

    cdbg_log("allocated render texture");
    return 0;

}

int32_t opengl_load_shader(void) {

    int32_t success     = 0;
    char info_log[1024] = {0};

    uint32_t cs_size = 0;
    const char* cs_data = read_file(COMPUTE_SHADER_PATH, &cs_size);
    if (cs_data == NULL) {
        cdbg_err("failed to read compute shader file");
        return -1;
    }

    GLint cs_length = (GLint)cs_size;   // @note file is not null-terminated, so pass the length
    GLuint compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute_shader, 1, &cs_data, &cs_length);
    close_file((char*)cs_data, cs_size);

    glCompileShader(compute_shader);
    glGetShaderiv(compute_shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(compute_shader, sizeof(info_log), NULL, info_log);
        cdbg_errf("%s", info_log);
        glDeleteShader(compute_shader);
        return -1;
    }

    raytrace_program = glCreateProgram();
    glAttachShader(raytrace_program, compute_shader);
    glLinkProgram(raytrace_program);
    glGetProgramiv(raytrace_program, GL_LINK_STATUS, &success);
    glDeleteShader(compute_shader);

    if (success != GL_TRUE) {
        glGetProgramInfoLog(raytrace_program, sizeof(info_log), NULL, info_log);
        cdbg_errf("%s", info_log);
        return -1;
    }

    cdbg_logf("raytrace_program: %u", raytrace_program);
    return 0;

}

void opengl_draw(cwindow_t handle) {

    cgraf_draw_start(NULL, handle);

        GLint groups_x = (RENDER_TEXTURE_WIDTH  + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
        GLint groups_y = (RENDER_TEXTURE_HEIGHT + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;

        glUseProgram(raytrace_program);
        glDispatchCompute(groups_x, groups_y, 1);
        glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);

        glBlitNamedFramebuffer(blit_fbo, 0,
            0, 0, RENDER_TEXTURE_WIDTH, RENDER_TEXTURE_HEIGHT,
            0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
            GL_COLOR_BUFFER_BIT, GL_LINEAR
        );

    cgraf_draw_end(NULL, handle);

}

int32_t main(int32_t argc, const char** argv) {

    err_t err = CORE_SUCCESS;
    cwindow_t handle = CORE_CWINDOW_INVALID;
    ccatch(&err, handle = cgraf_opengl_hook(&err, cwindow_create(&err, "simple-renderer", (ivec2){ WINDOW_WIDTH, WINDOW_HEIGHT })), 
        return 1;
    );

    if (opengl_initialize_render_target() < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    if (opengl_load_shader() < 0) {
        ccatch(&err, cwindow_destroy(&err, handle), {});
        return 1;
    }

    while (!cwindow_get_closed(NULL, handle)) {

        if (cinput_get_key(NULL, CORE_KEYCODE_ESC))
            cwindow_close(NULL, handle);
        ccatch(&err, cwindow_update(&err), break; );
        opengl_draw(handle);

    }

    ccatch(&err, cwindow_destroy(&err, handle), return 1;);
    return 0;

}