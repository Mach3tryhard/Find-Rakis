# Find Rakis

# FIND RAKIS (ENG)

A procedural space exploration and survival simulation where you pilot a lone spaceship through a vast, randomly generated universe.
Your mission is to locate the legendary planet **Rakis** while managing critical resources and surviving the harsh physics of deep space.
You must balance your fuel reserves, energy levels, and ore collection while navigating gravitational wells of stars, asteroids, and black holes.
The universe is finite and dangerous—every run creates a unique galaxy layout.

## CONTROLS

* **UP Arrow**: Engage Main Thrusters (Consumes Fuel).
* **Left / Right Arrows**: Rotate Ship.
* **C**: Fire Weapons (Consumes Energy).
* **Z**: Refuel (Hold near a Star).
* **X**: Engage Hyperdrive (Consumes Ore, requires momentum).
* **ESC**: Pause Game / Open Menu.

## RESOURCES USED IN THE PROJECT

### 1. Visuals & Textures

**Procedural Generation:**
Unlike traditional games using sprite sheets, most of the visuals in *Find Rakis* are generated procedurally via C++ code:
* **Planets:** Generated using Perlin Noise and Dithering algorithms (implemented in `Generator.cpp`) to create unique surface textures for every celestial body.
* **Spaceship:** Rendered using `sf::ConvexShape` geometry calculations to allow for dynamic rotation and scaling without pixelation.
* **Starfield:** A dynamic, multi-layered parallax system (`Starfield.cpp`) that simulates depth and movement speed.

**Static Assets:**
* **Window Icon:** `icon.png` (Screenshot from game)

### 2. Audio Effects

* Thruster Sound (`engine3.wav`): [https://sfxr.me/]
* Hyperdrive Sound (`powerUp.wav`): [https://sfxr.me/]
* Laser Fire (`laser.wav`): [https://sfxr.me/]
* Explosion (`explosion3.wav`): [https://sfxr.me/]
* Refueling (`refuel.wav`): [https://sfxr.me/]
* Soundtrack (`ambient.mp3`): [https://pixabay.com/music/ambient-space-ambient-cinematic-442834/]

### 3. Fonts

**UI Text:**
* **JetBrains Mono** (`jetbrains.ttf`):
  https://www.jetbrains.com/lp/mono/
  *Used for the Heads-Up Display (HUD), Ship Computer Log, and Menus.*

### 4. Documentation Used

**Official Documentation:**
* SFML 3.0 Documentation:
  https://www.sfml-dev.org/documentation/3.0.0/

**Tutorials & References:**
* SFML Particle System Tutorial (Adapted for `ParticleSystem.h`):
  https://www.sfml-dev.org/tutorials/3.0/graphics/vertex-array/#example-particle-system

### 5. Technologies Used

* **C++ 23**: Core programming language.
* **SFML 3.0.2**: Graphics, Windowing, System, and Audio modules.
* **CMake**: Build system and dependency management (FetchContent).

### 6. Design Patterns & Architecture

To ensure code quality and extensibility, the project implements several Object-Oriented Design Patterns:

* **Game Manager Pattern:** Encapsulates the entire game state, window management, and reset logic (`GameManager.h`).
* **Factory Pattern:** Used in `Factory.h` to dynamically generate Celestial bodies (Stars, Planets, Black holes, Asteroids) based on biome types.
* **Command Pattern:** Used in the Menu system to decouple UI buttons from their specific actions (Start Game, Exit, Regenerate).
* **Smart Pointers:** `std::unique_ptr` is used for memory management of the Universe and Starfield to prevent memory leaks during world regeneration.