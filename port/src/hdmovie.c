/* The film's own footage in place of a PS1 movie: hd_movies = <folder> in twine.ini (TWINE_HD_MOVIES) holding
 * <NAME>.mpg clips (MPEG-1 video, MP2 audio), as port/tools/hdmovies.py cuts them from the user's copy of the
 * film (by default an hdmovies folder next to the game, else $XDG_DATA_HOME/twine/movies, ~/.local/share/twine/movies; hd_movies = 0 turns them off).
 * The movie player asks before it plays a movie (fixes/movie.c); a clip found is shown here, filling
 * the window (hd_fit = zoom, the default: the film's sides cut off; fit: all of it, with black bars) with its
 * own sound, the game's held meanwhile, until it ends or is skipped (Enter, Esc, Space, Z, Start or
 * Cross), and the game goes on as if the PS1 movie had played. Decoding is pl_mpeg's (port/third_party, MIT). */
#define PL_MPEG_IMPLEMENTATION
#include <SDL3/SDL.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "port.h"
#include "spu.h"
#include "../third_party/pl_mpeg.h"

void *port_video_renderer(void);
int port_video_headless(void);
int port_video_poll_skip(void);
int port_video_skip_held(void);

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_AudioStream *audio;
    int zoom;
    int catching_up; /* after a stall: the sound passed meanwhile is left out */
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
    if (s->audio && !s->catching_up)
        SDL_PutAudioStreamData(s->audio, samples->interleaved, samples->count * 2 * sizeof(float));
}

/* where the clips are: TWINE_HD_MOVIES, else an hdmovies folder next to the game (make dist), else the user's own */
static const char *movies_dir(char *def, size_t size) {
    const char *dir = getenv("TWINE_HD_MOVIES"), *xdg = getenv("XDG_DATA_HOME"), *home = getenv("HOME");
    ssize_t n;
    char *slash;
    if (dir)
        return dir;
    n = readlink("/proc/self/exe", def, size - 10);
    slash = n > 0 ? (def[n] = 0, strrchr(def, '/')) : NULL;
    if (slash && (strcpy(slash + 1, "hdmovies"), access(def, F_OK) == 0))
        return def;
    if (xdg && *xdg)
        snprintf(def, size, "%s/twine/movies", xdg);
    else
        snprintf(def, size, "%s/.local/share/twine/movies", home ? home : ".");
    return def;
}

/* 1 if there are clips to play (PORT SETTINGS' HD MOVIES: a setting with nothing to play shows NONE); looked again
 * every two seconds, so a folder put there while the game runs counts */
int port_hd_movies_found(void) {
    static int found = -1;
    static Uint64 at;
    char def[900];
    const char *dir;
    DIR *d;
    struct dirent *e;
    int was = found;
    if (found >= 0 && SDL_GetTicks() - at < 2000)
        return found;
    at = SDL_GetTicks();
    found = 0;
    dir = movies_dir(def, sizeof(def));
    if (*dir && strcmp(dir, "0") && (d = opendir(dir))) {
        while (!found && (e = readdir(d))) {
            size_t l = strlen(e->d_name);
            found = l > 4 && !strcasecmp(e->d_name + l - 4, ".mpg");
        }
        closedir(d);
    }
    if (found != was)
        port_log("hd movie: %s in %s\n", found ? "clips" : "no clips (PORT SETTINGS: NONE)", dir);
    return found;
}

/* 1 if <name>'s clip was shown (the PS1 movie is then not to be played) */
int port_hd_movie(const char *name) {
    const char *dir;
    char path[1024], def[900];
    plm_t *plm;
    Show s = {0};
    SDL_AudioSpec spec;
    Uint64 start;
    double shown = 0;
    int skipped = 0;
    if (getenv("TWINE_HD") && !atoi(getenv("TWINE_HD")))
        return 0; /* hd = 0: the PS1's movies (the front end's PORT SETTINGS) */
    dir = movies_dir(def, sizeof(def));
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
    if (plm_get_num_audio_streams(plm) && !getenv("TWINE_HEADLESS")) { /* (headless: no sound device, ever) */
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
    /* the picture keeps to the time since the start, as the sound does; after a stall it catches up */
    start = SDL_GetTicksNS();
    while (!plm_has_ended(plm) && !(skipped = port_video_poll_skip())) {
        double now = (SDL_GetTicksNS() - start) / 1e9;
        s.catching_up = now - shown > 0.25;
        plm_decode(plm, now - shown);
        s.catching_up = 0;
        shown = now;
        SDL_Delay(1);
    }
    if (skipped) {
        /* the key or button that skipped is let go before the game reads the controls again */
        Uint64 until = SDL_GetTicksNS() + 2000000000ull;
        port_log("hd movie: skipped\n");
        while (port_video_skip_held() && SDL_GetTicksNS() < until) {
            SDL_PumpEvents();
            SDL_Delay(5);
        }
    }
    if (s.audio)
        SDL_DestroyAudioStream(s.audio);
    port_audio_hold(0);
    if (s.texture)
        SDL_DestroyTexture(s.texture);
    plm_destroy(plm);
    RT_LEAVE();
    return 1;
}
