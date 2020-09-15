# PwnAdventure 3 Cheats

The repository contains the source code for PwnAdventure 3 cheats.
The build target is Linux, which produces a shared object that can be loaded with `LD_PRELOAD`.

## Features:

- [x] Fly hack - TODO: In air movement
- [x] Speed hack
- [ ] Teleportation
- [ ] Money hack
- [ ] ESP

## Building

This repository currently has no external requirements.

Compile on Linux with cmake:

```bash
mkdir build
cd build
cmake ..
cmake --build
```

## Usage

`libCheats.so` would be produced and can be loaded to the game with.

```bash
LD_PRELOAD=libCheats.so ./PwnAdventure3-Linux-Shipping 
```
