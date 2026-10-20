#version 450
/* The hardware renderer's primitives (gpu_hw.c). Positions are in VRAM pixels: the PS1 GPU samples a pixel at
 * its top left corner, and a pixel of the scaled target samples at its own top left corner (pixel q of the
 * target at q / scale VRAM pixels), so at scale 1 every pixel is covered and textured as gpu.c does it, and
 * at a larger scale each VRAM pixel's block of target pixels samples the area that pixel stands for. z orders
 * the primitives (gpu_hw.c), w is 1 for now (perspective comes later). */

layout(location = 0) in vec4 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;
layout(location = 3) in uvec4 a_mode; /* mode bits, CLUT, texture window, drawing area top left */
layout(location = 4) in uint a_area;  /* drawing area bottom right */
layout(location = 5) in uint a_plane; /* exact mode: the triangle's planes (hw.frag) */

layout(location = 0) out vec2 v_uv;
layout(location = 1) noperspective out vec4 v_color;
layout(location = 2) flat out uvec4 v_mode;
layout(location = 3) flat out uint v_area;
layout(location = 4) flat out uint v_plane;

/* VRAM pixels to normalized device coordinates: x * xform.x + xform.z, y * xform.y + xform.w */
layout(set = 1, binding = 0) uniform Xform {
    vec4 xform;
};

void main() {
    gl_Position = vec4(a_pos.x * xform.x + xform.z, a_pos.y * xform.y + xform.w, a_pos.z, 1.0);
    v_uv = a_uv;
    v_color = a_color;
    v_mode = a_mode;
    v_area = a_area;
    v_plane = a_plane;
}
