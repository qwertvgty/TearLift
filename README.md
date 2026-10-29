# TearLift

English | [简体中文](README.zh-CN.md)

TearLift is an unofficial native PC port of *The Legend of Zelda: Tears of the Kingdom* (Nintendo Switch, version 1.4.3) for Windows. It is not an emulator: game code is translated ahead of time (static recompilation, ARM64 → C → x86-64), and the Switch system and its NVN graphics API are replaced by the port's own implementations on Windows and Vulkan. Shaders and textures are converted for PC graphics cards when the game is built, not while you play.

> **This repository contains no game code, game assets, Nintendo SDK code, firmware or keys, and does not tell you where to obtain them.**
> It does nothing without a copy of the game you dumped yourself from your own console. Everything derived from the game (translated code, the compiled game, shaders and textures) is generated on your PC from your files and must stay there: do not share it or open issues asking for it.

TearLift is not affiliated with or endorsed by Nintendo. *The Legend of Zelda* and *Tears of the Kingdom* are trademarks of Nintendo; the names are used only to identify the game this port works with.

This project is independently maintained and based on [kuraeow/ultratears](https://github.com/kuraeow/ultratears).
It retains the upstream Git history, author attribution and original GPL license. It is neither a from-scratch port nor an official upstream release.
Credit for the original recompiler, runtime, Vulkan implementation and build pipeline belongs to the original author and contributors.

## Development priorities

- Prioritize rendering correctness, stability and reproducible performance improvements on Windows.
- Consider both NVIDIA and AMD.
- Improve keyboard/mouse and controller support, and selectively integrate reviewed and tested community PRs.
- No current development focus on Linux, Steam Deck or a new cross-platform launcher.

## Current status

| Area | Known status |
|---|---|
| AMD | Great Sky Island runs on a Radeon RX 6750 GRE 12 GB |
| NVIDIA | Testing pending |
| Performance | Current testing does not achieve stable 30 fps at 1080p; four-thread command recording remains experimental |
| Keyboard/mouse | Debug bindings and mouse-to-right-stick emulation implemented |

## Default keyboard/mouse bindings

| Keyboard/mouse input | Controller input |
|---|---|
| E | A |
| Q | B |
| Space | X |
| Left mouse button | Y |
| Right mouse button | ZL |
| Esc | − |
| Tab | + |
| Mouse movement | Right stick |

F1 captures/releases the mouse. Switching away from the window automatically releases capture and clears held inputs.

## Build methods

There is no downloadable game-filled `UltraTears.exe`. You download a builder that translates and compiles the game on your PC from your own extracted dump. There are two methods:

| Method | Who it is for | What to install |
|---|---|---|
| **Builder package from upstream Releases** | Players who want the upstream version | Nothing |
| **Current GitHub source** | Developers and people testing their own changes | Git and any maintained Python 3; remaining tools download automatically |

Both methods produce the same playable folder layout. The Releases package includes the build tools; a source build reuses local tools and automatically downloads missing dependencies.

> The current Releases link points to upstream UltraTears and does not include TearLift's changes. Use Method 2 to build TearLift.
> Executable and configuration filenames currently remain `UltraTears.exe` and `ultratears.cfg`.

## Requirements for either method

- Windows 10/11 x64 and a processor with AVX2 (Intel Haswell / AMD Ryzen or newer).
- A graphics card with a current Vulkan 1.3 driver and `VK_EXT_shader_object` (recent NVIDIA, AMD and Intel drivers).
- 16 GB of RAM.
- About **32 GB of free space for build output**, plus about 16 GB for the original game dump. The dump in the working folder is no longer needed after a successful build.
- **Your own extracted copy of Tears of the Kingdom 1.4.3**, as two ordinary folders:

  ```text
  game\
  ├── exefs\    main, subsdk0, sdk, main.npdm, ...
  └── romfs\    Actor\, Pack\, Shader\, ... (about 304,000 files, 16 GB)
  ```

  Obtain these folders yourself from your own console and copy of the game with update 1.4.3 installed. This project does not provide or point to extraction tools. `.nsp`, `.xci`, `.nca` and other encrypted packages do not work: TearLift decrypts nothing and reads no keys.

## Method 1 — use the builder package from upstream Releases

### 1. Download and extract the builder

Download this file from the [**Releases**](https://github.com/kuraeow/ultratears/releases) page:

```text
UltraTears-builder-<version>-windows-x86_64.zip
```

Extract the **entire archive** into its own folder, for example:

```text
D:\Games\TearLift
```

Do not run files from inside the ZIP. After extraction, `setup.bat` and the `deps`, `tools` and `game` folders should be next to one another.

### 2. Put the game dump in `game`

The layout should be:

```text
D:\Games\TearLift\
├── setup.bat
├── deps\
├── tools\
└── game\
    ├── exefs\
    └── romfs\
```

If the dump already lives elsewhere, there is no need to copy 16 GB: drag the folder containing `exefs` and `romfs` onto `setup.bat`.

### 3. Run the build

Double-click `setup.bat` and leave its window open. The builder runs these steps in order:

| Step | What it does | Approximate time on 8 cores |
|---|---|---|
| `check` | Check the folder structure and exact game version 1.4.3 | Seconds |
| `romfs` | Pack 304,000 files into `data\romfs\romfs.bin` | ~4 minutes |
| `lift` | Translate game code from the ExeFS to C | ~2 minutes |
| `compile` | Compile the game code and port into `UltraTears.exe` | ~15 minutes |
| `shaders` | Translate shaders for Vulkan | ~5 minutes |
| `textures` | Convert textures to BC7/BC1 | ~7 minutes |
| `finish` | Write the launch configuration | Seconds |

Times depend on the CPU, disk and antivirus. The build is complete only after the `Done` message. If the PC shuts down or the window closes, run `setup.bat` again; completed work is reused.

### 4. Start the game

Run:

```text
D:\Games\TearLift\UltraTears.exe
```

Do not move the EXE by itself. It needs `ultratears.cfg` and the `data` folder beside it. Saves are stored in `userdata`; keep that folder when moving or updating the game.

## Method 2 — build the current GitHub source

Use this method when changing code or building the latest commit. Install Git and Python 3, obtain this repository's source,
and put `exefs` and `romfs` in `game` at the repository root as shown above. Open PowerShell at the repository root and run:

```powershell
python tools/setup.py
```

This **one command performs the entire process**:

1. Create `dist` inside the repository.
2. Copy the current tracked source files there.
3. Install missing build tools into `dist\deps`: download Python, Clang and .NET from their respective publishers, and build the shader translator from this repository's source.
4. Find the dump: first in the output folder's `game`, then in the repository's `game`, and finally at the path last supplied with `--dump` for that output folder.
5. Build the complete game into `dist`.

If the selected output folder already has its own populated `game`, that copy takes priority. `--dump` always selects the dump explicitly and removes ambiguity.

The executable appears here:

```text
<repository>\dist\UltraTears.exe
```

To build directly into another folder:

```powershell
python tools/setup.py --output D:\Games\TearLift
```

The original dump remains in the repository's `game`, while the completed data is written to `D:\Games\TearLift\data`. If the dump is elsewhere, specify it explicitly:

```powershell
python tools/setup.py --output D:\Games\TearLift --dump D:\Dumps\TOTK-1.4.3
```

Run the same command again after changing source code: unchanged data and object files are reused. More options and implementation details are in [BUILDING.md](BUILDING.md).

## How the builder processes the dump

The original `exefs` and `romfs` are never run directly or moved. The builder reads them and creates a PC-ready copy inside the output folder:

```text
game\exefs  ──→ data\exefs + data\generated ──→ UltraTears.exe
game\romfs  ──→ data\romfs\romfs.bin
             ├→ data\shaders
             └→ data\textures
```

After the `Done` message, the game reads only `data` beside `UltraTears.exe`. You may remove the working copies of `game\exefs` and `game\romfs` if the original dump is backed up safely elsewhere. Never delete `data` from the built game.

The finished folder looks roughly like this:

```text
TearLift\
├── UltraTears.exe      ← run this file
├── ultratears.cfg      ← data path configuration
├── data\               ← built game data; do not delete
├── userdata\           ← saves
├── build\              ← intermediate files for fast rebuilds
└── deps\               ← build tools
```

## If the build stops with an error

- The `python` command is not found — install Python 3 from [python.org](https://www.python.org/downloads/windows/) and enable `Add python.exe to PATH`, or try `py tools/setup.py`.
- `exefs\main is not the one ... 1.4.3` — the ExeFS is not from version 1.4.3.
- `the RomFS is not the one ... 1.4.3` — the RomFS lacks the required update, is damaged or contains mods.
- `encrypted packages are not accepted` — `.nsp`, `.xci` or `.nca` was supplied instead of ordinary `exefs` and `romfs` folders.
- A Vulkan error or black window — update the graphics driver; Vulkan 1.3 with `VK_EXT_shader_object` is required.
- After correcting the cause, run the same command or `setup.bat` again. There is no need to repeat successful steps manually.

## Legal

- This repository and release packages contain no game material: no code (including code translated from the game), assets, shaders, textures, fonts, keys or firmware. Everything derived from the game is generated on the player's own PC from their own dump and stays there.
- TearLift decrypts nothing and reads no keys: it uses the ExeFS and RomFS already extracted by the player.
- No Nintendo SDK code or headers are used; the system and graphics layers are the port's own implementations.
- The source contains only facts needed to run: addresses, hashes and short instruction replacements.
- The icon (`assets/icon.ico`) is the original author's own drawing, not Nintendo artwork.
- TearLift is not affiliated with or endorsed by Nintendo; you need your own purchased copy of the game.

## License

TearLift is free software under the **GNU General Public License v3.0 or later** ([LICENSE](LICENSE)). Third-party code retains its own license: [third_party/README.md](third_party/README.md). The builder package additionally includes `THIRD-PARTY-NOTICES.md` and `licenses\` for the tools it carries.
