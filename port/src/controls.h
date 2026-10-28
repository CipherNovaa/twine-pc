#ifndef CONTROLS_H
#define CONTROLS_H

/* The keys and buttons of the game's actions (controls.c): twine.ini's [controls] block, the front end's
 * CONTROLS page (fixes/shell.c), the F1 overlay and the input (video.c). */
#include <SDL3/SDL.h>

/* what can be bound: a key (by its place: an SDL scancode), a mouse button (SDL's 1-5), a wheel step (0 up,
 * 1 down), a gamepad button (SDL's), a gamepad trigger (0 left, 1 right) */
enum { BIND_NONE, BIND_KEY, BIND_MOUSE, BIND_WHEEL, BIND_PAD, BIND_TRIGGER };
typedef struct {
    unsigned char kind;
    unsigned short code;
} Bind;

#define MAX_BINDS 4

/* the actions, in the order the CONTROLS page and the F1 overlay list them; CTL_PAD_FIRE on are the gamepad's
 * (the modern pad layout's: the classic one is the PS1 pad's own) */
enum {
    CTL_FIRE, CTL_AIM, CTL_USE, CTL_RELOAD, CTL_PUNCH, CTL_GRENADE, CTL_CROUCH, CTL_SPRINT, CTL_PREV_WEAPON,
    CTL_NEXT_WEAPON, CTL_LAST_WEAPON, CTL_FORWARD, CTL_BACK, CTL_STRAFE_LEFT, CTL_STRAFE_RIGHT, CTL_TURN_LEFT,
    CTL_TURN_RIGHT, CTL_LOOK_UP, CTL_LOOK_DOWN, CTL_MESSAGES,
    CTL_PAD_FIRE, CTL_PAD_AIM, CTL_PAD_USE, CTL_PAD_RELOAD, CTL_PAD_PUNCH, CTL_PAD_GRENADE, CTL_PAD_CROUCH,
    CTL_PAD_SPRINT, CTL_PAD_PREV_WEAPON, CTL_PAD_NEXT_WEAPON, CTL_PAD_LAST_WEAPON,
    CTL_COUNT
};
#define CTL_FIRST_PAD CTL_PAD_FIRE

extern Bind port_binds[CTL_COUNT][MAX_BINDS];

void port_controls_init(void);                  /* from twine.ini (the environment), problems logged */
const char *port_control_key(int c);            /* its twine.ini key: "fire", "pad_fire", ... */
const char *port_control_label(int c);          /* what the CONTROLS page calls it */
int port_control_known(const char *key);
/* its keys or buttons as text: for twine.ini (style 0), the F1 overlay (1), the game's font (2: ASCII, " / " between) */
void port_control_text(int c, int style, char *out, size_t n);
const char *port_bind_name(Bind b, int style);
int port_bind_parse(const char *name, int pad, Bind *out, int max);
int port_bind_reserved(Bind b); /* a key or button the port keeps for itself (Esc, Enter, F keys, Start, ...) */
/* the CONTROLS page's changes, kept in twine.ini at once: `b` the only one (add 0) or one more (add 1), taken
 * from any other action that had it */
void port_control_set(int c, Bind b, int add);
void port_control_default(int c);
void port_controls_reset(void);

#endif
