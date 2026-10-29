# TearLift

[English](README.md) | 简体中文

TearLift 是《塞尔达传说：王国之泪》（Nintendo Switch，版本 1.4.3）面向 Windows 的非官方原生 PC 移植版。它不是模拟器：游戏程序代码会被提前翻译（静态重编译，ARM64 → C → x86-64），Switch 系统及其 NVN 图形 API 由该移植版在 Windows 和 Vulkan 上的自有实现替代。着色器和纹理在构建游戏时针对 PC 显卡转换，而不是在你游玩时转换。

> **本仓库不包含任何游戏代码、游戏资源、Nintendo SDK 代码、固件或密钥，也不会告诉你从哪里获取它们。**
> 如果没有你自己从自己的主机中转储的游戏副本，它无法做任何事。所有由游戏生成的内容（翻译后的代码、编译后的游戏、着色器、纹理）都在你的 PC 上由你的文件生成，并且必须留在那里：不要分享它们，也不要开 issue 索要它们。

TearLift 与 Nintendo 无关联，也未获其认可。《塞尔达传说》和《王国之泪》是 Nintendo 的商标；此名称仅用于说明本移植版适用于哪款游戏。

本项目基于 [kuraeow/ultratears](https://github.com/kuraeow/ultratears) 独立维护，
保留上游 Git 历史、作者信息和原有 GPL 许可证，并非从零编写的移植，也不是上游官方发行版。
原项目的重编译器、运行时、Vulkan 实现和构建流程应归功于原作者及贡献者。

## 开发方向

- 优先 Windows 上的画面正确性、稳定性和可复现的性能改进。
- 同时考虑 NVIDIA、AMD。
- 改善键鼠和手柄操作，选择性吸收经过审查、测试的社区 PR。
- 暂不投入 Linux、Steam Deck 或新的跨平台启动器开发。

## 当前状态

| 项目 | 已知情况 |
|---|---|
| AMD | RX 6750 GRE 12 GB 上成功运行初始空岛 |
| NVIDIA | 等待后续测试 |
| 性能 | 当前测试尚不能在 1080p 稳定 30 帧；四线程录制仍为实验设置 |
| 键鼠 | 已实现调试映射与鼠标模拟右摇杆 |

## 默认键鼠映射

| 键鼠输入 | 手柄按键 |
|---|---|
| E | A |
| Q | B |
| 空格 | X |
| 鼠标左键 | Y |
| 鼠标右键 | ZL |
| Esc | − |
| Tab | + |
| 鼠标移动 | 右摇杆 |

F1 捕获/释放鼠标；切出窗口自动释放并清空按键。

## 构建方式

没有可下载的、包含游戏的 `UltraTears.exe`。你下载的是一个构建器，它会用你自己的提取转储在你的 PC 上翻译并编译游戏。有两种方式：

| 方法 | 适合谁 | 需要安装什么 |
|---|---|---|
| **上游 Releases 中的构建器包** | 希望使用上游版本的玩家 | 无需安装 |
| **当前 GitHub 源码** | 开发者和测试自己改动的人 | Git 和任意仍在维护的 Python 3；其余工具会自动下载 |

两种方法生成的可游玩文件夹布局相同。Releases 包已携带构建工具；源码构建会复用本机工具，并自动下载缺失的依赖。

> 当前 Releases 链接指向上游 UltraTears，不包含 TearLift 的改动。构建 TearLift 请使用方法 2。
> 当前可执行文件和配置文件仍沿用 `UltraTears.exe` 与 `ultratears.cfg`。

## 两种方法的共同要求

- Windows 10/11 x64，以及支持 AVX2 的处理器（Intel Haswell / AMD Ryzen 或更新）。
- 显卡需有当前 Vulkan 1.3 驱动并支持 `VK_EXT_shader_object`（较新的 NVIDIA、AMD 和 Intel 驱动）。
- 16 GB 内存。
- 约 **32 GB 可用空间用于构建输出**，另需约 16 GB 用于原始游戏转储。成功构建后，工作文件夹中的原始转储不再需要。
- **你自己的、已提取的《王国之泪》1.4.3 副本**，为两个普通文件夹：

  ```text
  game\
  ├── exefs\    main, subsdk0, sdk, main.npdm, ...
  └── romfs\    Actor\, Pack\, Shader\, ... （约 304,000 个文件，16 GB）
  ```

  你需要自己从自己的主机和已安装 1.4.3 更新的游戏副本中获取这些文件夹。本项目不提供也不指向提取工具。`.nsp`、`.xci`、`.nca` 及其他加密包不可用：TearLift 不解密任何内容，也不读取密钥。

## 方法 1 — 使用上游 Releases 中的构建器包

### 1. 下载并解压构建器

从 [**Releases**](https://github.com/kuraeow/ultratears/releases) 页面下载此文件：

```text
UltraTears-builder-<version>-windows-x86_64.zip
```

将**整个压缩包**解压到它自己的文件夹中，例如：

```text
D:\Games\TearLift
```

不要从 ZIP 内部运行文件。解压后，`setup.bat` 与 `deps`、`tools`、`game` 文件夹应位于同一层。

### 2. 将游戏转储放入 `game`

布局应为：

```text
D:\Games\TearLift\
├── setup.bat
├── deps\
├── tools\
└── game\
    ├── exefs\
    └── romfs\
```

如果转储已在别处，无需复制 16 GB：将包含 `exefs` 和 `romfs` 的文件夹拖到 `setup.bat` 上。

### 3. 运行构建

双击 `setup.bat`，并保持其窗口打开。构建器会按顺序运行以下步骤：

| 步骤 | 发生什么 | 在 8 核上的大致时间 |
|---|---|---|
| `check` | 检查文件夹结构以及确切的游戏版本 1.4.3 | 数秒 |
| `romfs` | 将 304,000 个文件打包进 `data\romfs\romfs.bin` | 约 4 分钟 |
| `lift` | 将 ExeFS 中的游戏代码翻译为 C | 约 2 分钟 |
| `compile` | 将游戏代码和移植层编译为 `UltraTears.exe` | 约 15 分钟 |
| `shaders` | 将着色器转换为 Vulkan 用 | 约 5 分钟 |
| `textures` | 将纹理转换为 BC7/BC1 | 约 7 分钟 |
| `finish` | 写入启动配置 | 数秒 |

耗时取决于 CPU、磁盘和杀毒软件。只有出现 `Done` 消息后构建才算完成。如果电脑关机或窗口关闭，请再次运行 `setup.bat`；已完成的工作会被复用。

### 4. 启动游戏

运行：

```text
D:\Games\TearLift\UltraTears.exe
```

不要单独移动 EXE。它需要旁边的 `ultratears.cfg` 和 `data` 文件夹。存档存储在 `userdata` 中；移动或更新游戏时请保留该文件夹。

## 方法 2 — 构建当前 GitHub 源码

当你需要修改代码或构建最新提交时使用此方法。安装 Git 和 Python 3，获取本仓库源码，
按上文所示将 `exefs` 和 `romfs` 放入仓库根目录下的 `game`，然后在仓库根目录打开 PowerShell，运行：

```powershell
python tools/setup.py
```

这**一条命令执行整个过程**：

1. 在仓库内创建 `dist`。
2. 将当前受跟踪的源文件复制到那里。
3. 将缺失的构建工具安装到 `dist\deps`：从各自发布源下载 Python、Clang、.NET，并用本仓库源码构建着色器转换器。
4. 查找转储：先在输出文件夹的 `game` 中查找，然后在仓库的 `game` 中查找，最后尝试该输出文件夹上次通过 `--dump` 指定的路径。
5. 将完整游戏构建到 `dist`。

如果所选输出文件夹已有自己的、内容完整的 `game`，则该副本优先。`--dump` 始终显式选择转储，消除歧义。

可执行文件出现在这里：

```text
<仓库目录>\dist\UltraTears.exe
```

要直接构建到另一个文件夹：

```powershell
python tools/setup.py --output D:\Games\TearLift
```

原始转储仍留在仓库的 `game` 中，而完成的数据会写入 `D:\Games\TearLift\data`。如果转储在别处，请显式指定：

```powershell
python tools/setup.py --output D:\Games\TearLift --dump D:\Dumps\TOTK-1.4.3
```

更改源码后再次运行同一命令：未变的数据和目标文件会被复用。更多选项和实现细节见 [BUILDING.md](BUILDING.md)。

## 构建器如何处理转储

原始 `exefs` 和 `romfs` 从不被直接运行或移动。构建器读取它们，并在输出文件夹内创建可直接供 PC 使用的副本：

```text
game\exefs  ──→ data\exefs + data\generated ──→ UltraTears.exe
game\romfs  ──→ data\romfs\romfs.bin
             ├→ data\shaders
             └→ data\textures
```

出现 `Done` 消息后，游戏只读取 `UltraTears.exe` 旁边的 `data`。如果原始转储已在别处安全备份，你可以删除 `game\exefs` 和 `game\romfs` 的工作副本。切勿删除已构建游戏中的 `data`。

完成后的文件夹大致如下：

```text
TearLift\
├── UltraTears.exe      ← 运行此文件
├── ultratears.cfg      ← 数据路径配置
├── data\               ← 构建后的游戏数据；请勿删除
├── userdata\           ← 存档
├── build\              ← 用于快速重建的中间文件
└── deps\               ← 构建工具
```

## 如果构建因错误停止

- 找不到 `python` 命令 — 从 [python.org](https://www.python.org/downloads/windows/) 安装 Python 3，并启用 `Add python.exe to PATH`，或尝试 `py tools/setup.py`。
- `exefs\main is not the one ... 1.4.3` — 该 ExeFS 不是 1.4.3 版本。
- `the RomFS is not the one ... 1.4.3` — RomFS 缺少所需更新、已损坏或包含 mod。
- `encrypted packages are not accepted` — 提供的是 `.nsp`、`.xci` 或 `.nca`，而不是普通的 `exefs` 和 `romfs` 文件夹。
- Vulkan 错误或黑屏窗口 — 更新显卡驱动；需要支持 `VK_EXT_shader_object` 的 Vulkan 1.3。
- 修正原因后，再次运行同一命令或 `setup.bat`。无需手动重复已成功的步骤。

## 法律

- 本仓库或发布包中不含游戏的任何内容：没有代码（也没有从游戏翻译而来的代码）、没有资源、着色器、纹理、字体、密钥或固件。所有由游戏衍生的内容都在玩家自己的 PC 上、由玩家自己的转储生成，并保留在那里。
- TearLift 不解密任何内容，也不读取密钥：它使用玩家已提取的 ExeFS 和 RomFS。
- 不使用 Nintendo SDK 代码或头文件；系统和图形层是移植项目自己的实现。
- 源码中只包含运行所需的事实：地址、哈希、短指令替换。
- 图标（`assets/icon.ico`）是原作者自己的绘图，不是 Nintendo 的美术作品。
- TearLift 与 Nintendo 无关联，也未获其认可；你需要自己购买的游戏副本。

## 许可证

TearLift 是自由软件，采用 **GNU 通用公共许可证 v3.0 或更高版本**（[LICENSE](LICENSE)）。第三方代码保留其自己的许可证：[third_party/README.md](third_party/README.md)；构建器包为其携带的工具额外提供 `THIRD-PARTY-NOTICES.md` 和 `licenses\`。
