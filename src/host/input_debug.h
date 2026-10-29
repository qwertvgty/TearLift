// Small keyboard/mouse debug preset. Values are the legacy virtual key codes consumed by overrides_input.c.
#pragma once
#include <stdint.h>
#include <math.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_mouse.h>

static inline int input_debug_key(SDL_Scancode s) {
    switch (s) {
    case SDL_SCANCODE_ESCAPE: return 53; // Minus
    case SDL_SCANCODE_TAB: return 36;    // Plus
    case SDL_SCANCODE_SPACE: return 34;  // X
    case SDL_SCANCODE_Q: return 40;      // B
    case SDL_SCANCODE_E: return 38;      // A
    default: return -1;
    }
}
static inline uint64_t input_debug_mouse_button(uint8_t button) {
    return button == SDL_BUTTON_LEFT ? 8 : button == SDL_BUTTON_RIGHT ? 256 : 0; // Y / ZL
}
static inline int32_t input_debug_mouse_axis(double delta, double seconds) {
    if (!isfinite(delta) || !isfinite(seconds) || seconds <= 0) return 0;
    if (seconds < 0.001) seconds = 0.001;
    double v = delta * 32.0 / seconds; // basic velocity-to-stick mapping, not a native mouse camera
    return v > 32767 ? 32767 : v < -32767 ? -32767 : (int32_t)v;
}
static inline int input_debug_mouse_recent(uint64_t now, uint64_t sampled) {
    return sampled && now >= sampled && now - sampled <= 50000000; // don't retain motion if the event loop stalls
}

typedef struct { uint8_t down[SDL_SCANCODE_COUNT]; uint16_t refs[128]; } InputDebugKeys;
// Return the new logical state (0/1), or -1 when it did not change. Aliases such as E/J must not release each other.
static inline int input_debug_key_change(InputDebugKeys *s, SDL_Scancode physical, int logical, int down) {
    if ((unsigned)physical >= SDL_SCANCODE_COUNT || (unsigned)logical >= 128) return -1;
    down = !!down;
    if (s->down[physical] == down) return -1;
    s->down[physical] = (uint8_t)down;
    if (down) return s->refs[logical]++ == 0 ? 1 : -1;
    if (s->refs[logical]) s->refs[logical]--;
    return s->refs[logical] == 0 ? 0 : -1;
}
