# SHADE POWER

**Youtube Video Link** https://www.youtube.com/watch?v=2GYfDKOkVBg

## 🎮 Game Overview

**Shade Power** is a 2D horror-action game built from scratch using C++ and SDL2. You play as a tormented soul trapped at the sealed Gates of Heaven. To earn salvation, you must violently slay the surrounding demons using spectral projectiles.

**Endless Survival Design:**
The game has an **infinite difficulty scaling system**. Instead of a traditional win state, the objective is to survive as long as possible against increasingly large waves of enemies to achieve a **High Score**. This arcade-style loop places focus on replayability and the strategic management of limited resources (Ammo/Health).

The project demonstrates a custom **Entity Component System (ECS)** architecture, advanced C++ memory management, and binary data persistence with encryption.

## ✨ Features Implemented

### Gameplay Mechanics
* **8-Directional Movement:** Smooth, normalized movement using Delta Time (dt) for consistent speed across different framerates.
* **Combat System:** Directional shooting mechanics with projectile velocity calculations.
* **Ammo & Reloading:** Strategic resource management. The player has 10 shots and must press **'R'** to reload.
* **Health & Power-ups:**
    * Visual Hearts HUD updating in real-time.
    * **Potions:** Enemies have a chance to drop health potions (Hearts) which heal the player on collision.
* **Enemy AI:** Demons track the player's position using vector math and flip sprites to face the correct direction.
* **Wave System:** Infinite difficulty scaling; clearing a wave spawns a larger number of enemies in the next round.

### Visuals & Audio
* **Particle System:** Custom-coded blood splash effect using randomized velocity vectors on enemy death.
* **Animations:** State-based animation system (Idle vs. Run) with sprite flipping.
* **Dynamic UI:** Real-time Score, High Score, Health Hearts, and Ammo Bar (visual yellow indicators).
* **Audio:** Background music and sound effects (Shooting) implemented via SDL_Mixer.

### Technical & Architecture
* **ECS Architecture:** A custom Entity-Component-System decoupling Logic (Systems) from Data (Components).
* **Data Persistence (Advanced):** High scores are saved to a binary file (`shade_save.dat`).
* **Security (Advanced):** Implements **XOR Encryption** to secure save data against basic tampering.
* **Profiling & Optimization:**
    * Memory pooling simulation for projectiles.
    * Verified flat memory usage graph (no leaks) during gameplay loops.
* **Collision System:** Advanced AABB collision supporting multiple layers (Colliders vs. Map Objects).

## 🕹️ Controls

| Key | Action |
| :--- | :--- |
| **W, A, S, D** | Move Player |
| **SPACE** | Shoot Projectile |
| **R** | **Reload** (In-Game) / **Restart** (Game Over Screen) |
| **ENTER** | Start Game (Menu) |
| **X / ALT + F4** | Quit Application |

## 🛠️ Architectural Highlights

The core architecture was refactored from standard tutorials to ensure modern C++ compliance and "Separation of Concerns."

1.  **Decoupled Logic:** Input, Animation, and Physics are handled in dedicated `System` classes (`CollisionSystem`, `AnimationSystem`), keeping the main loop clean.
2.  **Robust Input Handling (`KeyboardEvent.h`):** Input logic is separated from visual logic. The input component only flags state changes; the Animation System decides what to draw.
3.  **Binary I/O with XOR Encryption:**
    * Instead of plain text, data is written in binary.
    * Logic: `EncryptedScore = RealScore ^ Key`.
    * This ensures the high score cannot be easily edited in a text editor.

## 🚀 How to Build & Run

### Dependencies
* **Visual Studio 2022**
* **SDL2 Library** (Core, Image, Mixer, TTF)

### Build Instructions
1.  Open `SDL2_Game.sln` in Visual Studio.
2.  Select **Release** configuration and **x64** Architecture.
3.  Ensure the SDL2 "include" and "lib" paths are correctly linked in Project Properties.
4.  Build the solution.
5.  **Run:** Navigate to the `bin/` folder (or `x64/Release/`) and run `SDL2_Game.exe`.
    * *Note: Ensure the `assets/` folder and `SDL2.dll` files are in the same directory as the executable.*

## 🤖 AI Declaration

In accordance with the module assessment guidelines, Artificial Intelligence (Gemini) was used in the following capacity:
* **Debugging:** To assist in resolving linker errors (LNK2005) and C++ syntax warnings (C26495).
* **Mathematical Explanation:** Used to explain the vector math required for the particle system velocity randomization.
* **Code Structure:** Used to discuss the pros/cons of placing Potions in `groupMap` vs `groupColliders`.

## 📖 References

**Tutorials & Core Structure:**
* Let's Make Games (2017) *SDL2/C++ Tutorial*. [YouTube]. Available at: https://www.youtube.com/watch?v=QQzAHcojEKg (Accessed: 9 November 2025).

**Assets:**
1. (Craftpix.net, n.d.) Reference List: Craftpix.net (n.d.) Free Pixel Art Tiny Hero Sprites. [Online] Available at: https://craftpix.net/freebies/free-pixel-art-tiny-hero-sprites/ (Accessed: 5 November 2025).
2. (Buckley, n.d.) Reference List: Buckley, S. (n.d.) Vanguard. [Online] Free Stock Music. Available at: https://www.free-stock-music.com/scott-buckley-vanguard.html (Accessed: 30 December 2025).
3. (Pixabay, n.d.) Reference List: Pixabay (n.d.) Laser Sound Effect (45816). [Online] Available at: https://pixabay.com/sound-effects/laser-45816/ (Accessed: 30 December 2025).
4. (Matteson, 2011) Reference List: Matteson, S. (2011) Open Sans. [Computer Typeface]. Google Fonts. Available at: https://fonts.google.com/specimen/Open+Sans (Accessed: 25 December 2025).

## 📬 Contact

**Rahil Mavani**
rahil.mavani@mail.bcu.ac.uk
