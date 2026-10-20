#version 450
/* The hardware renderer's pixel pipeline (gpu_hw.c), written from gpu.c's plot(): drawing area, texture
 * window, 4/8/15-bit texels with their CLUT from the native VRAM, colour 0 transparent, modulation, dithering,
 * the mask bit (alpha). Semi-transparency itself is the pipeline's blending; this only quarters the source for
 * mode 3.
 *
 * 15-bit mode (truecolor 0): the target has 16 bits a channel and holds a 5-bit value c as c * 2080 (of 65535),
 * so sums are exact and halving keeps the fraction (mode 0); whoever reads the target takes the whole part
 * ((value + 4) / 2080: picture.frag, gpu_hw.c's readback; the 4 covers the blend constant's rounding, a half
 * that is a hair under one), which is what the PS1's truncation after each blend gives:
 * floor((floor(x) + f) / 2) = floor((x + f) / 2). A saturated 65535 is 31.5: it reads as 31 and halves as 31
 * would. At scale 1 the colours and texture coordinates come from gpu.c's own fixed-point planes, so the result
 * is gpu.c's pixel for pixel. Truecolor mode: 8 bits a channel, nothing truncated or dithered. */

const float LEVEL = 2080.0 / 65535.0; /* one step of a 5-bit channel in 15-bit mode */

layout(location = 0) in vec2 v_uv;
layout(location = 1) noperspective in vec4 v_color;
layout(location = 2) flat in uvec4 v_mode;
layout(location = 3) flat in uint v_area;
layout(location = 4) flat in uint v_plane;

layout(location = 0) out vec4 o_color;

layout(set = 2, binding = 0) uniform usampler2D vram;

/* Exact mode (scale 1, 15-bit colour): a triangle's colour and texture coordinates as gpu.c steps them, 16.16
 * fixed point: r, g, b, u, v each at the top left of its box, step in x, step in y; then the box's top left. In
 * 32-bit arithmetic that wraps around, as gpu.c's does. */
layout(std430, set = 2, binding = 1) readonly buffer Planes {
    ivec4 planes[];
};

/* scale, truecolor, the batch's first plane */
layout(set = 3, binding = 0) uniform Params {
    ivec4 params;
};

/* v_mode.x: the texpage bits (0-8: x, y, semi-transparency mode, colour depth) and these */
const uint DITHER = 1u << 9, TEXTURED = 1u << 10, RAW = 1u << 11, GOURAUD = 1u << 12, MASK_SET = 1u << 15,
           BLIT = 1u << 16, BLEND = 1u << 17, HALVE = 1u << 18;
/* bits 13-14: which texels: all, those without bit 15 (a semi-transparent primitive's opaque ones), those with */
const uint TEXELS_SHIFT = 13u;

/* a little past float error, so a value that is a whole number stays one */
const float EPS_UV = 1.0 / 512.0, EPS_COLOR = 1.0 / 512.0;

const int dither_table[16] = int[16](-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2);

uint vram_at(int x, int y) {
    return texelFetch(vram, ivec2(x & 1023, y & 511), 0).r;
}

void main() {
    int scale = params.x;
    bool truecolor = params.y != 0;
    ivec2 hp = ivec2(gl_FragCoord.xy); /* the target's pixel */
    ivec2 p = hp / scale;               /* the VRAM pixel it is part of */
    uint mode = v_mode.x;
    uint mask = (mode & MASK_SET) != 0u ? 1u : 0u;
    ivec2 a0 = ivec2(v_mode.w & 0xFFFFu, v_mode.w >> 16), a1 = ivec2(v_area & 0xFFFFu, v_area >> 16);
    if (p.x < a0.x || p.y < a0.y || p.x > a1.x || p.y > a1.y)
        discard;

    /* an image the CPU put into VRAM: its pixels as they are */
    if ((mode & BLIT) != 0u) {
        uint t = vram_at(p.x, p.y);
        vec3 c5 = vec3(t & 31u, (t >> 5) & 31u, (t >> 10) & 31u);
        o_color = vec4(truecolor ? c5 * 8.0 / 255.0 : c5 * LEVEL, float(t >> 15));
        return;
    }

    /* the colour (0x80 is neutral for textures) and texture coordinates here: interpolated, or at scale 1 in
     * 15-bit colour gpu.c's (its planes) */
    vec3 col = v_color.rgb * 255.0;
    col = (mode & GOURAUD) != 0u ? floor(col + EPS_COLOR) : floor(col + 0.5);
    ivec3 c = ivec3(col);
    vec3 cf = truecolor && (mode & GOURAUD) != 0u ? v_color.rgb * 255.0 : col;
    ivec2 uv = ivec2(floor(v_uv + EPS_UV));
    if (v_plane != 0xFFFFFFFFu && scale == 1 && !truecolor) {
        int i = (params.z + int(v_plane)) * 4;
        ivec4 q0 = planes[i], q1 = planes[i + 1], q2 = planes[i + 2], q3 = planes[i + 3];
        int k = p.x - (q3.w & 0xFFFF), j = p.y - (q3.w >> 16);
        if ((mode & GOURAUD) != 0u)
            c = ivec3(q0.x + q0.y * k + q0.z * j, q0.w + q1.x * k + q1.y * j, q1.z + q1.w * k + q2.x * j) >> 16;
        uv = ivec2(q2.y + q2.z * k + q2.w * j, q3.x + q3.y * k + q3.z * j) >> 16;
    }

    bool raw = (mode & RAW) != 0u;
    if ((mode & TEXTURED) != 0u) {
        uint win = v_mode.z;
        uint u = uint(uv.x), v = uint(uv.y);
        uint mx = win & 31u, my = (win >> 5) & 31u, ox = (win >> 10) & 31u, oy = (win >> 15) & 31u;
        u = ((u & ~(mx * 8u)) | ((ox & mx) * 8u)) & 255u;
        v = ((v & ~(my * 8u)) | ((oy & my) * 8u)) & 255u;
        int tx = int(mode & 15u) * 64, ty = int((mode >> 4) & 1u) * 256;
        uint depth = (mode >> 7) & 3u;
        int cx = int(v_mode.y & 0xFFFFu), cy = int(v_mode.y >> 16);
        uint t;
        if (depth == 0u) {
            uint w = vram_at(tx + int(u >> 2), ty + int(v));
            t = vram_at(cx + int((w >> ((u & 3u) * 4u)) & 15u), cy);
        } else if (depth == 1u) {
            uint w = vram_at(tx + int(u >> 1), ty + int(v));
            t = vram_at(cx + int((w >> ((u & 1u) * 8u)) & 255u), cy);
        } else {
            t = vram_at(tx + int(u), ty + int(v));
        }
        if (t == 0u)
            discard;
        uint texels = (mode >> TEXELS_SHIFT) & 3u;
        if ((texels == 1u && (t & 0x8000u) != 0u) || (texels == 2u && (t & 0x8000u) == 0u))
            discard;
        mask |= t >> 15;
        ivec3 tc = ivec3(t & 31u, (t >> 5) & 31u, (t >> 10) & 31u);
        if (raw) {
            c = tc * 8;
            cf = vec3(c);
        } else {
            /* modulated in 8-bit precision, so dithering can apply */
            c = tc * 8 * c / 128;
            cf = vec3(tc * 8) * cf / 128.0;
        }
    }

    uint semi_mode = (mode >> 5) & 3u;
    vec3 outc;
    if (truecolor) {
        outc = clamp(cf, 0.0, 255.0) / 255.0;
    } else {
        if ((mode & DITHER) != 0u && !((mode & TEXTURED) != 0u && raw))
            c += dither_table[(hp.y & 3) * 4 + (hp.x & 3)];
        ivec3 c5 = clamp(c, 0, 255) >> 3;
        if ((mode & BLEND) != 0u && semi_mode == 3u)
            c5 >>= 2;
        outc = vec3(c5) * LEVEL;
    }
    if ((mode & BLEND) != 0u && semi_mode == 3u && truecolor)
        outc *= 0.25;
    if ((mode & HALVE) != 0u)
        outc *= 0.5;
    o_color = vec4(outc, float(mask));
}
