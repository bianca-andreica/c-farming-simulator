
# 🌾 C Farming Simulator — 2D Farming Simulation in C

![Language](https://img.shields.io/badge/Language-C99-blue.svg)
![Graphics](https://img.shields.io/badge/Graphics-raylib-red.svg)
![Build](https://img.shields.io/badge/Build-Makefile%2FMinGW-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)

A modular 2D farming simulation game built from scratch in pure **C** using the **raylib** graphics engine. The game features multiple interactable world zones, custom crop growth timers, dynamic animal care logic, and an in-game economy.

---

## 🎮 Gameplay & World Zones

The game environment is split into 4 distinct functional zones, alongside an active economy:

* 🏡 **Home & Rest Zone:** A designated area where the player can rest to restore energy levels required for daily farm tasks.
* 🌱 **Crop Farming Zone:** Plant crops and monitor real-time growth. Interact with the **Water Tower** to irrigate crops and speed up growth cycles.
* 🐔 **Livestock Zone:** Care for farm animals by feeding them and maintaining clean animal pens.
* 🛒 **In-Game Shop:** Purchase animal food, seeds, and water refills using earned farm currency.

---

## 🛠️ Technical Highlights & Architecture

* **Pure C Memory & State Management:** Game states, inventory limits, and energy loops are built using low-level dynamic/static memory structures without high-level engine abstractions.
* **Modular Codebase:** Clean separation between core execution (`main.c`), game state/logic (`game.c`), and hardware-accelerated rendering pipelines (`render.c`).
* **Standalone Build Dependencies:** Bundled with local raylib headers and static libraries (`lib/`) to ensure fast setup without complex environment configurations.

---

## 📂 Project Structure

```text
├── src/           # Game logic, player movement, rendering loop (.c)
├── include/       # Module headers and raylib declarations (.h)
├── lib/           # Static libraries (raylib for Windows/MinGW)
├── assets/        # Sprites, textures, and UI elements (.png)
├── Makefile       # Windows build automation script
└── README.md

---

## 🚀 Building & Running

### Prerequisites

* **MinGW / GCC** (C99 standard compiler)

### Compilation

Build the project using the included `Makefile`:

```bash
make
./farming_game.exe

```

Or compile manually via GCC:

```bash
gcc -Wall -std=c99 -Iinclude src/main.c src/game.c src/render.c -o farming_game.exe -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

```

---

## 👤 Author

* **Andreica Bianca Maria**

```

```
