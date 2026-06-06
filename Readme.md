# Kirby-Inspired Qt Game

A 2D Kirby-inspired platformer made in C++ with Qt Widgets.  
The project includes custom player movement, enemies, abilities, projectiles, multiple stages, collision handling, health/lives, items, menus, and a Windows playable build.

## Download

The latest Windows build is available in the [Releases](../../releases) section.

Download the ZIP, extract it, and run the executable.

## About the Project

This project was created as a university C++/Qt game project.  
Most of the game systems were implemented from scratch, including the main game loop, player states, enemy behavior, collision logic, stage loading, abilities, projectiles, items, HUD, and menu logic.

The game is inspired by classic Kirby platformers, but the implementation, structure, and gameplay systems were built manually using Qt Widgets and QPainter.

## Features

- 2D platforming movement
- Jumping, hovering, crouching, inhaling, swallowing, and spitting
- Fire and Spark abilities
- Ability storage menu
- Multiple enemy types with different behaviors
- Projectiles and enemy attacks
- Health, lives, damage, invincibility frames
- Collectible items such as Maxim Tomato and 1UP
- Multiple stages with platforms, terrain, hazards, and a portal/goal
- Start menu, game over screen, continue/quit logic
- Custom sprite loading through Qt resources

## Controls

### Gameplay

| Key | Action |
|---|---|
| Enter | Start game / confirm Game Over option |
| Left / Right | Move |
| Up / W | Jump, hover, or enter portal/goal |
| Down | Squat / swallow enemy in Mouthful state |
| X | Inhale, spit star, exhale air, or use current ability |
| V | Store or unequip current ability |
| C | Open or close ability menu |

### Ability Menu

| Key | Action |
|---|---|
| Left / Right | Select stored ability |
| Enter | Equip selected ability |
| V | Drop selected stored ability |

### Game Over Screen

| Key | Action |
|---|---|
| Up / Down | Switch Continue / Quit |
| Enter | Confirm selection |

## Development Environment

- Language: C++
- Framework: Qt Widgets
- Rendering: QPainter
- Game loop: QTimer
- IDE: Qt Creator
- Project file: `Kirby.pro`
- Window size: 1620 × 1080

## Main Files

| File | Purpose |
|---|---|
| `main.cpp` | Application entry point |
| `mainwindow.cpp/.h` | Main application window |
| `gamewidget.cpp/.h` | Game loop, drawing, camera, collisions, stages, game states |
| `player.cpp/.h` | Player movement, states, HP, abilities, animation |
| `enemy.cpp/.h` | Enemy types, behavior, attacks |
| `projectile.cpp/.h` | Kirby and enemy projectiles |
| `item.cpp/.h` | Collectible items |
| `stagedata.cpp/.h` | Stage layout, obstacles, enemies, environment |
| `assets.cpp/.h` | Sprite and asset loading |
| `resources.qrc` | Qt resource mapping |

## How to Build from Source

1. Open **Qt Creator**.
2. Select **Open Project**.
3. Open `Kirby.pro`.
4. Configure the project with a Qt kit that supports Qt Widgets.
5. Build the project.
6. Run it from Qt Creator.

## Status

The game is playable and includes the main required mechanics, enemies, stages, UI screens, and bonus ability-storage functionality.
