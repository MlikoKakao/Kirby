# Kirby's Adventure - Project 2

## Student Information

- Student ID(s): E24148434,E24115083 
- Name(s): 馬泰, 陸澤宇
- Completion / Last Updated Time: **2026-06-01**

## Development and Execution Environment

- Language: C++
- Framework: Qt Widgets
- IDE: Qt Creator
- Project file: `Kirby.pro`
- Main classes/files:
  - `main.cpp`, `mainwindow.cpp/.h`
  - `gamewidget.cpp/.h` - game loop, stage loading, drawing, camera, collisions, game states
  - `stagedata.cpp/.h` - stage obstacles, enemies, environment sprites
  - `player.cpp/.h` - Kirby movement, states, HP, abilities, animation
  - `enemy.cpp/.h` - enemy types and AI behavior
  - `projectile.cpp/.h` - Kirby and enemy projectiles
  - `item.cpp/.h` - Maxim Tomato and 1UP items
  - `assets.cpp/.h`, `resources.qrc` - image loading and Qt resource mapping

Tested/created for Qt Creator using a Qt Widgets project. The game window is fixed at **1620 x 1080** pixels.

## How to Run

1. Open **Qt Creator**.
2. Choose **Open Project**.
3. Select `Kirby.pro`.
4. Configure the project with a Qt kit that supports Qt Widgets.
5. Build the project.
6. Run the project from Qt Creator.

## Basic gameplay controls

- Enter - Start game / confirm Game Over option
- Left / Right - Move Kirby
- Up / W - Jump; press again in the air to hover; enter portal/goal when touching it
- Down - Squat; swallow enemy in Mouthful state
- X - Inhale, spit star, exhale air while hovering, or use current ability
- V - Store/unequip current ability and return Kirby to Normal
- Up / Down on Game Over screen - Switch Continue / Quit

## Menu controls

C - Open/close ability menu

Left/Right - Select Fire or Spark in the menu

Enter - Equip selected stored ability

V during gameplay - Store/unequip current ability and return Kirby to Normal

V inside ability menu - Permanently drop selected stored ability
