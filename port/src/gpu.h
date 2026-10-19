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
void gpu_load_image(s32 x, s32 y, s32 w, s32 h, const u16 *src);
void gpu_store_image(s32 x, s32 y, s32 w, s32 h, u16 *dst);
void gpu_move_image(s32 sx, s32 sy, s32 w, s32 h, s32 dx, s32 dy);
void gpu_fill_rect(s32 x, s32 y, s32 w, s32 h, u16 c);
void gpu_set_draw_area(s32 x1, s32 y1, s32 x2, s32 y2);
void gpu_set_draw_offset(s32 x, s32 y);
void gpu_set_texpage(u32 tp, int dither, int draw_on_display);
void gpu_set_texwindow(u32 mx, u32 my, u32 ox, u32 oy);

#endif
