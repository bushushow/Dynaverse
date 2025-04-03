# Dynaverse

## Overview

**Dynaverse** is a C++-based RPG simulation prototype built as a proof-of-concept for the Realm Shaper class, part of the fictional H.U.SLAND game. It combines two tree structures: a **custom binary tree** (Shaper Tree) for ranking players, and a **self-balancing AVL tree** (World Tree) for representing the dynamic game world.

This simulation features player duels, territory access mechanics, item collection, realm crafting, and real-time updates through file and terminal output.

---

## Features

- 🌳 **Shaper Tree (Custom Binary Tree)**
  - Array-based complete binary tree
  - Players are ranked by insertion order and duel performance
  - Duel system with honour point mechanics
  - Node reordering and deletion based on duel results

- 🌍 **World Tree (AVL Tree)**
  - Pointer-based AVL tree for world map
  - Dynamic insertion, deletion, and balancing
  - Isle overcrowding handling with automatic destruction
  - Item enrichment with post-order and pre-order traversal logic

- 🎮 **Gameplay Simulation**
  - Duel logs and access logs are processed from file input
  - Honour point system for tracking player success
  - Realm crafting by accessing or creating new Isles

- 🧠 **Single Player Demo**
  - Interactive terminal mode with duels driven by trivia and logic questions
  - Real-time Shaper Tree and World Tree display
  - Honour tracking and item-based crafting via menu

---

## Installation

```bash
git clone https://github.com/bushushow/Dynaverse.git
cd Dynaverse
g++ -std=c++11 *.cpp *.h -o HUSLAND
```

## Usage

After successful compilation, run the game with the following command:

```bash
./HUSLAND initial_world.txt initial_realm_shapers.txt access.log duels.log \
  current_isles.txt current_map.txt current_realm_shapers.txt current_shaper_tree.txt
```
