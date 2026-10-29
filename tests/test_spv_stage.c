#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/gfx/vkb_spv.h"

int main(int argc, char **argv) {
    uint32_t w[] = {0x07230203, 0x00010300, 0, 2, 0,
                    (5u << 16) | 15, 0, 1, 0x6e69616d, 0};
    assert(vkb_spv_has_main_stage(w, 10, 0));
    assert(!vkb_spv_has_main_stage(w, 10, 4));
    w[6] = 4;
    assert(vkb_spv_has_main_stage(w, 10, 4));
    w[6] = 5;
    assert(vkb_spv_has_main_stage(w, 10, 5));
    assert(!vkb_spv_has_main_stage(w, 10, 0));
    assert(!vkb_spv_has_main_stage(w, 9, 5));
    assert(!vkb_spv_has_main_stage(w, 4, 5));
    w[0] = 0;
    assert(!vkb_spv_has_main_stage(w, 10, 5));
    w[0] = 0x07230203;
    w[5] = 15;
    assert(!vkb_spv_has_main_stage(w, 10, 5));
    w[5] = (5u << 16) | 15;
    w[8] = 0x72656874;
    assert(!vkb_spv_has_main_stage(w, 10, 5));
    if (argc == 2) {   // optional local crash fixture; game-derived shaders must not be committed
        FILE *f = fopen(argv[1], "rb");
        assert(f);
        fseek(f, 0, SEEK_END);
        long bytes = ftell(f);
        assert(bytes >= 20 && !(bytes & 3));
        rewind(f);
        uint32_t *spv = malloc((size_t)bytes);
        assert(spv && fread(spv, 1, (size_t)bytes, f) == (size_t)bytes);
        fclose(f);
        assert(vkb_spv_has_main_stage(spv, (size_t)bytes / 4, 0));
        assert(!vkb_spv_has_main_stage(spv, (size_t)bytes / 4, 4));
        free(spv);
    }
    puts("SPIR-V stage guard: all checks passed");
    return 0;
}
