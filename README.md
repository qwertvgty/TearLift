# UltraTears Windows

**Windows-first community development, with NVIDIA and AMD both in scope.**

This is an independently maintained derivative of [UltraTears by kuraeow](https://github.com/kuraeow/ultratears),
not an official upstream release or a from-scratch port. The original Git history, author attribution and license
are retained. **UltraTears Windows is a working name.** Executable names and internal identifiers remain unchanged.

Our priorities are rendering correctness, stability, reproducible frame-time improvements and practical PC controls
on Windows. NVIDIA and AMD are equal validation targets; this is not a claim that every GPU or driver has been tested.
Linux and Steam Deck development are outside the current roadmap. Existing cross-platform code is not removed solely
because it is outside that roadmap.

See [中文说明](README.zh-CN.md), [project scope and hardware validation](PROJECT.md), and
[upstream provenance](UPSTREAM.md). There is no separate release of this derivative yet; upstream release downloads
do not include our local changes. Build this checkout to use them.

**UltraTears** is an unofficial builder: on your PC it turns your own copy of a Nintendo Switch game into a native
Windows program. It works with one game, *Tears of the Kingdom* version 1.4.3.
It is not an emulator: the program code of your copy is translated ahead of time (static
recompilation, ARM64 → C → x86-64), and the Switch system and its NVN graphics API are replaced by the port's own
implementations on Windows and Vulkan. Shaders and textures are converted for PC graphics cards when the game is built,
not while you play.

> **This repository contains no game code, no game assets, no Nintendo SDK code, no firmware and no keys, and it does
> not tell you where to get any of them.** It does nothing without a copy of the game you dumped yourself from your own
> console. Everything made from the game (the translated code, the compiled game, shaders, textures) is made on your
> PC from your files and must stay there: do not share it, and do not open issues asking for it.
>
> UltraTears is not affiliated with or endorsed by Nintendo. *The Legend of Zelda* and *Tears of the Kingdom* are
> trademarks of Nintendo; the name is used only to say which game this works with.

Русская версия (upstream documentation; not yet adapted to this derivative): [README.ru.md](README.ru.md).

## Status

Early and experimental. The current derivative has the following validation status:

| What | Result |
|---|---|
| Windows / AMD Radeon RX 6750 GRE 12 GB | user confirmed the tested black-screen scene renders with the optional attachment-copy fallback; not a full-game compatibility claim |
| Windows / NVIDIA laptop | planned test hardware; model, driver, rendering and performance results are not yet recorded |
| Frame rate | stable 30 fps at 1080p has not been achieved in the current user's testing; four recording threads are experimental |
| Keyboard / mouse | basic right-stick mouse emulation and debug bindings implemented; not a native mouse camera |
| Steam Deck / Linux | outside the current development and validation scope |
| Other PCs and graphics cards | not yet validated by this derivative |

The upstream README reported testing on a GeForce RTX 3060. That result does not validate our modified build on NVIDIA.

Known issues: hitches the first time an effect or place is seen (shader compilation); the controller layout follows the
Switch (A is the right face button); no settings menu yet (window size, resolution, upscaling come later).

## Choose how to build first

There is no downloadable game-filled `UltraTears.exe`. You download a builder, and it translates and compiles the game
on your PC from your own extracted dump. There are two ways to do that:

| Method | Who it is for | What you install |
|---|---|---|
| **Upstream builder package** | Users who specifically want the upstream version, without this derivative's changes | Nothing |
| **This source checkout** | Required for this derivative until a separate builder is published | Git and any maintained Python 3; the remaining tools download automatically |

Both methods produce the same playable folder. The Releases package already carries every build tool. The source build
uses the tools already on your PC and downloads only the missing ones from where they are published (Python from
python.org, Clang from MSYS2, .NET from Microsoft to build the shader translator from the same source), then performs
the same game build.

## Requirements for either method

- Windows 10/11 x64 and a processor with AVX2 (Intel Haswell / AMD Ryzen or newer). Older processors with SSE4.2
  (Intel Core 2nd and 3rd generation, AMD FX) work too: the builder detects the CPU and builds the game for it,
  somewhat slower.
- A graphics card with a current Vulkan 1.3 driver and `VK_EXT_shader_object` (recent NVIDIA, AMD and Intel drivers).
- At least **16 GB of RAM** and a graphics card with at least **4 GB of video memory**. A PC with 8 GB of RAM, or a
  GPU that shares the system memory (integrated graphics), is not supported for now.
- At least **40 GB free** on the drive of the game folder (an SSD is recommended): the build takes about 32 GB, and the
  texture cache grows while you play. The original game dump needs about 16 GB more; the dump in the working folder is
  no longer needed after a successful build.
- **Your own extracted copy of Tears of the Kingdom version 1.4.3** as two ordinary folders:

  ```text
  game\
  ├── exefs\    main, subsdk0, sdk, main.npdm, ...
  └── romfs\    Actor\, Pack\, Shader\, ... (about 304,000 files, 16 GB)
  ```

  You obtain these folders yourself from your own console and copy of the game with update 1.4.3 installed. This project
  does not provide or point to extraction tools. `.nsp`, `.xci`, `.nca` and other encrypted packages do not work:
  UltraTears decrypts nothing and reads no keys.

## Method 1 — upstream builder package (does not include this derivative)

The following package instructions are retained for upstream users. To build **UltraTears Windows**, use Method 2.

### 1. Download and extract the builder

Download this file from the [**Releases**](https://github.com/kuraeow/ultratears/releases) page:

```text
UltraTears-builder-<version>-windows-x86_64.zip
```

Extract the **entire archive** into its own folder, for example:

```text
D:\Games\UltraTears
```

Do not run files from inside the ZIP. After extraction, `setup.bat` and the `deps`, `tools` and `game` folders should
be next to one another.

### 2. Put the game dump in `game`

The layout should be:

```text
D:\Games\UltraTears\
├── setup.bat
├── deps\
├── tools\
└── game\
    ├── exefs\
    └── romfs\
```

If the dump already lives elsewhere, you do not need to copy 16 GB: drag the folder that contains `exefs` and `romfs`
onto `setup.bat`.

### 3. Run the build

Double-click `setup.bat` and leave its window open. The builder runs these steps in order:

| Step | What happens | Approximate time on 8 cores |
|---|---|---|
| `check` | The folder structure and exact game version 1.4.3 are checked | seconds |
| `romfs` | 304,000 files are packed into `data\romfs\romfs.bin` | ~4 min |
| `lift` | Game code from the ExeFS is translated to C | ~2 min |
| `compile` | The game code and port are compiled into `UltraTears.exe` | ~15 min |
| `shaders` | The shaders are translated for Vulkan | ~5 min |
| `textures` | The textures are converted to BC7/BC1 | ~7 min |
| `finish` | The launch configuration is written | seconds |

Times depend on the CPU, disk and antivirus. The build is complete only after the `Done` message. If the PC shuts down
or the window closes, run `setup.bat` again; completed work is reused.

### 4. Start the game

Run:

```text
D:\Games\UltraTears\UltraTears.exe
```

Do not move the EXE by itself. It needs `ultratears.cfg` and the `data` folder beside it. Saves are stored in
`userdata`; keep that folder when moving or updating the game.

## Method 2 — build the current GitHub source

Use this method for this derivative. Install Git and Python 3, open PowerShell in this checkout, then run:

```powershell
python tools/setup.py
```

Before running the command, put `exefs` and `romfs` in this checkout's `game` directory as shown above.
Alternatively, use an existing dump without copying it into this checkout:

```powershell
python tools/setup.py --dump D:\Dumps\TOTK-1.4.3
```

This **one command performs the entire process**:

1. Creates `dist` inside the repository.
2. Copies the current tracked source files there.
3. Installs the build tools this PC lacks into `dist\deps` (Python 3.14, Clang, and the shader translator built from
   this source); nothing comes from a release.
4. Finds the dump: first in the output folder's `game`, then in the repository's `game`, then the folder given with
   `--dump` last time for this output folder.
5. Builds the complete game into `dist`.

If the selected output folder already has its own populated `game`, that copy takes priority. `--dump` always selects
the dump explicitly and removes any ambiguity.

The executable appears here:

```text
ultratears\dist\UltraTears.exe
```

To build directly into another folder:

```powershell
python tools/setup.py --output D:\Games\UltraTears
```

The original dump remains in `ultratears\game`, while the completed data is written to
`D:\Games\UltraTears\data`. If the dump is elsewhere, name it explicitly:

```powershell
python tools/setup.py --output D:\Games\UltraTears --dump D:\Dumps\TOTK-1.4.3
```

Run the same command again after changing the source: unchanged data and object files are reused. More options and
implementation details are in [BUILDING.md](BUILDING.md).

## What the builder does with the dump

The original `exefs` and `romfs` are never run directly or moved. The builder reads them and creates a PC-ready copy
inside the output folder:

```text
game\exefs  ──→ data\exefs + data\generated ──→ UltraTears.exe
game\romfs  ──→ data\romfs\romfs.bin
             ├→ data\shaders
             └→ data\textures
```

After the `Done` message, the game reads only `data` beside `UltraTears.exe`. You may remove the working copies of
`game\exefs` and `game\romfs` if the original dump is backed up safely elsewhere. Never delete `data` from the built
game.

The finished folder looks roughly like this:

```text
UltraTears\
├── UltraTears.exe      ← run this file
├── ultratears.cfg      ← paths to the data
├── data\               ← built game data; do not delete
├── userdata\           ← saves
├── build\              ← intermediate files for fast rebuilds
└── deps\               ← build tools
```

## Controls and updates

The debug keyboard/mouse preset is: WASD moves; E (or J) = A/confirm; Q (or K) = B; Space (or I) = X;
left mouse button (or U) = Y; right mouse button (or left Alt) = ZL; Esc = −; Tab (or Enter) = +.
Mouse motion maps to the right stick, with arrow keys kept as an alternative. F1 captures/releases the mouse;
the first left click in an uncaptured game window captures it without attacking. Switching away releases capture
and clears held inputs. This is a basic stick emulation for debugging, not a native PC mouse camera.
Left Shift = ZR, T F G H = d-pad, Z / C = stick buttons, and F11 or Alt+Enter toggles full screen.
Xbox, DualSense, Switch Pro and other controllers work through SDL3; the face-button layout follows the Switch
(A is the right button). Mouse/keyboard and the physical controller remain separate input sources.

To update a Releases build, extract the new builder package over the old folder and run `setup.bat` again. Keep
`userdata` to preserve saves. For a Git build, run `git pull`, then repeat the same `python tools/setup.py` command and
options.

> **Steam Deck and Linux are not supported yet.** The Windows build does not work through Proton because its memory
> access method is incompatible with `winevulkan`. Native Linux and Steam Deck development are outside this
> derivative's current roadmap.

## If the build stops with an error

- The `python` command is not found — install Python 3 from
  [python.org](https://www.python.org/downloads/windows/) and enable `Add python.exe to PATH`, or try
  `py tools/setup.py`.
- `exefs\main is not the one ... 1.4.3` — the ExeFS is not from version 1.4.3.
- `the RomFS is not the one ... 1.4.3` — the RomFS lacks the required update, is damaged or contains mods.
- `encrypted packages are not accepted` — `.nsp`, `.xci` or `.nca` was supplied instead of ordinary `exefs` and
  `romfs` folders.
- A Vulkan error or black window — update the graphics driver; Vulkan 1.3 with `VK_EXT_shader_object` is required.
- After correcting the cause, run the same command or `setup.bat` again. You do not need to repeat successful steps
  manually.

## Legal

- Nothing of the game is in this repository or in a release: no code (also none translated from the game), no assets,
  shaders, textures, fonts, keys or firmware. Everything derived from the game is made on the player's PC from the
  player's own dump and stays there.
- UltraTears decrypts nothing and reads no keys: it takes the ExeFS and RomFS the player has already extracted.
- No Nintendo SDK code or headers are used; the system and graphics layers are the port's own implementations.
- Only the facts needed to run are in the source: addresses, hashes, short instruction replacements.
- The icon (`assets/icon.ico`) is the author's own drawing, not Nintendo artwork.
- UltraTears is not affiliated with or endorsed by Nintendo; you need your own purchased copy of the game.

## License

UltraTears is free software under the **GNU General Public License v3.0 or later** ([LICENSE](LICENSE)). Third-party
code keeps its own license: [third_party/README.md](third_party/README.md); the builder package adds
`THIRD-PARTY-NOTICES.md` and `licenses\` for the tools it carries.
