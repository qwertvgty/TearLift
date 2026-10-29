# Clean one/four recording-thread comparison

## Experiment Summary
**Change:** one recording thread vs four, with the feedback-copy fallback enabled on both. Renderer code unchanged.

**Setup**
- Build: 76ad0e3; Ryzen 5 7500F, RX 6750 GRE 12GB, display driver 32.0.21045.16010.
- Separate headless runs, 1920x1080 presented-frame capture, same initial save/profile snapshot and camera position.
- Seed: game exposes no fixed RNG seed; dynamic wind/lighting and scheduling are not deterministic.
- Shader-ahead disabled; warmed binaries retained; no concurrent game process.
- The same E presses at presents 300, 600 and 900 load the save. Both captures at 2400 show the Great Sky Island
  scene, not merely the title screen or the load-confirmation dialog.
- No audit or tracing variables present. Startup logs explicitly report `recording threads: 1` and `recording threads: 4`.
- Six 300-frame windows ending at 2700, 3000, 3300, 3600, 3900 and 4200 (1800 frames) are compared.

## Results
Values below are **arithmetic means of the six per-window percentiles**, NOT global run percentiles or average FPS.

| Metric (ms) | One thread | Four threads | Delta |
|---|---:|---:|---:|
| Frame-time window P50 mean | 37.118 | 25.947 | -11.171 |
| Frame-time window P95 mean | 41.232 | 60.048 | +18.816 |
| Submit+present window P50 mean | 33.917 | 20.077 | -13.840 |
| Submit+present window P95 mean | 37.848 | 23.538 | -14.310 |
| Logged fatal/access-violation count | 0 | 0 | 0 |

Visual inspection of both same-position captures finds no large black-rendering regression. Wind/grass and lighting
are not pixel-identical; this is not a full visual-fidelity certification. Headless timing is not a windowed 1080p
FPS guarantee, and submit+present is not a pure CPU timing. Native-front-end GPU=0 placeholders are not GPU-load data.

## Invalid preliminary run and launcher bug
The initial requested-four-thread run actually logged one thread, forced by `TOTK_WATCH`. Those preliminary results
are not a one-vs-four comparison and are excluded above. A native C probe reproduced why: on the current Windows
PowerShell host, `[Environment]::SetEnvironmentVariable(name, $null, 'Process')` creates an empty environment entry.
Native `getenv` returns a non-NULL empty string, enabling the renderer's presence-based diagnostic/thread override.
`Remove-Item Env:name` actually removes the entry. The launcher now uses removal for cleared/previously absent
variables, restores non-empty and empty caller values, and tests presence rather than truthiness in DryRun.
`tests/test_launch_debug.ps1` covers this without launching a game.

## Conclusion
- Submit/recording-path improvement: YES, about 41% reduction in the compared window-P50 mean.
- Stable 30 FPS: NOT established. Tail frame times are worse in the four-thread run.
- Long-term stability/statistical significance: not established; one run per configuration, no fixed seed.
- Keep one thread as the conservative launcher default; retain four as an experimental manual comparison.
- Next optimization target: frame pacing/translation queue waits and long frames, followed by feedback-copy/barrier
  reductions with correctness checks, rather than increasing recorder count blindly.
- Original saves and the user's interactive game were not controlled by the benchmark; tests used private copies.

Local logs, native environment probe, JSON window metrics and BMP captures remain in ignored `dev-local/perf-20261030`.
No game-derived artifacts are committed. Skill used: experiment-eval.
