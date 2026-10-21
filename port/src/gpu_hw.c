/* Hardware renderer: the PC's GPU (SDL_GPU on Vulkan) draws what the software GPU (gpu.c) draws, into a copy
 * of VRAM at TWINE_SCALE times its resolution (1-8), and the display shows that copy. gpu.c stays the real
 * VRAM: textures and CLUTs are read from it (uploaded to the GPU as a 1024x512 16-bit texture, in blocks, when
 * changed), and so are readbacks, movies and every effect that copies VRAM around. Written from the psx-spx
 * documentation and gpu.c; the shaders are port/shaders/hw.* (the pixel pipeline) and picture.* (the display).
 *
 * Each primitive gpu.c rasterizes is recorded here first, as triangles in VRAM coordinates with its texture
 * page, CLUT, texture window, drawing area and mode bits as flat attributes, and drawn later in batches. A
 * batch is drawn when it must be: before VRAM that one of its primitives reads from changes, before a copy
 * within VRAM, and when a frame is shown.
 *
 * Order: every primitive gets the next depth value, and the depth test passes only later ones (depth GREATER,
 * written). So a batch draws all its opaque primitives in one go, in any order, and then the ones that blend
 * with what is under them (semi-transparent, or checking the mask bit) in their own order: where a later
 * opaque primitive covers one of those, the depth test throws it out, as drawing in order would have
 * overwritten it. A semi-transparent textured primitive blends only its texels with bit 15 set: it is drawn
 * twice, its other texels as an opaque primitive and those as a blending one. The depth buffer (16 bits)
 * starts over every 65535 primitives.
 *
 * Colours: TWINE_TRUECOLOR=0 (the default at scale 1) draws 15-bit colour with gpu.c's 8-bit arithmetic and
 * dithering, into a target of 16 bits a channel that holds a 5-bit value c as c * 2080 (of 65535): blending
 * keeps the fractions and readers take the whole part, which comes to the PS1's truncation after every blend
 * (hw.frag). (A 5-bit target won't do: the GPU blends one in about 5 bits, rounding each product, and mode 0's
 * (B + F) / 2 comes out wrong a quarter of the time.) At scale 1 the colours and texture coordinates are
 * gpu.c's own fixed-point planes, and the GPU's fill rule is gpu.c's top-left one, so the picture is gpu.c's
 * pixel for pixel. TWINE_TRUECOLOR=1 (the default above scale 1) draws into 8 bits a channel without dithering
 * or truncation, the colours and texture coordinates interpolated. The alpha channel is the mask bit. Mask
 * checking is the blend factors (1 - destination alpha); mode 0 semi-transparency with mask checking is only
 * approximate (where the mask bit is set it halves what is there; the game never checks the mask).
 *
 * TWINE_HW_COMPARE=1 (scale 1, 15-bit colour) compares each displayed frame with gpu.c's VRAM and logs how much
 * is the same and the worst pixels; frames TWINE_SHOT names are also written as cmp_NNNN.ppm (gpu.c | GPU |
 * difference). TWINE_HW_STATS=1 logs every 100 frames what was drawn and how long it took (the CPU's time in
 * here, and the GPU's: then each frame waits for the GPU). TWINE_HW_PROBE=<frame>[-<frame>],<x>,<y> logs what
 * those frames draw over a VRAM pixel, TWINE_HW_LOG=1 the uploads and batches. TWINE_HW_DEBUG=1 asks for the
 * Vulkan validation layers. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "gpu.h"
#include "gpu_hw.h"
#include "shaders.h"

int gpu_hw_on;

/* mode bits of a vertex (as hw.frag has them); bits 0-8 are the texpage's */
#define M_DITHER (1u << 9)
#define M_TEXTURED (1u << 10)
#define M_RAW (1u << 11)
#define M_GOURAUD (1u << 12)
#define M_OPAQUE_TEXELS (1u << 13) /* only texels without bit 15 */
#define M_SEMI_TEXELS (2u << 13)   /* only those with it */
#define M_MASK_SET (1u << 15)
#define M_BLIT (1u << 16)
#define M_BLEND (1u << 17)
#define M_HALVE (1u << 18) /* mode 0 with mask checking: the source halved (the blend can't) */

typedef struct {
    float x, y, z, w;
    float u, v;
    u8 r, g, b, a;
    u32 mode, clut, window, area0, area1;
    u32 plane; /* the triangle's planes (exact mode), from the batch's first; NO_PLANE: none */
} Vertex;

#define NO_PLANE 0xFFFFFFFFu

/* pipelines: their blending (semi-transparency mode 0-3) and whether they check the mask bit */
enum { P_OPAQUE, P_SEMI = 1, P_CHECK = 5, P_SEMI_CHECK = 6, P_COUNT = 10 };

#define LIST_MAX 65536 /* vertices */
#define RUNS_MAX 8192
#define PLANES_MAX 65536 /* triangles */
#define VBUF_SIZE (16 * 1024 * 1024)
#define PLANE_SIZE 64 /* bytes */
#define PBUF_SIZE (2 * PLANES_MAX * PLANE_SIZE)
#define IMAGES_SIZE (1024 * 512 * 2)
#define UPLOAD_SIZE (2 * LIST_MAX * (u32)sizeof(Vertex) + PLANES_MAX * PLANE_SIZE + 1024 * 512 * 2 + IMAGES_SIZE)

static SDL_GPUDevice *dev;
static int scale = 1, truecolor, exact;
static SDL_GPUTextureFormat target_format;
static SDL_GPUTexture *target, *depth, *vram, *move_tmp;
static u32 move_w, move_h;
static SDL_GPUSampler *nearest;
static SDL_GPUGraphicsPipeline *pipes[P_COUNT], *picture_pipe;
static SDL_GPUBuffer *vbuf, *pbuf;
static u32 vbuf_used, pbuf_used;
static int vbuf_cycle, pbuf_cycle; /* the next upload to it gets a buffer of its own (in use by the GPU) */
static SDL_GPUTransferBuffer *upload, *download;
static u32 download_size;
static SDL_GPUCommandBuffer *cmd;
static int cmd_batches; /* batches in it: it is sent off every so many, even while no frame is shown */
static int clear_color, clear_depth; /* the next render pass clears the target / the depth buffer */

/* the batch: opaque vertices, then runs of vertices drawn in order, each with its pipeline */
static Vertex opaque[LIST_MAX], ordered[LIST_MAX];
static int n_opaque, n_ordered;
static struct {
    int pipe, first, count;
} runs[RUNS_MAX];
static int n_runs;
static u32 order;         /* the last primitive's depth value */
static u32 cur_frame = 1; /* the frame being drawn (video.c's count) */
/* Exact mode (scale 1, 15-bit colour): each triangle's colour and texture coordinate planes as gpu.c steps them
 * (GpuHwPlane, 32 bits of each: r, g, b, u, v at, dx, dy, then the box's top left), so the shader works out
 * each pixel's values as gpu.c does rather than interpolating them. */
static s32 planes[PLANES_MAX][PLANE_SIZE / 4];
static int n_planes;

/* VRAM in blocks of 64x16 pixels (16 columns, 32 rows; a row's columns are the bits of a u16): those gpu.c
 * has changed since they were last uploaded, and those the batch reads */
typedef struct {
    u16 row[32];
} Blocks;
static Blocks dirty, sampled;
/* the images the batch draws, from the game's own pixels (gpu_hw_load_image), uploaded with it */
#define IMAGES_MAX 256
static u8 images[IMAGES_SIZE];
static u32 images_used;
static struct {
    s32 x, y, w, h;
    u32 at;
} image[IMAGES_MAX];
static int n_images;

/* the line being recorded */
static Vertex line_v;
static int line_pipe, line_ordered;

/* TWINE_HW_STATS: this period's counts and times */
static int stats_on, compare_on, log_on;
static struct {
    u32 frames, prims, flushes, draws, upload_bytes;
    u64 cpu_ns, gpu_ns;
} st;
static struct {
    u32 frames, worst_frame;
    double sum, worst;
} cmp;

static u64 now_ns(void) {
    return SDL_GetTicksNS();
}

/* ---- VRAM blocks ---- */

/* the blocks of a VRAM rectangle (coordinates wrap as in VRAM) */
static void blocks_of(Blocks *b, s32 x, s32 y, s32 w, s32 h) {
    u16 cols = 0;
    s32 c, r, c0, c1, r0, r1;
    memset(b, 0, sizeof(*b));
    if (w <= 0 || h <= 0)
        return;
    if (w > 1024)
        w = 1024;
    if (h > 512)
        h = 512;
    c0 = (x & 1023) >> 6;
    c1 = ((x & 1023) + w - 1) >> 6;
    r0 = (y & 511) >> 4;
    r1 = ((y & 511) + h - 1) >> 4;
    for (c = c0; c <= c1; c++)
        cols |= (u16)(1u << (c & 15));
    for (r = r0; r <= r1; r++)
        b->row[r & 31] |= cols;
}

static void blocks_add(Blocks *to, const Blocks *b) {
    int r;
    for (r = 0; r < 32; r++)
        to->row[r] |= b->row[r];
}

static int blocks_meet(const Blocks *a, const Blocks *b) {
    int r;
    for (r = 0; r < 32; r++)
        if (a->row[r] & b->row[r])
            return 1;
    return 0;
}

/* texture coordinates lo-hi as ranges within the page (0-255): one, or two where they wrap around it */
static int page_ranges(s32 lo, s32 hi, u32 window, s32 r[2][2]) {
    if (window || hi - lo >= 255) {
        r[0][0] = 0, r[0][1] = 255;
        return 1;
    }
    lo &= 255, hi &= 255;
    if (lo <= hi) {
        r[0][0] = lo, r[0][1] = hi;
        return 1;
    }
    r[0][0] = lo, r[0][1] = 255;
    r[1][0] = 0, r[1][1] = hi;
    return 2;
}

/* The blocks a textured primitive reads: its texels (u, v from min to max, and one more each way: the steps'
 * rounding reaches past the vertices' values, and -1 is the far side of the page; through the texture window)
 * and its CLUT. */
static void texture_blocks(Blocks *t, const GpuHwPrim *p, s32 umin, s32 umax, s32 vmin, s32 vmax) {
    Blocks c;
    s32 tx = (p->tpage & 15) * 64, ty = ((p->tpage >> 4) & 1) * 256, depth_bits = (p->tpage >> 7) & 3, x0, x1;
    s32 us[2][2], vs[2][2];
    int shift = depth_bits == 0 ? 2 : depth_bits == 1 ? 1 : 0, i, j, nu, nv;
    /* (A page at VRAM's right edge goes on at its left one, where the picture is drawn. While gpu.c draws
     * later, the margin alone doesn't take it there, past the far side of the page or past the last texel the
     * coordinates reach: what the rounding finds there would make gpu.c draw everything, at most a pixel of an
     * edge each frame then shows an older picture's.) */
    s32 ulo = umin - 1, uhi = umax + 1, vlo = vmin - 1, vhi = vmax + 1;
    if (gpu_deferring() && umin == 0 && tx + (255 >> shift) > 1023)
        ulo = 0;
    if (gpu_deferring() && umax < 255 && tx + (uhi >> shift) > 1023 && tx + (umax >> shift) <= 1023)
        uhi = umax;
    nu = page_ranges(ulo, uhi, p->tw_mask_x, us);
    nv = page_ranges(vlo, vhi, p->tw_mask_y, vs);
    memset(t, 0, sizeof(*t));
    for (i = 0; i < nu; i++)
        for (j = 0; j < nv; j++) {
            x0 = tx + (us[i][0] >> shift), x1 = tx + (us[i][1] >> shift);
            /* (while gpu.c draws later, nothing reads round VRAM's right edge into its left one either: what a
             * console finds there is the picture being drawn) */
            if (gpu_deferring() && x0 > 1023)
                continue;
            if (gpu_deferring() && x1 > 1023)
                x1 = 1023;
            blocks_of(&c, x0, ty + vs[j][0], x1 - x0 + 1, vs[j][1] - vs[j][0] + 1);
            blocks_add(t, &c);
        }
    if (depth_bits < 2) {
        blocks_of(&c, p->clut_x, p->clut_y, depth_bits ? 256 : 16, 1);
        blocks_add(t, &c);
    }
}

/* ---- batches ---- */

static void begin_commands(void) {
    if (cmd)
        return;
    cmd = SDL_AcquireGPUCommandBuffer(dev);
    if (!cmd)
        port_fatal("gpu_hw: SDL_AcquireGPUCommandBuffer: %s\n", SDL_GetError());
    vbuf_used = pbuf_used = 0;
    vbuf_cycle = pbuf_cycle = 1;
    cmd_batches = 0;
}

static void submit(int wait);

/* the target and depth buffer as the render pass's */
static SDL_GPURenderPass *begin_target_pass(void) {
    SDL_GPUColorTargetInfo ct;
    SDL_GPUDepthStencilTargetInfo dt;
    SDL_GPURenderPass *pass;
    memset(&ct, 0, sizeof(ct));
    memset(&dt, 0, sizeof(dt));
    ct.texture = target;
    ct.load_op = clear_color ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
    ct.store_op = SDL_GPU_STOREOP_STORE;
    dt.texture = depth;
    dt.load_op = clear_depth ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
    dt.store_op = SDL_GPU_STOREOP_STORE;
    dt.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
    dt.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
    dt.clear_depth = 0;
    clear_color = clear_depth = 0;
    pass = SDL_BeginGPURenderPass(cmd, &ct, 1, &dt);
    if (!pass)
        port_fatal("gpu_hw: SDL_BeginGPURenderPass: %s\n", SDL_GetError());
    return pass;
}

/* Upload the blocks the batch reads that gpu.c has changed: runs of blocks along each row, rows with the
 * same runs together. Returns the bytes used of the transfer buffer from `at` on. */
static u32 upload_vram(SDL_GPUCopyPass *copy, u8 *mapped, u32 at) {
    u16 need[32];
    int r, r1, c, c1;
    u32 start = at;
    for (r = 0; r < 32; r++) {
        need[r] = dirty.row[r] & sampled.row[r];
        dirty.row[r] &= (u16)~need[r];
    }
    gpu_sync_blocks(need); /* gpu.c rasterizes what it kept for later there */
    for (r = 0; r < 32; r++) {
        while (need[r]) {
            SDL_GPUTextureTransferInfo src;
            SDL_GPUTextureRegion dst;
            u16 run;
            s32 x, y, w, h, j;
            for (c = 0; !(need[r] & (1u << c)); c++)
                ;
            for (c1 = c; c1 < 16 && (need[r] & (1u << c1)); c1++)
                ;
            run = (u16)(((1u << c1) - 1) & ~((1u << c) - 1));
            for (r1 = r; r1 < 32 && (need[r1] & run) == run; r1++)
                need[r1] &= (u16)~run;
            x = c * 64, y = r * 16, w = (c1 - c) * 64, h = (r1 - r) * 16;
            for (j = 0; j < h; j++)
                memcpy(mapped + at + j * w * 2, &gpu_vram[(y + j) * 1024 + x], w * 2);
            memset(&src, 0, sizeof(src));
            memset(&dst, 0, sizeof(dst));
            src.transfer_buffer = upload;
            src.offset = at;
            src.pixels_per_row = w;
            src.rows_per_layer = h;
            dst.texture = vram;
            dst.x = x, dst.y = y, dst.w = w, dst.h = h, dst.d = 1;
            SDL_UploadToGPUTexture(copy, &src, &dst, false);
            if (log_on)
                port_log("hw: frame %u: upload %dx%d at %d,%d\n", cur_frame, w, h, x, y);
            at += w * h * 2;
        }
    }
    st.upload_bytes += at - start;
    return at - start;
}

static void push_uniforms(void) {
    float xform[4];
    s32 params[4] = {scale, truecolor, (s32)(pbuf_used / PLANE_SIZE), 0};
    /* x VRAM pixels -> target pixel x * scale + 0.5 (the pixel whose top left corner samples there), then
     * normalized device coordinates (y up) */
    xform[0] = 2.0f / 1024.0f;
    xform[1] = -2.0f / 512.0f;
    xform[2] = 1.0f / (1024.0f * scale) - 1.0f;
    xform[3] = 1.0f - 1.0f / (512.0f * scale);
    SDL_PushGPUVertexUniformData(cmd, 0, xform, sizeof(xform));
    SDL_PushGPUFragmentUniformData(cmd, 0, params, sizeof(params));
}

/* Draw the batch: upload what it reads and its vertices, then its opaque primitives and its runs. */
static void flush(void) {
    SDL_GPUCopyPass *copy;
    SDL_GPURenderPass *pass;
    SDL_GPUTransferBufferLocation src;
    SDL_GPUBufferRegion dst;
    SDL_GPUBufferBinding vb;
    SDL_GPUTextureSamplerBinding tsb;
    u8 *mapped;
    u32 bytes = (u32)(n_opaque + n_ordered) * sizeof(Vertex), pbytes = (u32)n_planes * PLANE_SIZE, at;
    int i;
    if (!n_opaque && !n_ordered && !clear_color && !clear_depth && !n_images)
        return;
    begin_commands();
    if (vbuf_used + bytes > VBUF_SIZE || pbuf_used + pbytes > PBUF_SIZE) {
        vbuf_used = pbuf_used = 0;
        vbuf_cycle = pbuf_cycle = 1;
    }
    mapped = SDL_MapGPUTransferBuffer(dev, upload, true);
    if (!mapped)
        port_fatal("gpu_hw: SDL_MapGPUTransferBuffer: %s\n", SDL_GetError());
    memcpy(mapped, opaque, n_opaque * sizeof(Vertex));
    memcpy(mapped + n_opaque * sizeof(Vertex), ordered, n_ordered * sizeof(Vertex));
    memcpy(mapped + bytes, planes, pbytes);
    copy = SDL_BeginGPUCopyPass(cmd);
    at = bytes + pbytes;
    at += upload_vram(copy, mapped, at);
    for (i = 0; i < n_images; i++) { /* (after the blocks: they may hold what was there before) */
        SDL_GPUTextureTransferInfo isrc;
        SDL_GPUTextureRegion idst;
        u32 size = (u32)(image[i].w * image[i].h * 2);
        memcpy(mapped + at, images + image[i].at, size);
        memset(&isrc, 0, sizeof(isrc));
        memset(&idst, 0, sizeof(idst));
        isrc.transfer_buffer = upload;
        isrc.offset = at;
        isrc.pixels_per_row = image[i].w;
        isrc.rows_per_layer = image[i].h;
        idst.texture = vram;
        idst.x = image[i].x, idst.y = image[i].y, idst.w = image[i].w, idst.h = image[i].h, idst.d = 1;
        SDL_UploadToGPUTexture(copy, &isrc, &idst, false);
        st.upload_bytes += size;
        at += size;
    }
    n_images = 0;
    images_used = 0;
    SDL_UnmapGPUTransferBuffer(dev, upload);
    if (bytes) {
        src.transfer_buffer = upload;
        src.offset = 0;
        dst.buffer = vbuf;
        dst.offset = vbuf_used;
        dst.size = bytes;
        SDL_UploadToGPUBuffer(copy, &src, &dst, vbuf_cycle);
        vbuf_cycle = 0;
        if (pbytes) {
            src.offset = bytes;
            dst.buffer = pbuf;
            dst.offset = pbuf_used;
            dst.size = pbytes;
            SDL_UploadToGPUBuffer(copy, &src, &dst, pbuf_cycle);
            pbuf_cycle = 0;
        }
    }
    SDL_EndGPUCopyPass(copy);

    pass = begin_target_pass();
    if (bytes) {
        SDL_FColor half = {0.5f, 0.5f, 0.5f, 0.5f}; /* semi-transparency mode 0 */
        SDL_SetGPUBlendConstants(pass, half);
        push_uniforms();
        vb.buffer = vbuf;
        vb.offset = vbuf_used;
        SDL_BindGPUVertexBuffers(pass, 0, &vb, 1);
        tsb.texture = vram;
        tsb.sampler = nearest;
        SDL_BindGPUFragmentSamplers(pass, 0, &tsb, 1);
        SDL_BindGPUFragmentStorageBuffers(pass, 0, &pbuf, 1);
        if (n_opaque) {
            SDL_BindGPUGraphicsPipeline(pass, pipes[P_OPAQUE]);
            SDL_DrawGPUPrimitives(pass, n_opaque, 1, 0, 0);
            st.draws++;
        }
        for (i = 0; i < n_runs; i++) {
            SDL_BindGPUGraphicsPipeline(pass, pipes[runs[i].pipe]);
            SDL_DrawGPUPrimitives(pass, runs[i].count, 1, n_opaque + runs[i].first, 0);
            st.draws++;
        }
        vbuf_used += bytes;
        pbuf_used += pbytes;
    }
    SDL_EndGPURenderPass(pass);
    n_opaque = n_ordered = n_runs = n_planes = 0;
    memset(&sampled, 0, sizeof(sampled));
    st.flushes++;
    if (++cmd_batches >= 64)
        submit(0);
}

static void submit(int wait) {
    SDL_GPUFence *fence;
    if (!cmd)
        return;
    if (!wait) {
        SDL_SubmitGPUCommandBuffer(cmd);
        cmd = NULL;
        return;
    }
    fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
    cmd = NULL;
    if (fence) {
        SDL_WaitForGPUFences(dev, true, &fence, 1);
        SDL_ReleaseGPUFence(dev, fence);
    }
}

/* Room for a primitive's vertices (else the batch is drawn first), and its depth value. */
static float next_prim(int n_op, int n_ord) {
    if (n_opaque + n_op > LIST_MAX || n_ordered + n_ord > LIST_MAX || n_runs + 2 > RUNS_MAX ||
        n_planes >= PLANES_MAX)
        flush();
    if (order >= 65535) {
        flush();
        clear_depth = 1;
        order = 0;
        if (log_on)
            port_log("hw: frame %u: depth starts over\n", cur_frame);
    }
    st.prims++;
    return (float)++order / 65535.0f;
}

static Vertex *add_opaque(int n) {
    Vertex *v = &opaque[n_opaque];
    n_opaque += n;
    return v;
}

static Vertex *add_ordered(int pipe, int n) {
    Vertex *v = &ordered[n_ordered];
    if (!n_runs || runs[n_runs - 1].pipe != pipe) {
        runs[n_runs].pipe = pipe;
        runs[n_runs].first = n_ordered;
        runs[n_runs].count = 0;
        n_runs++;
    }
    runs[n_runs - 1].count += n;
    n_ordered += n;
    return v;
}

/* Before gpu.c changes the VRAM in `w`: if the batch reads any of it, the batch is drawn with it as it is. */
static void will_write(const Blocks *w) {
    if (blocks_meet(w, &sampled))
        flush();
    blocks_add(&dirty, w);
}

/* After recording a textured primitive that reads VRAM it draws to (blocks t and w meet): it is drawn at once,
 * before gpu.c draws it, so it sees VRAM as it was before it (gpu.c sees it change as it draws; that the GPU
 * can't follow). Its blocks are changed again after that. */
static void feedback(const Blocks *w, const Blocks *t) {
    if (!blocks_meet(w, t))
        return;
    flush();
    blocks_add(&dirty, w);
}

/* the flat attributes of a primitive's vertices */
static void prim_attrs(Vertex *t, const GpuHwPrim *p, float z) {
    t->z = z;
    t->w = 1;
    t->a = 0;
    t->mode = (p->tpage & 0x1FF) | (p->dither ? M_DITHER : 0) | (p->textured ? M_TEXTURED : 0) |
              (p->raw ? M_RAW : 0) | (p->gouraud ? M_GOURAUD : 0) | (p->mask_set ? M_MASK_SET : 0);
    t->clut = p->clut_x | (p->clut_y << 16);
    t->window = p->tw_mask_x | (p->tw_mask_y << 5) | (p->tw_off_x << 10) | (p->tw_off_y << 15);
    t->area0 = (u32)p->area_x1 | ((u32)p->area_y1 << 16);
    t->area1 = (u32)p->area_x2 | ((u32)p->area_y2 << 16);
    t->plane = NO_PLANE;
}

/* TWINE_HW_PROBE=<frame>[-<frame>],<x>,<y>: log what those frames draw or copy over that VRAM pixel (x -1:
 * anywhere) */
static int probe_hit(float x0, float y0, float x1, float y1) {
    static int on = -1;
    static u32 from, to;
    static s32 x, y;
    if (on < 0) {
        const char *s = getenv("TWINE_HW_PROBE");
        on = s && (sscanf(s, "%u-%u,%d,%d", &from, &to, &x, &y) == 4 ||
                   (sscanf(s, "%u,%d,%d", &from, &x, &y) == 3 && (to = from)));
    }
    return on && cur_frame >= from && cur_frame <= to && (x < 0 || (x >= x0 && x <= x1 && y >= y0 && y <= y1));
}

static void probe(const GpuHwPrim *p, const Vertex *v, int n) {
    float x0 = 1e9f, y0 = 1e9f, x1 = -1e9f, y1 = -1e9f;
    int i;
    for (i = 0; i < n; i++) {
        x0 = v[i].x < x0 ? v[i].x : x0, x1 = v[i].x > x1 ? v[i].x : x1;
        y0 = v[i].y < y0 ? v[i].y : y0, y1 = v[i].y > y1 ? v[i].y : y1;
    }
    if (probe_hit(x0, y0, x1, y1)) {
        port_log("hw probe: prim %u (%.0f,%.0f)-(%.0f,%.0f) mode %05X semi %d check %d clut %08X rgb %d,%d,%d uv "
                 "%.0f,%.0f\n",
                 order, x0, y0, x1, y1, v[0].mode, p->semi, p->mask_check, v[0].clut, v[0].r, v[0].g, v[0].b, v[0].u,
                 v[0].v);
        if (n == 3)
            port_log("hw probe:   (%.0f,%.0f) (%.0f,%.0f) (%.0f,%.0f) uv %.0f,%.0f %.0f,%.0f %.0f,%.0f\n", v[0].x, v[0].y,
                     v[1].x, v[1].y, v[2].x, v[2].y, v[0].u, v[0].v, v[1].u, v[1].v, v[2].u, v[2].v);
        if (v[0].plane != NO_PLANE) {
            const s32 *q = planes[v[0].plane];
            port_log("hw probe:   planes r %08X %08X %08X, u %08X %08X %08X, v %08X %08X %08X from %d,%d\n", q[0], q[1],
                     q[2], q[9], q[10], q[11], q[12], q[13], q[14], q[15] & 0xFFFF, q[15] >> 16);
        }
    }
}

static void probe_vram(const char *what, s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy) {
    if (probe_hit((float)x, (float)y, (float)(x + w - 1), (float)(y + h - 1)))
        port_log("hw probe: %s %dx%d at %d,%d (from %d,%d)\n", what, w, h, x, y, sx, sy);
}

/* Record n vertices (whole triangles) of a primitive: where they go by its semi-transparency and mask
 * checking (see the top). */
static void emit(const GpuHwPrim *p, const Vertex *v, int n) {
    int semi_mode = (p->tpage >> 5) & 3, i;
    Vertex *o;
    probe(p, v, n);
    if (!p->semi) {
        o = p->mask_check ? add_ordered(P_CHECK, n) : add_opaque(n);
        memcpy(o, v, n * sizeof(Vertex));
        return;
    }
    if (p->textured) {
        o = p->mask_check ? add_ordered(P_CHECK, n) : add_opaque(n);
        for (i = 0; i < n; i++) {
            o[i] = v[i];
            o[i].mode |= M_OPAQUE_TEXELS;
        }
    }
    o = add_ordered((p->mask_check ? P_SEMI_CHECK : P_SEMI) + semi_mode, n);
    for (i = 0; i < n; i++) {
        o[i] = v[i];
        o[i].mode |= M_BLEND | (p->textured ? M_SEMI_TEXELS : 0) | (p->mask_check && !semi_mode ? M_HALVE : 0);
    }
}

static void timed(u64 t0) {
    if (stats_on)
        st.cpu_ns += now_ns() - t0;
}

void gpu_hw_triangle(const GpuHwPrim *p, const GpuHwVertex *v, s32 minx, s32 miny, s32 maxx, s32 maxy,
                     const GpuHwPlane *pl) {
    Blocks w, t;
    Vertex out[3];
    float z;
    u32 plane = NO_PLANE;
    int i;
    u64 t0 = stats_on ? now_ns() : 0;
    RT_ENTER();
    blocks_of(&w, minx, miny, maxx - minx + 1, maxy - miny + 1);
    will_write(&w);
    z = next_prim(3, 3);
    if (p->textured) {
        s32 umin = v[0].u, umax = v[0].u, vmin = v[0].v, vmax = v[0].v;
        for (i = 1; i < 3; i++) {
            umin = v[i].u < umin ? v[i].u : umin;
            umax = v[i].u > umax ? v[i].u : umax;
            vmin = v[i].v < vmin ? v[i].v : vmin;
            vmax = v[i].v > vmax ? v[i].v : vmax;
        }
        texture_blocks(&t, p, umin, umax, vmin, vmax);
        blocks_add(&sampled, &t);
    }
    if (exact && (p->gouraud || p->textured)) {
        s32 *q = planes[n_planes];
        for (i = 0; i < 5; i++) {
            q[i * 3] = (s32)pl[i].at;
            q[i * 3 + 1] = (s32)pl[i].dx;
            q[i * 3 + 2] = (s32)pl[i].dy;
        }
        q[15] = minx | (miny << 16);
        plane = n_planes++;
    }
    for (i = 0; i < 3; i++) {
        prim_attrs(&out[i], p, z);
        out[i].plane = plane;
        out[i].x = (float)v[i].x;
        out[i].y = (float)v[i].y;
        out[i].u = (float)v[i].u;
        out[i].v = (float)v[i].v;
        out[i].r = (u8)v[i].r;
        out[i].g = (u8)v[i].g;
        out[i].b = (u8)v[i].b;
    }
    emit(p, out, 3);
    if (p->textured)
        feedback(&w, &t);
    RT_LEAVE();
    timed(t0);
}

/* two triangles covering (x0, y0)-(x1, y1), the texture from (u0, v0) to (u1, v1) */
static void quad(Vertex *q, const Vertex *a, float x0, float y0, float x1, float y1, float u0, float v0, float u1,
                 float v1) {
    int i;
    for (i = 0; i < 6; i++)
        q[i] = *a;
    q[0].x = x0, q[0].y = y0, q[0].u = u0, q[0].v = v0;
    q[1].x = x1, q[1].y = y0, q[1].u = u1, q[1].v = v0;
    q[2].x = x0, q[2].y = y1, q[2].u = u0, q[2].v = v1;
    q[3] = q[1];
    q[4] = q[2];
    q[5].x = x1, q[5].y = y1, q[5].u = u1, q[5].v = v1;
}

void gpu_hw_rect(const GpuHwPrim *p, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b, s32 u, s32 v, int flip_x,
                 int flip_y) {
    Blocks wb, t;
    Vertex a, q[6];
    s32 x0 = x, y0 = y, x1 = x + w - 1, y1 = y + h - 1;
    float u0, u1, v0, v1;
    const float back = 1.0f - 1.0f / 64.0f; /* flipped: just under the next texel, so it rounds down to it */
    u64 t0 = stats_on ? now_ns() : 0;
    /* only the part inside the drawing area is drawn */
    x0 = x0 < p->area_x1 ? p->area_x1 : x0;
    y0 = y0 < p->area_y1 ? p->area_y1 : y0;
    x1 = x1 > p->area_x2 ? p->area_x2 : x1;
    y1 = y1 > p->area_y2 ? p->area_y2 : y1;
    if (w <= 0 || h <= 0 || x0 > x1 || y0 > y1)
        return;
    RT_ENTER();
    blocks_of(&wb, x0, y0, x1 - x0 + 1, y1 - y0 + 1);
    will_write(&wb);
    a.x = a.y = a.u = a.v = 0;
    prim_attrs(&a, p, next_prim(6, 6));
    a.r = (u8)r, a.g = (u8)g, a.b = (u8)b;
    if (p->textured) {
        texture_blocks(&t, p, flip_x ? u - w + 1 : u, flip_x ? u : u + w - 1, flip_y ? v - h + 1 : v,
                       flip_y ? v : v + h - 1);
        blocks_add(&sampled, &t);
    }
    u0 = flip_x ? u + back : (float)u;
    u1 = flip_x ? u + back - w : (float)(u + w);
    v0 = flip_y ? v + back : (float)v;
    v1 = flip_y ? v + back - h : (float)(v + h);
    quad(q, &a, (float)x, (float)y, (float)(x + w), (float)(y + h), u0, v0, u1, v1);
    emit(p, q, 6);
    if (p->textured)
        feedback(&wb, &t);
    RT_LEAVE();
    timed(t0);
}

void gpu_hw_line(const GpuHwPrim *p, s32 ax, s32 ay, s32 bx, s32 by) {
    Blocks w;
    s32 x0 = ax < bx ? ax : bx, x1 = ax < bx ? bx : ax, y0 = ay < by ? ay : by, y1 = ay < by ? by : ay;
    RT_ENTER();
    x0 = x0 < p->area_x1 ? p->area_x1 : x0;
    y0 = y0 < p->area_y1 ? p->area_y1 : y0;
    x1 = x1 > p->area_x2 ? p->area_x2 : x1;
    y1 = y1 > p->area_y2 ? p->area_y2 : y1;
    if (x0 <= x1 && y0 <= y1) {
        blocks_of(&w, x0, y0, x1 - x0 + 1, y1 - y0 + 1);
        will_write(&w);
    }
    memset(&line_v, 0, sizeof(line_v));
    prim_attrs(&line_v, p, next_prim(0, 0));
    line_v.mode &= ~(u32)M_GOURAUD; /* each pixel is a square of its own colour */
    line_ordered = p->semi || p->mask_check;
    line_pipe = p->semi ? (p->mask_check ? P_SEMI_CHECK : P_SEMI) + ((p->tpage >> 5) & 3) : P_CHECK;
    if (p->semi)
        line_v.mode |= M_BLEND | (p->mask_check && !((p->tpage >> 5) & 3) ? M_HALVE : 0);
    RT_LEAVE();
}

void gpu_hw_line_pixel(s32 x, s32 y, s32 r, s32 g, s32 b) {
    Vertex *q;
    RT_ENTER();
    if (n_opaque + 6 > LIST_MAX || n_ordered + 6 > LIST_MAX || n_runs + 2 > RUNS_MAX)
        flush();
    line_v.r = (u8)r, line_v.g = (u8)g, line_v.b = (u8)b;
    q = line_ordered ? add_ordered(line_pipe, 6) : add_opaque(6);
    quad(q, &line_v, (float)x, (float)y, (float)(x + 1), (float)(y + 1), 0, 0, 0, 0);
    RT_LEAVE();
}

/* A w x h rectangle of VRAM at (x, y) as the pieces it is in where it wraps around VRAM's edges (up to four):
 * where each is in VRAM, where in the rectangle, its size. */
typedef struct {
    s32 x, y, ox, oy, w, h;
} Piece;

static int pieces(s32 x, s32 y, s32 w, s32 h, Piece *p) {
    s32 xs[2], xo[2], ws[2], ys[2], yo[2], hs[2], nx = 1, ny = 1, i, j, n = 0;
    x &= 1023, y &= 511;
    w = w > 1024 ? 1024 : w;
    h = h > 512 ? 512 : h;
    xs[0] = x, xo[0] = 0, ws[0] = x + w > 1024 ? 1024 - x : w;
    ys[0] = y, yo[0] = 0, hs[0] = y + h > 512 ? 512 - y : h;
    if (ws[0] < w)
        xs[1] = 0, xo[1] = ws[0], ws[1] = w - ws[0], nx = 2;
    if (hs[0] < h)
        ys[1] = 0, yo[1] = hs[0], hs[1] = h - hs[0], ny = 2;
    for (j = 0; j < ny; j++)
        for (i = 0; i < nx; i++, n++) {
            p[n].x = xs[i], p[n].y = ys[j], p[n].ox = xo[i], p[n].oy = yo[j], p[n].w = ws[i], p[n].h = hs[j];
        }
    return n;
}

/* a rectangle of VRAM drawn the same everywhere (fill, image): a quad for each piece (the caller has made
 * room for four) */
static void vram_rect(const Vertex *a, s32 x, s32 y, s32 w, s32 h) {
    Piece p[4];
    int n = pieces(x, y, w, h, p), i;
    for (i = 0; i < n; i++)
        quad(add_opaque(6), a, (float)p[i].x, (float)p[i].y, (float)(p[i].x + p[i].w), (float)(p[i].y + p[i].h), 0, 0,
             0, 0);
}

void gpu_hw_fill(s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b) {
    Blocks wb;
    Vertex a;
    if (w <= 0 || h <= 0)
        return;
    RT_ENTER();
    probe_vram("fill", x, y, w, h, r | (g << 8) | (b << 16), 0);
    blocks_of(&wb, x, y, w, h);
    will_write(&wb);
    memset(&a, 0, sizeof(a));
    a.plane = NO_PLANE;
    a.z = next_prim(24, 0);
    a.w = 1;
    a.r = (u8)r, a.g = (u8)g, a.b = (u8)b;
    a.area1 = 1023 | (511u << 16);
    vram_rect(&a, x, y, w, h);
    RT_LEAVE();
}

void gpu_hw_load_image(s32 x, s32 y, s32 w, s32 h, const u16 *src) {
    Blocks wb;
    Vertex a;
    u32 size = (u32)(w * h * 2);
    int direct = x + w <= 1024 && y + h <= 512 && size <= IMAGES_SIZE;
    if (w <= 0 || h <= 0)
        return;
    RT_ENTER();
    probe_vram("image", x, y, w, h, 0, 0);
    blocks_of(&wb, x, y, w, h);
    if (direct) {
        /* the game's pixels go up with the batch: VRAM's copy there is then right without gpu.c's (which
         * may do it later); a batch reading what was there before is drawn first */
        if (blocks_meet(&wb, &sampled) || n_images == IMAGES_MAX || images_used + size > IMAGES_SIZE)
            flush();
        image[n_images].x = x, image[n_images].y = y, image[n_images].w = w, image[n_images].h = h;
        image[n_images].at = images_used;
        memcpy(images + images_used, src, size);
        images_used += size;
        n_images++;
    } else
        will_write(&wb); /* round VRAM's edge: uploaded from gpu.c's VRAM, as the blocks it reads */
    memset(&a, 0, sizeof(a));
    a.plane = NO_PLANE;
    a.z = next_prim(24, 0);
    a.w = 1;
    a.mode = M_BLIT;
    a.area1 = 1023 | (511u << 16);
    if (!direct)
        blocks_add(&sampled, &wb); /* (after next_prim, which may draw the batch) */
    vram_rect(&a, x, y, w, h);
    RT_LEAVE();
}

/* A copy within VRAM: the target's pixels are copied too, all of the source into a texture of the copy's size
 * and from there to the destination (as gpu.c does it: the two may overlap, and wrap around VRAM's edges), so
 * what was drawn there keeps its resolution. */
void gpu_hw_move_image(s32 sx, s32 sy, s32 w, s32 h, s32 dx, s32 dy) {
    Blocks wb;
    SDL_GPUCopyPass *copy;
    SDL_GPUTextureLocation a, b;
    Piece src[4], dst[4];
    int ns, nd, i;
    u32 cw, ch;
    w = w > 1024 ? 1024 : w;
    h = h > 512 ? 512 : h;
    if (w <= 0 || h <= 0)
        return;
    ns = pieces(sx, sy, w, h, src);
    nd = pieces(dx, dy, w, h, dst);
    RT_ENTER();
    probe_vram("copy", dx, dy, w, h, sx, sy);
    if (log_on)
        port_log("hw: frame %u: copy %dx%d from %d,%d to %d,%d\n", cur_frame, w, h, sx, sy, dx, dy);
    flush();
    blocks_of(&wb, dx, dy, w, h);
    blocks_add(&dirty, &wb);
    cw = (u32)w * scale, ch = (u32)h * scale;
    if (!move_tmp || move_w < cw || move_h < ch) {
        SDL_GPUTextureCreateInfo ti;
        if (move_tmp)
            SDL_ReleaseGPUTexture(dev, move_tmp);
        move_w = cw > move_w ? cw : move_w;
        move_h = ch > move_h ? ch : move_h;
        memset(&ti, 0, sizeof(ti));
        ti.type = SDL_GPU_TEXTURETYPE_2D;
        ti.format = target_format;
        ti.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
        ti.width = move_w;
        ti.height = move_h;
        ti.layer_count_or_depth = 1;
        ti.num_levels = 1;
        move_tmp = SDL_CreateGPUTexture(dev, &ti);
        if (!move_tmp)
            port_fatal("gpu_hw: move texture: %s\n", SDL_GetError());
    }
    begin_commands();
    copy = SDL_BeginGPUCopyPass(cmd);
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    a.texture = target;
    b.texture = move_tmp;
    for (i = 0; i < ns; i++) {
        a.x = (u32)src[i].x * scale, a.y = (u32)src[i].y * scale;
        b.x = (u32)src[i].ox * scale, b.y = (u32)src[i].oy * scale;
        SDL_CopyGPUTextureToTexture(copy, &a, &b, (u32)src[i].w * scale, (u32)src[i].h * scale, 1, false);
    }
    for (i = 0; i < nd; i++) {
        a.x = (u32)dst[i].x * scale, a.y = (u32)dst[i].y * scale;
        b.x = (u32)dst[i].ox * scale, b.y = (u32)dst[i].oy * scale;
        SDL_CopyGPUTextureToTexture(copy, &b, &a, (u32)dst[i].w * scale, (u32)dst[i].h * scale, 1, false);
    }
    SDL_EndGPUCopyPass(copy);
    RT_LEAVE();
}

/* gpu_reset: VRAM is cleared */
void gpu_hw_reset(void) {
    if (!dev)
        return;
    RT_ENTER();
    n_opaque = n_ordered = n_runs = n_planes = 0;
    memset(&sampled, 0, sizeof(sampled));
    memset(&dirty, 0xFF, sizeof(dirty));
    clear_color = clear_depth = 1;
    order = 0;
    RT_LEAVE();
}

/* ---- setup ---- */

static int env_int(const char *name, int dflt) {
    const char *s = getenv(name);
    return s && *s ? atoi(s) : dflt;
}

int gpu_hw_wanted(void) {
    const char *r = getenv("TWINE_RENDERER");
    return r && !SDL_strcasecmp(r, "hw");
}

static SDL_GPUShader *shader(const unsigned char *code, unsigned len, SDL_GPUShaderStage stage, int samplers,
                             int storage_buffers, int uniforms) {
    SDL_GPUShaderCreateInfo si;
    SDL_GPUShader *s;
    memset(&si, 0, sizeof(si));
    si.code_size = len;
    si.code = code;
    si.entrypoint = "main";
    si.format = SDL_GPU_SHADERFORMAT_SPIRV;
    si.stage = stage;
    si.num_samplers = samplers;
    si.num_storage_buffers = storage_buffers;
    si.num_uniform_buffers = uniforms;
    s = SDL_CreateGPUShader(dev, &si);
    if (!s)
        port_log("gpu_hw: shader: %s\n", SDL_GetError());
    return s;
}

/* The blending of pipeline `k`: semi-transparency (B = what is there, F = the primitive) 0: B / 2 + F / 2
 * (the blend constant is a half), 1: B + F, 2: B - F, 3: B + F / 4 (the shader quarters F); with mask checking,
 * F only where the mask bit (alpha) is clear. Alpha is the primitive's mask bit. */
static SDL_GPUColorTargetBlendState blend_of(int k) {
    SDL_GPUColorTargetBlendState b;
    int check = k >= P_CHECK, mode = k - (check ? P_SEMI_CHECK : P_SEMI);
    memset(&b, 0, sizeof(b));
    b.enable_blend = true;
    b.color_blend_op = b.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
    b.src_color_blendfactor = check ? SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_ALPHA : SDL_GPU_BLENDFACTOR_ONE;
    b.src_alpha_blendfactor = b.src_color_blendfactor;
    b.dst_alpha_blendfactor = check ? SDL_GPU_BLENDFACTOR_ONE : SDL_GPU_BLENDFACTOR_ZERO;
    if (k == P_OPAQUE)
        b.enable_blend = false;
    else if (k == P_CHECK)
        b.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_DST_ALPHA;
    else if (mode == 0) {
        if (!check)
            b.src_color_blendfactor = SDL_GPU_BLENDFACTOR_CONSTANT_COLOR;
        b.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_CONSTANT_COLOR;
    } else {
        b.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
        if (mode == 2)
            b.color_blend_op = SDL_GPU_BLENDOP_REVERSE_SUBTRACT;
    }
    return b;
}

static SDL_GPUTexture *texture(SDL_GPUTextureFormat f, SDL_GPUTextureUsageFlags usage, u32 w, u32 h) {
    SDL_GPUTextureCreateInfo ti;
    SDL_GPUTexture *t;
    memset(&ti, 0, sizeof(ti));
    ti.type = SDL_GPU_TEXTURETYPE_2D;
    ti.format = f;
    ti.usage = usage;
    ti.width = w;
    ti.height = h;
    ti.layer_count_or_depth = 1;
    ti.num_levels = 1;
    t = SDL_CreateGPUTexture(dev, &ti);
    if (!t)
        port_log("gpu_hw: texture %ux%u: %s\n", w, h, SDL_GetError());
    return t;
}

static int create_pipelines(void) {
    SDL_GPUShader *vs = shader(hw_vert_spv, hw_vert_spv_len, SDL_GPU_SHADERSTAGE_VERTEX, 0, 0, 1);
    SDL_GPUShader *fs = shader(hw_frag_spv, hw_frag_spv_len, SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 1, 1);
    SDL_GPUShader *pvs = shader(picture_vert_spv, picture_vert_spv_len, SDL_GPU_SHADERSTAGE_VERTEX, 0, 0, 0);
    SDL_GPUShader *pfs = shader(picture_frag_spv, picture_frag_spv_len, SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 0, 1);
    SDL_GPUVertexBufferDescription vbd;
    SDL_GPUVertexAttribute attrs[6] = {
        {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, offsetof(Vertex, x)},
        {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Vertex, u)},
        {2, 0, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM, offsetof(Vertex, r)},
        {3, 0, SDL_GPU_VERTEXELEMENTFORMAT_UINT4, offsetof(Vertex, mode)},
        {4, 0, SDL_GPU_VERTEXELEMENTFORMAT_UINT, offsetof(Vertex, area1)},
        {5, 0, SDL_GPU_VERTEXELEMENTFORMAT_UINT, offsetof(Vertex, plane)},
    };
    SDL_GPUColorTargetDescription ctd;
    SDL_GPUGraphicsPipelineCreateInfo pi;
    int k;
    if (!vs || !fs || !pvs || !pfs)
        return 0;
    memset(&vbd, 0, sizeof(vbd));
    vbd.slot = 0;
    vbd.pitch = sizeof(Vertex);
    vbd.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    for (k = 0; k < P_COUNT; k++) {
        memset(&pi, 0, sizeof(pi));
        memset(&ctd, 0, sizeof(ctd));
        ctd.format = target_format;
        ctd.blend_state = blend_of(k);
        pi.vertex_shader = vs;
        pi.fragment_shader = fs;
        pi.vertex_input_state.vertex_buffer_descriptions = &vbd;
        pi.vertex_input_state.num_vertex_buffers = 1;
        pi.vertex_input_state.vertex_attributes = attrs;
        pi.vertex_input_state.num_vertex_attributes = 6;
        pi.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
        pi.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
        pi.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
        pi.rasterizer_state.enable_depth_clip = false;
        pi.depth_stencil_state.enable_depth_test = true;
        pi.depth_stencil_state.enable_depth_write = true;
        pi.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_GREATER;
        pi.target_info.color_target_descriptions = &ctd;
        pi.target_info.num_color_targets = 1;
        pi.target_info.has_depth_stencil_target = true;
        pi.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D16_UNORM;
        pipes[k] = SDL_CreateGPUGraphicsPipeline(dev, &pi);
        if (!pipes[k]) {
            port_log("gpu_hw: pipeline %d: %s\n", k, SDL_GetError());
            return 0;
        }
    }
    memset(&pi, 0, sizeof(pi));
    memset(&ctd, 0, sizeof(ctd));
    ctd.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    pi.vertex_shader = pvs;
    pi.fragment_shader = pfs;
    pi.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    pi.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    pi.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
    pi.target_info.color_target_descriptions = &ctd;
    pi.target_info.num_color_targets = 1;
    picture_pipe = SDL_CreateGPUGraphicsPipeline(dev, &pi);
    if (!picture_pipe) {
        port_log("gpu_hw: picture pipeline: %s\n", SDL_GetError());
        return 0;
    }
    SDL_ReleaseGPUShader(dev, vs);
    SDL_ReleaseGPUShader(dev, fs);
    SDL_ReleaseGPUShader(dev, pvs);
    SDL_ReleaseGPUShader(dev, pfs);
    return 1;
}

/* The device and everything the renderer draws with. SDL's video subsystem must be up (video.c). */
int gpu_hw_init(void) {
    SDL_GPUSamplerCreateInfo si;
    SDL_GPUBufferCreateInfo bi;
    SDL_GPUTransferBufferCreateInfo ti;
    SDL_PropertiesID props;
    scale = env_int("TWINE_SCALE", 1);
    scale = scale < 1 ? 1 : scale > 8 ? 8 : scale;
    truecolor = env_int("TWINE_TRUECOLOR", scale > 1) != 0;
    exact = scale == 1 && !truecolor;
    stats_on = env_int("TWINE_HW_STATS", 0);
    compare_on = env_int("TWINE_HW_COMPARE", 0);
    log_on = env_int("TWINE_HW_LOG", 0);
    dev = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, env_int("TWINE_HW_DEBUG", 0) != 0, "vulkan");
    if (!dev) {
        port_log("gpu_hw: no Vulkan GPU device: %s\n", SDL_GetError());
        return 0;
    }
    props = SDL_GetGPUDeviceProperties(dev);
    target_format = truecolor ? SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM : SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UNORM;
    if (!create_pipelines())
        goto fail;
    target = texture(target_format, SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER, 1024 * scale,
                     512 * scale);
    depth = texture(SDL_GPU_TEXTUREFORMAT_D16_UNORM, SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET, 1024 * scale,
                    512 * scale);
    vram = texture(SDL_GPU_TEXTUREFORMAT_R16_UINT, SDL_GPU_TEXTUREUSAGE_SAMPLER, 1024, 512);
    if (!target || !depth || !vram)
        goto fail;
    memset(&si, 0, sizeof(si));
    si.min_filter = si.mag_filter = SDL_GPU_FILTER_NEAREST;
    si.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
    si.address_mode_u = si.address_mode_v = si.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    nearest = SDL_CreateGPUSampler(dev, &si);
    memset(&bi, 0, sizeof(bi));
    bi.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    bi.size = VBUF_SIZE;
    vbuf = SDL_CreateGPUBuffer(dev, &bi);
    bi.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ;
    bi.size = PBUF_SIZE;
    pbuf = SDL_CreateGPUBuffer(dev, &bi);
    memset(&ti, 0, sizeof(ti));
    ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    ti.size = UPLOAD_SIZE;
    upload = SDL_CreateGPUTransferBuffer(dev, &ti);
    if (!nearest || !vbuf || !pbuf || !upload) {
        port_log("gpu_hw: %s\n", SDL_GetError());
        goto fail;
    }
    memset(&dirty, 0xFF, sizeof(dirty));
    clear_color = clear_depth = 1;
    gpu_hw_on = 1;
    port_log("gpu_hw: %s (%s %s), scale %d (%dx%d), %s\n",
             SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_NAME_STRING, "?"),
             SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_DRIVER_NAME_STRING, "?"),
             SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_DRIVER_VERSION_STRING, "?"), scale, 1024 * scale,
             512 * scale, truecolor ? "true colour" : "15-bit colour with dithering");
    return 1;
fail:
    SDL_DestroyGPUDevice(dev);
    dev = NULL;
    return 0;
}

void *gpu_hw_device(void) {
    return dev;
}

int gpu_hw_scale(void) {
    return scale;
}

/* ---- the display ---- */

void gpu_hw_picture(void *dst, int dst_w, int dst_h, s32 x, s32 y, s32 w, s32 h) {
    SDL_GPUColorTargetInfo ct;
    SDL_GPUTextureSamplerBinding tsb;
    SDL_GPURenderPass *pass;
    float params[8];
    u64 t0 = stats_on ? now_ns() : 0, t1;
    RT_ENTER();
    flush();
    begin_commands();
    memset(&ct, 0, sizeof(ct));
    ct.texture = dst;
    ct.load_op = SDL_GPU_LOADOP_DONT_CARE;
    ct.store_op = SDL_GPU_STOREOP_STORE;
    ct.cycle = true;
    pass = SDL_BeginGPURenderPass(cmd, &ct, 1, NULL);
    if (pass) {
        SDL_GPUViewport vp = {0, 0, (float)dst_w, (float)dst_h, 0, 1};
        SDL_SetGPUViewport(pass, &vp);
        SDL_BindGPUGraphicsPipeline(pass, picture_pipe);
        tsb.texture = target;
        tsb.sampler = nearest;
        SDL_BindGPUFragmentSamplers(pass, 0, &tsb, 1);
        params[0] = (float)(x * scale), params[1] = (float)(y * scale);
        params[2] = (float)(w * scale), params[3] = (float)(h * scale);
        params[4] = (float)dst_w, params[5] = (float)dst_h, params[6] = truecolor ? 0.0f : 1.0f, params[7] = 0;
        SDL_PushGPUFragmentUniformData(cmd, 0, params, sizeof(params));
        SDL_DrawGPUPrimitives(pass, 3, 1, 0, 0);
        SDL_EndGPURenderPass(pass);
    }
    if (stats_on) {
        t1 = now_ns();
        st.cpu_ns += t1 - t0;
        submit(1);
        st.gpu_ns += now_ns() - t1;
    } else
        submit(0);
    RT_LEAVE();
}

/* the whole part of a 15-bit-mode target channel (hw.frag) */
static u16 channel5(u16 v) {
    return (u16)((v + 4) / 2080);
}

/* The target's pixels in (x, y, w, h) (VRAM pixels) as 0xBBGGRR, scale x scale for each VRAM pixel; in 15-bit
 * colour also as VRAM pixels (`raw`, may be NULL). */
static int read_target(s32 x, s32 y, s32 w, s32 h, u32 *rgb, u16 *raw) {
    SDL_GPUCopyPass *copy;
    SDL_GPUTextureRegion src;
    SDL_GPUTextureTransferInfo dst;
    u32 pw = (u32)w * scale, ph = (u32)h * scale, bpp = truecolor ? 4 : 8, i;
    const u8 *m;
    if (pw * ph * bpp > download_size) {
        SDL_GPUTransferBufferCreateInfo ti;
        if (download)
            SDL_ReleaseGPUTransferBuffer(dev, download);
        memset(&ti, 0, sizeof(ti));
        ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
        ti.size = download_size = pw * ph * bpp;
        download = SDL_CreateGPUTransferBuffer(dev, &ti);
        if (!download) {
            download_size = 0;
            return 0;
        }
    }
    flush();
    begin_commands();
    copy = SDL_BeginGPUCopyPass(cmd);
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    src.texture = target;
    src.x = (u32)x * scale, src.y = (u32)y * scale, src.w = pw, src.h = ph, src.d = 1;
    dst.transfer_buffer = download;
    SDL_DownloadFromGPUTexture(copy, &src, &dst);
    SDL_EndGPUCopyPass(copy);
    submit(1);
    m = SDL_MapGPUTransferBuffer(dev, download, false);
    if (!m)
        return 0;
    for (i = 0; i < pw * ph; i++) {
        if (truecolor) {
            rgb[i] = m[i * 4] | (m[i * 4 + 1] << 8) | (m[i * 4 + 2] << 16);
        } else {
            const u16 *c = (const u16 *)(m + i * 8);
            u16 v = (u16)(channel5(c[0]) | (channel5(c[1]) << 5) | (channel5(c[2]) << 10) | (c[3] >= 0x8000 ? 0x8000 : 0));
            rgb[i] = ((v & 31) << 3) | (((v >> 5) & 31) << 11) | (((v >> 10) & 31) << 19);
            if (raw)
                raw[i] = v;
        }
    }
    SDL_UnmapGPUTransferBuffer(dev, download);
    return 1;
}

static void write_ppm(const char *path, const u32 *px, u32 w, u32 h) {
    FILE *f = fopen(path, "wb");
    u32 i;
    if (!f)
        return;
    fprintf(f, "P6\n%u %u\n255\n", w, h);
    for (i = 0; i < w * h; i++) {
        u8 c[3] = {(u8)px[i], (u8)(px[i] >> 8), (u8)(px[i] >> 16)};
        fwrite(c, 1, 3, f);
    }
    fclose(f);
}

static u32 vram_rgb(u16 c) {
    return ((c & 31) << 3) | (((c >> 5) & 31) << 11) | (((c >> 10) & 31) << 19);
}

/* TWINE_HW_COMPARE: the frame against gpu.c's VRAM */
static void compare(u32 frame, s32 x, s32 y, s32 w, s32 h, const char *shot) {
    static u32 *rgb;
    static u16 *raw;
    s32 i, j, k, n = 0, mask_only = 0;
    struct {
        s32 d, x, y;
        u16 sw, hw;
    } worst[5];
    double same;
    char line[512];
    int len;
    if (scale != 1 || truecolor) {
        static int said;
        if (!said++)
            port_log("gpu_hw: TWINE_HW_COMPARE needs scale 1 in 15-bit colour\n");
        return;
    }
    gpu_sync(x, y, w, h); /* what gpu.c kept for later */
    rgb = realloc(rgb, w * h * sizeof(u32));
    raw = realloc(raw, w * h * sizeof(u16));
    if (!read_target(x, y, w, h, rgb, raw))
        return;
    memset(worst, 0, sizeof(worst));
    for (j = 0; j < h; j++)
        for (i = 0; i < w; i++) {
            u16 sw = gpu_vram[((y + j) & 511) * 1024 + ((x + i) & 1023)], hw = raw[j * w + i];
            s32 d = 0, c;
            if (sw == hw)
                continue;
            if ((sw & 0x7FFF) == (hw & 0x7FFF)) {
                mask_only++;
                continue;
            }
            n++;
            for (c = 0; c < 15; c += 5)
                d += abs((s32)((sw >> c) & 31) - (s32)((hw >> c) & 31));
            for (k = 0; k < 5; k++)
                if (d > worst[k].d) {
                    memmove(&worst[k + 1], &worst[k], (4 - k) * sizeof(worst[0]));
                    worst[k].d = d, worst[k].x = x + i, worst[k].y = y + j, worst[k].sw = sw, worst[k].hw = hw;
                    break;
                }
        }
    same = 100.0 * (w * h - n) / (w * h);
    len = snprintf(line, sizeof(line), "hw compare %u: %.3f%% identical (%d of %d pixels differ, %d more in the mask bit only)",
                   frame, same, n, w * h, mask_only);
    for (k = 0; k < 5 && worst[k].d; k++)
        len += snprintf(line + len, sizeof(line) - len, "%s (%d,%d) %04X/%04X", k ? "," : "; worst", worst[k].x,
                        worst[k].y, worst[k].sw, worst[k].hw);
    port_log("%s\n", line);
    cmp.frames++;
    cmp.sum += same;
    if (cmp.frames == 1 || same < cmp.worst)
        cmp.worst = same, cmp.worst_frame = frame;
    if (cmp.frames % 100 == 0)
        port_log("hw compare: %u frames, %.3f%% identical on average, worst %.3f%% (%u)\n", cmp.frames,
                 cmp.sum / cmp.frames, cmp.worst, cmp.worst_frame);
    if (shot) {
        /* gpu.c | GPU | the difference (white where the colour differs, grey where only the mask bit does) */
        u32 *side = malloc(w * 3 * h * sizeof(u32));
        char path[600];
        const char *base = strrchr(shot, '/');
        for (j = 0; j < h; j++)
            for (i = 0; i < w; i++) {
                u16 sw = gpu_vram[((y + j) & 511) * 1024 + ((x + i) & 1023)], hw = raw[j * w + i];
                side[j * w * 3 + i] = vram_rgb(sw);
                side[j * w * 3 + w + i] = rgb[j * w + i];
                side[j * w * 3 + 2 * w + i] = sw == hw ? 0 : (sw & 0x7FFF) == (hw & 0x7FFF) ? 0x606060 : 0xFFFFFF;
            }
        base = base ? base + 1 : shot;
        snprintf(path, sizeof(path), "cmp_%s", base);
        write_ppm(path, side, w * 3, h);
        free(side);
    }
}

void gpu_hw_frame(u32 frame, s32 x, s32 y, s32 w, s32 h, const char *shot) {
    u64 t0;
    int shown = w > 0 && h > 0 && w <= 1024 && h <= 512;
    RT_ENTER();
    t0 = stats_on ? now_ns() : 0;
    flush();
    if (stats_on && t0)
        st.cpu_ns += now_ns() - t0;
    if (shown && compare_on)
        compare(frame, x, y, w, h, shot);
    else if (shown && shot) {
        u32 *rgb = malloc((size_t)w * h * scale * scale * sizeof(u32));
        if (rgb && read_target(x, y, w, h, rgb, NULL))
            write_ppm(shot, rgb, w * scale, h * scale);
        free(rgb);
    }
    if (stats_on) {
        t0 = now_ns();
        submit(1);
        st.gpu_ns += now_ns() - t0;
    } else
        submit(0);
    if (log_on)
        port_log("hw: frame %u: depth %u, %d batches since the last submit\n", frame, order, cmd_batches);
    cur_frame = frame + 1;
    if (stats_on && ++st.frames == 100) {
        port_log("hw stats: per frame %.0f primitives, %.1f batches, %.1f draws, %.1f KB uploaded; CPU %.3f ms, GPU %.3f ms "
                 "(scale %d)\n",
                 st.prims / 100.0, st.flushes / 100.0, st.draws / 100.0, st.upload_bytes / 102400.0,
                 st.cpu_ns / 1e8, st.gpu_ns / 1e8, scale);
        memset(&st, 0, sizeof(st));
    }
    RT_LEAVE();
}
