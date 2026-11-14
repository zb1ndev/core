#version 460 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba16f, binding = 0) uniform writeonly image2D image_output;

void main() {

    ivec2 texel_coord = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size        = imageSize(image_output);

    if (any(greaterThanEqual(texel_coord, size)))
        return;

    vec2 uv = vec2(texel_coord) / vec2(size);
    imageStore(image_output, texel_coord, vec4(uv, 0.0, 1.0));

}