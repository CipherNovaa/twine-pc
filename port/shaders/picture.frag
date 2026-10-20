#version 450
/* The displayed rectangle of the hardware renderer's target as a picture (gpu_hw.c): each output pixel is the
 * average of the target pixels its area covers (a box filter: supersampling when the target is larger than
 * the output; a plain copy at the same size). A 15-bit-mode target's values (c * 2080 of 65535, hw.frag) come
 * out as the software display shows VRAM (5 bits a channel shifted up by 3). */

layout(location = 0) out vec4 o_color;

layout(set = 2, binding = 0) uniform sampler2D target;

/* src: the rectangle in target pixels (x, y, w, h); size: the output's w, h, then 1 for a 15-bit target */
layout(set = 3, binding = 0) uniform Params {
    vec4 src;
    vec4 size;
};

vec3 texel(int x, int y) {
    vec3 c = texelFetch(target, ivec2(x, y), 0).rgb;
    return size.z != 0.0 ? floor((floor(c * 65535.0 + 0.5) + 4.0) / 2080.0) * 8.0 / 255.0 : c;
}

void main() {
    vec2 o = floor(gl_FragCoord.xy);
    vec2 r = src.zw / size.xy;
    vec2 f0 = src.xy + o * r, f1 = f0 + r;
    vec3 sum = vec3(0.0);
    float total = 0.0;
    for (int y = int(floor(f0.y)); float(y) < f1.y; y++) {
        float wy = min(f1.y, float(y + 1)) - max(f0.y, float(y));
        for (int x = int(floor(f0.x)); float(x) < f1.x; x++) {
            float w = (min(f1.x, float(x + 1)) - max(f0.x, float(x))) * wy;
            sum += texel(x, y) * w;
            total += w;
        }
    }
    o_color = vec4(sum / max(total, 1e-6), 1.0);
}
