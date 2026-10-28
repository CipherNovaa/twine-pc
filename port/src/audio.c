/* Audio output (SDL3): a 44100 Hz stereo S16 stream whose callback, on SDL's audio thread, pulls samples
 * from the SPU (spu.c then guards its state with a mutex). The callback runs no game code and calls nothing
 * of the runtime but spu_render. Without a device (TWINE_HEADLESS=1, or no audio available) the SPU
 * follows host time instead, so voices still play and end for the game's status polls.
 * TWINE_HEADLESS=1 TWINE_WAV=<path> records what the SPU mixes (the intro's, then the game's) as a WAV. */
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "spu.h"

#define CHUNK 256 /* frames rendered per lock, so register writes from the game never wait long */

static SDL_AudioStream *stream;
static FILE *wav;
static u32 wav_frames;

static void put32(u8 *p, u32 v) {
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
    p[2] = (u8)(v >> 16);
    p[3] = (u8)(v >> 24);
}

/* 44-byte RIFF header of 16-bit stereo PCM at 44100 Hz, sized for what has been written so far (rewritten
 * after every block, as the process may end any time) */
static void wav_header(void) {
    u8 h[44] = {'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0, 2, 0,
                0x44, 0xAC, 0, 0, 0x10, 0xB1, 2, 0, 4, 0, 16, 0, 'd', 'a', 't', 'a'};
    put32(h + 4, 36 + wav_frames * 4);
    put32(h + 40, wav_frames * 4);
    fseek(wav, 0, SEEK_SET);
    fwrite(h, 1, sizeof(h), wav);
    fseek(wav, 0, SEEK_END);
}

void port_audio_wav(const s16 *samples, int frames) {
    if (!wav || frames <= 0)
        return;
    RT_ENTER();
    fwrite(samples, 4, frames, wav);
    wav_frames += frames;
    wav_header();
    fflush(wav);
    RT_LEAVE();
}

static void SDLCALL feed(void *userdata, SDL_AudioStream *s, int additional, int total) {
    static s16 buf[CHUNK * 2];
    int frames = (additional + 3) / 4;
    (void)userdata;
    (void)total;
    while (frames > 0) {
        int n = frames < CHUNK ? frames : CHUNK;
        spu_render(buf, n);
        SDL_PutAudioStreamData(s, buf, n * 4);
        frames -= n;
    }
}

void port_notice(const char *key, const char *fmt, ...); /* setup.c */

void port_audio_init(void) {
    SDL_AudioSpec spec;
    if (getenv("TWINE_HEADLESS")) {
        spu_set_clock(SPU_CLOCK_HOST);
        if (getenv("TWINE_WAV") && (wav = fopen(getenv("TWINE_WAV"), "wb"))) {
            wav_header();
            spu_set_sink(port_audio_wav);
        }
        return;
    }
    RT_ENTER();
    SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, "512");
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;
    spec.freq = 44100;
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO) || !spu_set_clock(SPU_CLOCK_DEVICE) ||
        !(stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, feed, NULL))) {
        port_log("audio: no output (%s); the SPU runs on host time\n", SDL_GetError());
        port_notice("sound", "No sound device could be opened (%s), so the game plays without sound.",
                    SDL_GetError());
        spu_set_clock(SPU_CLOCK_HOST);
    } else if (!SDL_ResumeAudioStreamDevice(stream)) {
        port_log("audio: cannot start the device (%s); the SPU runs on host time\n", SDL_GetError());
        SDL_DestroyAudioStream(stream);
        stream = NULL;
        spu_set_clock(SPU_CLOCK_HOST);
    }
    RT_LEAVE();
}

/* The game's sound stopped (1) or going again (0), while something else plays (hdmovie.c); the SPU, on the
 * device's clock, waits meanwhile. */
void port_audio_hold(int hold) {
    if (!stream)
        return;
    RT_ENTER();
    if (hold)
        SDL_PauseAudioStreamDevice(stream);
    else
        SDL_ResumeAudioStreamDevice(stream);
    RT_LEAVE();
}
