#include <core.h> // redundant, but vscode is stupid

#define MAX_VERTEX_COUNT                (400000U)
#define VERTEX_SIZE                     (32U)
#define INDEX_SIZE                      (4U)
#define RING_BUFFER_BLOCK_COUNT         (3U)

#define VERTEX_SHADER_PATH              "./examples/02-simple-renderer-ogl/shaders/vertex.glsl"
#define FRAGMENT_SHADER_PATH            "./examples/02-simple-renderer-ogl/shaders/fragment.glsl"
#define COMPUTE_SHADER_PATH             "./examples/02-simple-renderer-ogl/shaders/fast-rt.comp"

// @note Weird Raytracing Implementation ( "simple-renderer" ).
// A raytracing-ish renderer with persistent mapping and fences for mesh data streaming
// and maybe try implementing a 360 depth pass cube map that can be used for bounces that dont
// result in hidden geometry or translusence. It can be used always for determining which pixels
// to cast from and where the first ray will land. We will have to create normal textures and a
// ray traversal grid, but for now we can just stick to the fast bounce method. We will generate 
// the cube map at 512x512, render at 720p and upscale to 1080p with nearest neighbor. 

// @note Still cant decide whether we should actually upscale or not. Also the depth pass should
// be hevily optimized to save as much as possible as raytracing on an iGPU is going to be rough.
// My processor is a 12-core AMD Ryzen 5 7640U with Radeon 760M Graphics and 16GB of RAM. Target is 
// 30 frames per second in a static demo scene.

internal GLuint vertex_buffer_object    = 0;
internal GLuint index_buffer_object     = 0;

// @note these are ring buffers
internal void*  vertex_buffer           = NULL; // @note glMapBuffer with GL_ARRAY_BUFFER
internal void*  index_buffer            = NULL; // @note glMapBuffer with GL_ELEMENT_ARRAY_BUFFER

#define VERTEX_BUFFER_BLOCK_SIZE        ALIGN(VERTEX_SIZE       * MAX_VERTEX_COUNT, 64)
#define INDEX_BUFFER_BLOCK_SIZE         ALIGN((VERTEX_SIZE / 4) * MAX_VERTEX_COUNT, 64)

internal uint8_t current_buffer_offset                         = 0;
internal GLsync ring_buffer_fences[RING_BUFFER_BLOCK_COUNT]    = { NULL }; 

int32_t write_to_ring_buffer(void* vdata, uint64_t vsize, void* idata, uint64_t isize) {

    if (vdata == NULL || idata == NULL) {
        cdbg_err("invalid parameters");
        return -1;
    }

    // @note wait until gpu is finished with the data we gave it last
    GLsync current_fence = ring_buffer_fences[current_buffer_offset]; 

    if (current_fence != NULL) {

        GLenum result = glClientWaitSync(current_fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        if (result == GL_WAIT_FAILED) {
            cdbg_err("failed waiting for ring buffer fence");
            return -1;
        }

        glDeleteSync(current_fence);
        ring_buffer_fences[current_buffer_offset] = NULL;

    }

    if (vsize > VERTEX_BUFFER_BLOCK_SIZE) {
        cdbg_warn("vertex buffer overflow");
        cdbg_logf("vsize: %lu, target: %lu", vsize, VERTEX_BUFFER_BLOCK_SIZE);
        vsize = VERTEX_BUFFER_BLOCK_SIZE;
    }

    if (isize > INDEX_BUFFER_BLOCK_SIZE) {
        cdbg_warn("index buffer overflow");
        cdbg_logf("isize: %lu, target: %lu", isize, INDEX_BUFFER_BLOCK_SIZE);
        isize = INDEX_BUFFER_BLOCK_SIZE;
    }

    uint64_t vertex_offset = current_buffer_offset * VERTEX_BUFFER_BLOCK_SIZE;
    uint64_t index_offset  = current_buffer_offset * INDEX_BUFFER_BLOCK_SIZE;

    memcpy(vertex_buffer + vertex_offset, vdata, vsize);
    memcpy(index_buffer + index_offset, idata, isize);
    
    glFlushMappedBufferRange(GL_ARRAY_BUFFER, vertex_offset, vsize);
    glFlushMappedBufferRange(GL_ELEMENT_ARRAY_BUFFER, index_offset, isize);

    current_buffer_offset = (current_buffer_offset + 1) % RING_BUFFER_BLOCK_COUNT;
    return 0;

}

internal GLuint depth_cube_map          = 0;

#define DEPTH_CUBE_MAP_WIDTH            (512U) // @note this should be large enough to get us close, if not we can adjust it
#define DEPTH_CUBE_MAP_HEIGHT           (512U)
#define DEPTH_CUBE_MIP_LEVELS           (1U)

internal GLuint render_texture          = 0;

#define RENDER_TEXTURE_HEIGHT           (720U)
#define RENDER_TEXTURE_WIDTH            (1280U)

internal GLuint vertex_shader           = 0;
internal GLuint fragment_shader         = 0;
internal GLuint compute_shader          = 0;

internal GLuint default_program         = 0;
internal GLuint raytrace_program        = 0;

int32_t opengl_create_buffers(void) {

    GLbitfield storage_flags        = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;
    GLbitfield mapping_flags        = GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_PERSISTENT_BIT;
    
    GLsizeiptr vertex_array_size    = VERTEX_BUFFER_BLOCK_SIZE * RING_BUFFER_BLOCK_COUNT;
    GLsizeiptr index_array_size     = INDEX_BUFFER_BLOCK_SIZE  * RING_BUFFER_BLOCK_COUNT;

    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferStorage(GL_ARRAY_BUFFER, vertex_array_size, NULL, storage_flags);
    if ((vertex_buffer = glMapBufferRange(GL_ARRAY_BUFFER, 0, vertex_array_size, mapping_flags)) == NULL) {
        cdbg_err("glMapBufferRange on vertex buffer failed");
        return -1;
    }

    glGenBuffers(1, &index_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
    glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, index_array_size, NULL, storage_flags);
    if ((index_buffer = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, index_array_size, mapping_flags)) == NULL) {
        cdbg_errf("glMapBufferRange on index buffer failed: %d", glGetError());
        return -1;
    }

    cdbg_log("allocated and mapped vertex and index buffers");

    glGenTextures(1, &depth_cube_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cube_map);

    glTexStorage2D(
        GL_TEXTURE_CUBE_MAP, 
        DEPTH_CUBE_MIP_LEVELS, 
        GL_DEPTH_COMPONENT32F, 
        DEPTH_CUBE_MAP_WIDTH, 
        DEPTH_CUBE_MAP_HEIGHT
    );

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    cdbg_log("allocated depth cube map");

    glGenTextures(1, &render_texture);
    glBindTexture(GL_TEXTURE_2D, render_texture);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, RENDER_TEXTURE_WIDTH, RENDER_TEXTURE_HEIGHT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    cdbg_log("allocated render texture");

    return 0;

}

int32_t opengl_load_shaders(void) {
    
    int32_t success     = 0;
    char info_log[1024] = {0};

    uint32_t vs_size, fs_size, cs_size;
    const char* vs_data = read_file(VERTEX_SHADER_PATH, &vs_size);
    const char* fs_data = read_file(FRAGMENT_SHADER_PATH, &fs_size);
    const char* cs_data = read_file(COMPUTE_SHADER_PATH, &cs_size);

    if (vs_data == NULL || fs_data == NULL || cs_data == NULL) {
        cdbg_err("failed to read shader files");
        return -1;
    }

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_data, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    
    if (success != GL_TRUE) {
        glGetShaderInfoLog(vertex_shader, 1024, NULL, info_log);
        cdbg_errf("%s", info_log);
        return -1;
    }

    cdbg_log("compiled vertex shader");
    close_file((char*)vs_data, vs_size);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_data, NULL); 
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        glGetShaderInfoLog(fragment_shader, 1024, NULL, info_log);
        cdbg_errf("%s", info_log);
        return -1;
    }

    cdbg_log("compiled fragment shader");
    close_file((char*)fs_data, fs_size);

    compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute_shader, 1, &cs_data, NULL);
    glCompileShader(compute_shader);
    glGetShaderiv(compute_shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        glGetShaderInfoLog(compute_shader, 1024, NULL, info_log);
        cdbg_errf("%s", info_log);
        return -1;
    }

    cdbg_log("compiled compute shader");
    close_file((char*)cs_data, cs_size);

    // @note doesnt work, may need to do something different
    default_program = glCreateProgram();
    glAttachShader(default_program, vertex_shader);
    glAttachShader(default_program, fragment_shader);
    glLinkProgram(default_program);
    glGetProgramiv(default_program, GL_LINK_STATUS, &success);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        cdbg_errf("OpenGL error before program creation: 0x%X", err);
    }

    if (success != GL_TRUE) {
        glGetProgramInfoLog(default_program, 1024, NULL, info_log);
        cdbg_errf("%s", info_log);
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    cdbg_logf("default_program: %lu", default_program);

    raytrace_program = glCreateProgram();
    glAttachShader(raytrace_program, compute_shader);
    glLinkProgram(raytrace_program);
    glGetProgramiv(raytrace_program, GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        glGetProgramInfoLog(raytrace_program, 1024, NULL, info_log);
        cdbg_errf("%s", info_log);
        return -1;
    }

    glDeleteShader(compute_shader);
    cdbg_logf("raytrace_program: %lu", raytrace_program);
    
    cdbg_log("shaders loaded");
    return 0;

}