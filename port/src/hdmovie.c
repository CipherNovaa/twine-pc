/* The film's own footage in place of a PS1 movie: hd_movies = <folder> in twine.ini (TWINE_HD_MOVIES) holding
 * <NAME>.mpg clips (MPEG-1 video, MP2 audio), as port/tools/hdmovies.py cuts them from the user's copy of the
 * film (by default $XDG_DATA_HOME/twine/movies, ~/.local/share/twine/movies; hd_movies = 0 turns them off).
 * The movie player asks before it plays a movie (fixes/movie.c); a clip found is shown here, filling
 * the window (hd_fit = zoom, the default: the film's sides cut off; fit: all of it, with black bars) with its
 * own sound, the game's held meanwhile, until it ends or is skipped (Enter, Esc, Space, Z, Start or
 * Cross), and the game goes on as if the PS1 movie had played. Decoding is pl_mpeg's (port/third_party, MIT). */
#define PL_MPEG_IMPLEMENTATION
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "spu.h"
#include "../third_party/pl_mpeg.h"

void *port_video_renderer(void);
int port_video_headless(void);
int port_video_poll_skip(void);

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_AudioStream *audio;
    int zoom;
} Show;

static void SDLCALL on_video(plm_t *plm, plm_frame_t *f, void *user) {
    Show *s = user;
    SDL_FRect dst;
    int ww = 0, wh = 0;
    (void)plm;
    if (!s->texture) {
        s->texture = SDL_CreateTexture(s->renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, f->width,
                                       f->height);
        if (!s->texture)
            return;
        SDL_SetTextureScaleMode(s->texture, SDL_SCALEMODE_LINEAR);
    }
    SDL_UpdateYUVTexture(s->texture, NULL, f->y.data, f->y.width, f->cb.data, f->cb.width, f->cr.data, f->cr.width);
    SDL_GetCurrentRenderOutputSize(s->renderer, &ww, &wh);
    dst.w = (float)ww;
    dst.h = (float)ww * f->height / f->width;
    if ((dst.h > wh) != s->zoom) {
        dst.h = (float)wh;
        dst.w = (float)wh * f->width / f->height;
    }
    dst.x = (ww - dst.w) / 2;
    dst.y = (wh - dst.h) / 2;
    SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 255);
    SDL_RenderClear(s->renderer);
    SDL_RenderTexture(s->renderer, s->texture, NULL, &dst);
    SDL_RenderPresent(s->renderer);
}

static void SDLCALL on_audio(plm_t *plm, plm_samples_t *samples, void *user) {
    Show *s = user;
    (void)plm;
    if (s->audio)
        SDL_PutAudioStreamData(s->audio, samples->interleaved, samples->count * 2 * sizeof(float));
}

/* 1 if <name>'s clip was shown (the PS1 movie is then not to be played) */
int port_hd_movie(const char *name) {
    const char *dir = getenv("TWINE_HD_MOVIES"), *xdg = getenv("XDG_DATA_HOME"), *home = getenv("HOME");
    char path[1024], def[900];
    plm_t *plm;
    Show s = {0};
    SDL_AudioSpec spec;
    Uint64 last;
    int skipped = 0;
    if (!dir) {
        if (xdg && *xdg)
            snprintf(def, sizeof(def), "%s/twine/movies", xdg);
        else
            snprintf(def, sizeof(def), "%s/.local/share/twine/movies", home ? home : ".");
        dir = def;
    }
    port_log("hd movie: %s asked for\n", name);
    if (!*dir || !strcmp(dir, "0") || port_video_headless() || !(s.renderer = port_video_renderer()))
        return 0;
    snprintf(path, sizeof(path), "%s/%s.mpg", dir, name);
    RT_ENTER();
    plm = plm_create_with_filename(path);
    if (!plm || !plm_probe(plm, 5000 * 1024) || !plm_get_num_video_streams(plm)) {
        port_log("hd movie: no clip %s\n", path);
        if (plm)
            plm_destroy(plm);
        RT_LEAVE();
        return 0;
    }
    port_log("hd movie: %s (%.0f s)\n", path, plm_get_duration(plm));
    s.zoom = !getenv("TWINE_HD_FIT") || SDL_strcasecmp(getenv("TWINE_HD_FIT"), "fit");
    port_audio_hold(1);
    if (plm_get_num_audio_streams(plm)) {
        spec.format = SDL_AUDIO_F32;
        spec.channels = 2;
        spec.freq = plm_get_samplerate(plm);
        s.audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
        if (s.audio)
            SDL_ResumeAudioStreamDevice(s.audio);
        /* the sound is queued ahead by the device's own lag, so it comes out with its frame */
        plm_set_audio_lead_time(plm, 0.05);
    } else
        plm_set_audio_enabled(plm, 0);
    plm_set_video_decode_callback(plm, on_video, &s);
    plm_set_audio_decode_callback(plm, on_audio, &s);
    last = SDL_GetTicksNS();
    while (!plm_has_ended(plm) && !(skipped = port_video_poll_skip())) {
        Uint64 now = SDL_GetTicksNS();
        double dt = (now - last) / 1e9;
        last = now;
        plm_decode(plm, dt > 0.1 ? 0.1 : dt);
        SDL_Delay(1);
    }
    if (skipped)
        port_log("hd movie: skipped\n");
    if (s.audio)
        SDL_DestroyAudioStream(s.audio);
    port_audio_hold(0);
    if (s.texture)
        SDL_DestroyTexture(s.texture);
    plm_destroy(plm);
    RT_LEAVE();
    return 1;
}
