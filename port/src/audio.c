/* Audio output (SDL3): a 44100 Hz stereo S16 stream whose callback, on SDL's audio thread, pulls samples
 * from the SPU (spu.c then guards its state with a mutex). The callback runs no game code and calls nothing
 * of the runtime but spu_render. Without a device (TWINE_HEADLESS=1, or no audio available) the SPU
 * follows host time instead, so voices still play and end for the game's status polls. */
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "spu.h"

#define CHUNK 256 /* frames rendered per lock, so register writes from the game never wait long */

static SDL_AudioStream *stream;

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

void port_audio_init(void) {
    SDL_AudioSpec spec;
    if (getenv("TWINE_HEADLESS")) {
        spu_set_clock(SPU_CLOCK_HOST);
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
        spu_set_clock(SPU_CLOCK_HOST);
    } else if (!SDL_ResumeAudioStreamDevice(stream)) {
        port_log("audio: cannot start the device (%s); the SPU runs on host time\n", SDL_GetError());
        SDL_DestroyAudioStream(stream);
        stream = NULL;
        spu_set_clock(SPU_CLOCK_HOST);
    }
    RT_LEAVE();
}
