# Origin and attribution

This project is derived from [kuraeow/ultratears](https://github.com/kuraeow/ultratears).
The original static recompiler, runtime, Vulkan graphics implementation and build pipeline are upstream work.
This derivative is independently maintained, is not an official upstream release, and is not a from-scratch port.

## Starting point

- Upstream development commit used by the existing local work: `8213475`.
- The source copy was created from local `dev` at `6c4cccb` on 2026-10-30.
- Existing local changes retained in full:
  - `56f1931`: sampled-attachment copy fallback and shader-stage guard.
  - `76ad0e3`: debug keyboard/mouse bindings and clean launch profiles.
  - `6c4cccb`: removal of empty diagnostic environment overrides.
- Original commits, author identities and copyright notices are retained.
- The upstream remote is for source comparison and selective integration, not automatic publication.

The project name is **TearLift**. Executable names, configuration keys and internal identifiers
remain UltraTears for now. Branding does not change the origin or license of the source.

## License and community contributions

The original [LICENSE](LICENSE) is unchanged. The upstream README identifies the project as GPL v3.0 or later.
Third-party components retain their own licenses; see [third_party/README.md](third_party/README.md).
For each future community integration, retain original authorship and document the PR URL, exact commit,
local adaptations and validation. Existing experiments do not imply that those upstream PRs have been merged.

## Repository contents

The project repository is [qwertvgty/TearLift](https://github.com/qwertvgty/TearLift).
Only project source, documentation and project assets belong here. Game dumps, generated game code, compiled game
executables, caches, system fonts, firmware, keys, user saves and private experiment artifacts are not distributed.
