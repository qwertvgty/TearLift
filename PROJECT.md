# Project scope and validation

UltraTears Windows is a working name for an independently maintained derivative of UltraTears.
It retains the upstream history, attribution and GPL license; see [UPSTREAM.md](UPSTREAM.md).

## Current priorities

1. Correct rendering and reliable execution on Windows.
2. Compatibility validation on both NVIDIA and AMD, using explicit GPU and driver records.
3. Reproducible frame-time improvements, including slow frames and stutter, not just peak FPS.
4. Practical keyboard/mouse and controller support.
5. Selective integration of reviewed community fixes, with provenance and regression checks.

Linux, Steam Deck and a new cross-platform launcher are not current development targets.
This does not require deleting existing platform abstractions or rejecting a harmless portability fix.

## Hardware validation matrix

| Target | Hardware | Status | Limitations / next evidence needed |
|---|---|---|---|
| Windows / AMD | Radeon RX 6750 GRE 12 GB | user verified that the tested black-screen scene renders with the attachment-copy fallback | not full-game validation; current testing does not achieve stable 30 fps at 1080p |
| Windows / NVIDIA | user's laptop, GPU model not yet recorded | test device available; this derivative not yet tested | record GPU, CPU, memory, OS, driver, selected graphics adapter and power mode before benchmarking |

Upstream's own hardware results are not automatically results for this modified build.
Neither vendor is described as fully supported until there is evidence for the specific configurations concerned.

## Performance acceptance

- Compare the baseline and candidate using the same dump, save, scene, camera, resolution and settings.
- Record the exact commit, GPU, driver, recording-thread count and feedback-loop path.
- On laptops, record AC/battery state, power profile, active GPU and render/display adapter routing when known.
- Separate shader/cache warm-up from steady-state measurements; repeat runs and record their duration.
- Report frame-time distributions and slow frames, not only average or maximum FPS.
- Check rendering and stability alongside speed; retain a rollback for driver-specific experiments.
- Do not claim stable 30/60 fps or improvement on both vendors from a single machine or short run.

## Community changes

Follow [PR.md](PR.md), with Windows and the validation matrix above as the current scope.
Keep one topic per change. Record the original PR URL, author, source commit, adaptations, tests and unresolved risks.
Passing a build alone does not establish runtime correctness. Crash guards must explain any behavior skipped.
Experimental renderer and threading changes stay opt-in until their correctness and performance are understood.

## Release boundary

No independent release is published yet. Inherited release links refer to upstream packages, not this derivative.
Game dumps, translated game code, built game executables, shaders/textures derived from the game, system fonts,
firmware, keys and user saves must not be published with this repository or its releases.
Review tracked files and history before any public push. Preserve third-party license notices.
