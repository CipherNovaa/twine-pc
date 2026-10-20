#ifndef GPU_H
#define GPU_H

/* Software PS1 GPU (gpu.c) */
extern u16 gpu_vram[512 * 1024];

typedef struct {
    s32 x, y, w, h;   /* displayed VRAM rectangle */
    int rgb24;        /* 24-bit display mode (movies) */
    int blank;        /* SetDispMask(0) */
} GpuDisplay;
extern GpuDisplay gpu_display;

void gpu_gp0(const u32 *words, int n);
void gpu_reset(void);
void gpu_load_image(s32 x, s32 y, s32 w, s32 h, const u16 *src);
void gpu_store_image(s32 x, s32 y, s32 w, s32 h, u16 *dst);
void gpu_move_image(s32 sx, s32 sy, s32 w, s32 h, s32 dx, s32 dy);
void gpu_fill_rect(s32 x, s32 y, s32 w, s32 h, u16 c);
/* VRAM there is about to be read: primitives kept for later (with the hardware renderer) are rasterized; the
 * blocks are 64x16 pixels, bit c of row r for the one at (c * 64, r * 16) */
void gpu_sync(s32 x, s32 y, s32 w, s32 h);
void gpu_sync_blocks(const u16 *rows);
/* primitives are kept for later (with the hardware renderer, TWINE_SW_DEFER) */
int gpu_deferring(void);
void gpu_set_draw_area(s32 x1, s32 y1, s32 x2, s32 y2);
void gpu_set_draw_offset(s32 x, s32 y);
void gpu_set_texpage(u32 tp, int dither, int draw_on_display);
void gpu_set_texwindow(u32 mx, u32 my, u32 ox, u32 oy);
void gpu_save(void);
void gpu_restore(void);
void gpu_draw_origin(s32 *x, s32 *y);

#endif
