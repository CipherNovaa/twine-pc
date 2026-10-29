#ifndef GPU_HW_H
#define GPU_HW_H

/* Hardware renderer (gpu_hw.c): gpu.c's primitives drawn again by the PC's GPU, at a multiple of VRAM's
 * resolution. gpu.c calls these next to its own rasterization (and before it changes VRAM) while gpu_hw_on;
 * video.c shows the result. */
extern int gpu_hw_on;

/* A primitive as gpu.c draws it, with the drawing environment it is drawn in */
typedef struct {
    int textured, raw, semi, gouraud, dither;
    u32 tpage; /* texpage bits 0-8 */
    u32 clut_x, clut_y;
    u32 tw_mask_x, tw_mask_y, tw_off_x, tw_off_y;
    s32 area_x1, area_y1, area_x2, area_y2;
    int mask_set, mask_check;
} GpuHwPrim;

typedef struct {
    s32 x, y;    /* VRAM pixels (the drawing offset added) */
    s32 r, g, b; /* 0-255 */
    s32 u, v;
} GpuHwVertex;

/* An attribute across a triangle as gpu.c steps it: 16.16 fixed point at the top left of the triangle's box,
 * and its steps per pixel in x and y */
typedef struct {
    s64 at, dx, dy;
} GpuHwPlane;

/* gpu.c: a triangle it rasterizes inside (minx, miny)-(maxx, maxy) (its box within the drawing area), with its
 * planes for r, g, b (gouraud), u, v (textured) */
void gpu_hw_triangle(const GpuHwPrim *p, const GpuHwVertex *v, s32 minx, s32 miny, s32 maxx, s32 maxy,
                     const GpuHwPlane *planes);
/* a rectangle (sprite) of w x h pixels at (x, y), its texture from (u, v) on, backwards when flipped */
void gpu_hw_rect(const GpuHwPrim *p, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b, s32 u, s32 v, int flip_x,
                 int flip_y);
/* a line from (x0, y0) to (x1, y1): its pixels follow, as gpu.c plots them */
void gpu_hw_line(const GpuHwPrim *p, s32 x0, s32 y0, s32 x1, s32 y1);
void gpu_hw_line_pixel(s32 x, s32 y, s32 r, s32 g, s32 b);
/* VRAM changed outside of drawing: a fill (8-bit colour), an image from the CPU, a copy within VRAM */
void gpu_hw_fill(s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b);
void gpu_hw_load_image(s32 x, s32 y, s32 w, s32 h, const u16 *src);
void gpu_hw_move_image(s32 sx, s32 sy, s32 w, s32 h, s32 dx, s32 dy);
void gpu_hw_reset(void);

/* video.c */
int gpu_hw_wanted(void);       /* TWINE_RENDERER=hw, or nothing asked with a window */
int gpu_hw_asked(void);        /* TWINE_RENDERER=hw */
int gpu_hw_init(void);         /* 1 when the GPU is ready (gpu_hw_on is then 1) */
void *gpu_hw_device(void);     /* its SDL_GPUDevice */
int gpu_hw_scale(void);
/* a displayed frame (video.c's count): VRAM rectangle (x, y, w, h) (w 0: a 24-bit frame, not the renderer's);
 * compares, logs, writes `shot` (a PPM path, or NULL) */
void gpu_hw_frame(u32 frame, s32 x, s32 y, s32 w, s32 h, const char *shot);
void gpu_hw_shot_rect(s32 x, s32 y, s32 w, s32 h, const char *path);
/* the rectangle into `dst` (an SDL_GPUTexture, R8G8B8A8, dst_w x dst_h), area-averaged */
void gpu_hw_picture(void *dst, int dst_w, int dst_h, s32 x, s32 y, s32 w, s32 h);

#endif
