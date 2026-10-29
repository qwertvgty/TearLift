# Sampled-attachment copy fallback — 2026-10-29

## Experiment Summary

**Change:** optional `TOTK_VK_FEEDBACK_COPY=1` snapshots sampled render targets before a draw on devices without
native feedback-loop support. The existing targets still receive the draw. Independent snapshots are cached per
in-flight slot and recording worker; idle slots retire unused copies. The SPIR-V entry-stage guard is also included.

**Setup**
- Hardware: Ryzen 5 7500F, RX 6750 GRE 12GB; Windows display driver 32.0.21045.16010.
- Game data: locally verified 1.4.3 dump; nothing derived from that dump is included in this commit.
- Seed: the runtime exposes no fixed game RNG seed; none is claimed.
- Baseline/new: same executable, copied identical userdata, shader-ahead disabled, one recording thread,
  audit windows 300..359 and 900..959; switch only `TOTK_VK_FEEDBACK_COPY` between 0 and 1.
- Shader binaries remain cached. These are correctness probes, not a controlled FPS benchmark.

**Results**

| Metric at presents 900..959 (title menu) | Baseline: copy off | New: copy on | Delta |
|---|---:|---:|---:|
| Draws/dispatches in audit | 18,974 | 18,973 | -1 |
| Draws/frame with a texture outside sampled layout | 23 | 0 | -23 |
| Such texture events/frame | 33 | 0 | -33 |

The menu captures visibly differ in lighting. These measurements do not establish correctness in all game scenes.
Later attempts at scripted `J` input reached the load-confirmation dialog, not a completed automated gameplay test.
The user subsequently confirmed that the displayed picture was normal and the black-screen issue was gone with the
fallback-enabled instance. That observation is user confirmation, not an automated traversal or a reference-image match.

**Tests**
- `tests/test_feedback_regions.c`: colours, separate depth/stencil aspects, all mip levels, array layers, 3D extents,
  dimensions clamped to one, invalid shapes/capacity, null arguments. Compiled with Clang, `-O2 -Wall -Wextra`.
- `tests/test_spv_stage.c`: main entry-stage checks and malformed/truncated module probes. Its optional local crash
  fixture is game-derived and is not committed.
- Incremental game build passed. `git diff --check` passed.
- Vulkan validation-layer testing and multi-threaded recording testing were not completed. No claim of passing them.

**Conclusion**
- Improvement: YES for the audited substitutions; the user reports the observed black rendering resolved.
- Keep as an opt-in compatibility experiment, not a default-on or fully validated renderer replacement.
- Statistical significance and long-term stability: not established (single paired menu run, no fixed seed).
- Reading a snapshot of earlier draws is not equivalent to every possible within-draw feedback dependency.
- Enable with `TOTK_VK_FEEDBACK_COPY=1`; the tested recording setting is `TOTK_VK_RECORD_THREADS=1`.
- No driver flags are forced. The native path remains unchanged on devices supporting it.

Private logs, screenshots, shader fixtures, and binaries remain under ignored `dev-local/`; none are committed.

## Reproducing the source-only tests

Use the project's CLANG64 compiler (its full path, or put its `bin` folder on PATH), from the checkout root:

```powershell
clang tests/test_spv_stage.c src/gfx/vkb_spv.c -O2 -Wall -Wextra -o dev-local/test_spv_stage.exe
.\dev-local\test_spv_stage.exe
clang tests/test_feedback_regions.c -Isrc/gfx -Isrc/gfx/nvn -Isrc/gfx/nvn/include -Isrc/gfx/compat -O2 -Wall -Wextra -o dev-local/test_feedback_regions.exe
.\dev-local\test_feedback_regions.exe
```
