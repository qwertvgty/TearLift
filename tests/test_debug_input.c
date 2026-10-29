#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/host/input_debug.h"

int main(void) {
    assert(input_debug_key(SDL_SCANCODE_ESCAPE) == 53);  // Minus
    assert(input_debug_key(SDL_SCANCODE_TAB) == 36);     // Plus
    assert(input_debug_key(SDL_SCANCODE_SPACE) == 34);   // X
    assert(input_debug_key(SDL_SCANCODE_Q) == 40);       // B
    assert(input_debug_key(SDL_SCANCODE_E) == 38);       // A
    assert(input_debug_key(SDL_SCANCODE_W) == -1);       // keep legacy movement
    assert(input_debug_mouse_button(SDL_BUTTON_LEFT) == 8);    // Y
    assert(input_debug_mouse_button(SDL_BUTTON_RIGHT) == 256); // ZL
    assert(input_debug_mouse_button(SDL_BUTTON_MIDDLE) == 0);
    assert(input_debug_mouse_axis(0, 0.016) == 0);
    assert(input_debug_mouse_axis(4, 0.016) == input_debug_mouse_axis(8, 0.032));
    assert(input_debug_mouse_axis(-4, 0.016) == -input_debug_mouse_axis(4, 0.016));
    assert(input_debug_mouse_axis(100000, 0.001) == 32767);
    assert(input_debug_mouse_axis(-100000, 0.001) == -32767);
    assert(input_debug_mouse_axis(4, 0) == 0);
    assert(input_debug_mouse_axis(NAN, 0.016) == 0);
    assert(!input_debug_mouse_recent(100, 0));
    assert(input_debug_mouse_recent(50000100, 100));
    assert(!input_debug_mouse_recent(50000101, 100));
    assert(!input_debug_mouse_recent(99, 100));
    InputDebugKeys keys = {0};
    assert(input_debug_key_change(&keys, SDL_SCANCODE_E, 38, 1) == 1);
    assert(input_debug_key_change(&keys, SDL_SCANCODE_E, 38, 1) == -1); // repeat
    assert(input_debug_key_change(&keys, SDL_SCANCODE_J, 38, 1) == -1); // alias held too
    assert(input_debug_key_change(&keys, SDL_SCANCODE_E, 38, 0) == -1); // J still holds A
    assert(input_debug_key_change(&keys, SDL_SCANCODE_J, 38, 0) == 0);
    assert(input_debug_key_change(&keys, SDL_SCANCODE_J, 38, 0) == -1);
    assert(input_debug_key_change(&keys, SDL_SCANCODE_COUNT, 38, 1) == -1);
    memset(&keys, 0, sizeof keys); // focus lost: no stuck logical keys
    assert(input_debug_key_change(&keys, SDL_SCANCODE_E, 38, 1) == 1);
    puts("Debug keyboard/mouse input: all checks passed");
    return 0;
}
