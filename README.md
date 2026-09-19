# 🌾 C Farming Simulator — 2D Farming Simulation in C

![Language](https://img.shields.io/badge/Language-C99-blue.svg)
![Graphics](https://img.shields.io/badge/Graphics-raylib-red.svg)
![Build](https://img.shields.io/badge/Build-Makefile%2FGCC-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg)

A modular 2D farming simulation game built from scratch in pure **C** using the **raylib** graphics engine. The game features multiple interactable world zones, custom crop growth timers, dynamic animal care logic, an in-game economy, inventory management, and persistent save data.

---

## 🎮 Gameplay & World Zones

The game environment is split into 4 distinct functional zones, alongside an active in-game economy:

* 🏡 **Home & Rest Zone:** A designated area where the player can rest to restore energy levels required for daily farm tasks.
* 🌱 **Crop Farming Zone:** Plant crops and monitor real-time growth. Interact with the **Water Tower** to irrigate crops and speed up growth cycles.
* 🐔 **Livestock Zone:** Care for farm animals by feeding them and maintaining clean animal pens.
* 🛒 **In-Game Shop:** Purchase animal food, seeds, and water refills using earned farm currency.

---

## 🛠️ Technical Highlights & Architecture

* **Pure C Memory & State Management:** Game states, inventory limits, and energy loops are implemented using C data structures and explicit state management.
* **Modular Codebase:** Clean separation between core execution (`main.c`), game logic (`game.c`), and rendering (`render.c`).
* **Cross-Platform Support:** The game can be built and run on both **Windows and Linux**.
* **Fullscreen Mode:** The game starts directly in fullscreen mode using the monitor's available resolution.
* **Persistent Game State:** Player progress, money, energy, food stock, pens, and plots can be saved and restored using a local save file.
* **raylib Graphics:** Rendering, textures, input handling, audio, and window management are implemented using raylib.

---

## 📂 Project Structure

```text
.
├── assets/        # Sprites, textures, fonts, icons, UI assets
├── include/       # Header files
├── lib/           # Bundled raylib libraries for Windows/MinGW
├── src/           # Game logic and rendering
├── .gitignore
├── Makefile       # Cross-platform build configuration
└── README.md
```

---

# 🚀 Building & Running

## 🪟 Windows

### Prerequisites

* GCC / MinGW
* `make`
* raylib libraries included in the `lib/` directory

The Windows build uses the raylib libraries bundled with the project.

### Build

From the project root:

```bash
make
```

### Run

```bash
./farming_game.exe
```

### Manual GCC Compilation

If you want to compile the game manually:

```bash
gcc -Wall -std=c99 -Iinclude src/main.c src/game.c src/render.c -o farming_game.exe -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
```

---

## 🐧 Linux

### Prerequisites

Linux requires a local installation of **raylib**, because the raylib library bundled in `lib/` was built for Windows/MinGW and cannot be linked directly with Linux GCC.

Install the required development tools:

```bash
sudo apt update
sudo apt install build-essential pkgconf
```

### Install raylib

Install raylib and its dependencies according to your Linux distribution.

For Ubuntu/Debian-based systems, if raylib is not already available through the configured repositories, install/build raylib locally so that the system provides:

```bash
pkg-config --modversion raylib
```

The project can then use the locally installed Linux raylib library.

### Verify raylib

Run:

```bash
pkg-config --modversion raylib
```

You should receive the installed raylib version.

You can also check the compiler and linker flags with:

```bash
pkg-config --cflags --libs raylib
```

### Build

From the project root:

```bash
make
```

### Run

```bash
./farming_game
```

### Manual Linux Compilation

If needed, the game can also be compiled directly with GCC:

```bash
gcc -Wall -std=c99 -Iinclude src/main.c src/game.c src/render.c -o farming_game $(pkg-config --cflags --libs raylib)
```

---

## 🔧 Linux Build Notes

The project uses different raylib libraries depending on the operating system:

```text
Windows
   ↓
Bundled raylib library
lib/libraylib.a
   ↓
MinGW/GCC
   ↓
farming_game.exe


Linux
   ↓
System/local raylib installation
   ↓
GCC
   ↓
farming_game
```

The Windows raylib library included in `lib/` should **not** be used for Linux builds because it is compiled for the Windows/MinGW environment.

---

## 🎮 Controls

The game uses keyboard and mouse controls for navigation, farming, animal interaction, inventory management, and shop interaction.

The game starts in fullscreen mode and uses the available monitor resolution.

---

## 💾 Save System

The game stores persistent progress in:

```text
savegame.dat
```

The save file contains game state such as:

* Money
* Energy
* Food stock
* Animal pens
* Farming plots

The game automatically saves the current state when the application is closed.

---

## 👤 Author

**Andreica Bianca Maria**

