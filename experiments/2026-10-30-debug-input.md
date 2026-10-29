# Debug keyboard/mouse preset and clean performance-launch preparation

## Experiment Summary
**Change:** E=A, Esc=Minus, Tab=Plus, Space=X, Q=B; relative mouse motion=right stick,
mouse left=Y, mouse right=ZL. J/K/I/U and arrow keys remain fallback controls.

**Setup**
- Source baseline: 56f1931. Existing game kept running, not controlled or restarted.
- Seed: not applicable to deterministic source-only input tests; no fixed game RNG seed claimed.
- The keyboard/mouse executable was built separately so the existing process and executable were not replaced.
- Mouse capture is opt-in through F1 or the first left click; first click captures only.
- Focus loss releases mouse input and resets keyboard alias reference counts. Physical pad and mouse are independent.
- Mouse velocity maps to a bounded stick value, expires after a stalled event loop, and is not a native PC camera.

## Results
| Check | Baseline | New | Delta |
|---|---|---|---|
| Requested keyboard debug preset | Different fixed mapping | Five requested keyboard mappings covered by passing tests | Implemented |
| Mouse-to-right-stick/buttons | No window-event implementation | Compiles and helper tests pass | Implemented; in-game interaction not yet verified |
| Alias hold/release (e.g. E/J) | Not tested for new aliases | Repeat, overlapping hold, release and focus-reset tests pass | Regression coverage added |
| Launch environment | Audit/script variables can be inherited | Dry run confirms clearing diagnostics and restoring caller environment | Prepared, no game launched |
| In-game FPS / p95 frame time | Prior diagnostic run forced one recorder | Not measured for this build | No performance improvement claimed |

The source-only test was first run before the new helper existed and failed to compile. After implementation,
`tests/test_debug_input.c` passes with Clang `-O2 -Wall -Wextra`. It covers key assignments, mouse buttons,
axis direction/range/time normalization, invalid timing, expiration and aliased-key transitions.
Prior SPIR-V-stage and feedback-copy-region regression tests are retained. The alternate game link passes.

## Performance plan
The immediate comparison should use the same saved position and camera, warmed shader binaries, and no audit,
scripted input, image captures or concurrent game instances. `tools/launch_debug.ps1` provides explicit one-thread
and four-thread choices, keeps timing statistics and disables competing shader-ahead work for the measurement.
It does not change persistent environment settings or close an existing game. Four-thread copying remains
experimental until picture and stability are checked; no speedup is inferred from a dry run.

## Conclusion
- Input implementation and deterministic tests: YES; actual mouse capture/game interactions need a manual run.
- FPS improvement: NOT EVALUATED. No benchmark was run alongside the user's existing game.
- Keep the input work separate from the previously committed Vulkan fix and from future renderer optimization.
- Use skill: experiment-eval; results above distinguish implementation/tests from unperformed gameplay benchmarks.
