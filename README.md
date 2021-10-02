# Eagle [![License](https://img.shields.io/github/license/RaskiTech/Eagle.svg)](https://github.com/RaskiTech/Eagle/blob/master/LICENSE)

![Eagle](Eagle/Assets/EagleLogo.png?raw=true "Eagle")

Eagle is a 2D graphics rendering engine that is currently available for Windows. Eagle can be used to make any sorts of desktop applications, including games. Currently Eagle uses OpenGL to rended it's graphics.

The goal for the Eagle engine is to have many things done through code, while being easy and fast to deal with. You should easily be able to make a static scene or an interactive application. Once python is implemented it can be used to run python programs with an interface.

## Start using Eagle
To start using Eagle, make sure you have git installed. If not, you can get it here: https://git-scm.com/downloads.
Open Command prompt and run `git clone --recursive https://github.com/RaskiTech/Eagle Eagle`.
If the repository was previously cloned without the `--recursive` flag, it doesn't have the required submodules. You get them by going into the folder and running `git submodule update --init`.

After the installation go into the folder and inside Startup-folder click your os setup file. Now you can open the Eagle.sln and start developing.

## Features include
- OpenGL renderer
- An interactive UI library
- Serialization
- ECS
- Audio
- Particle system
- Profiling
- Logging

## Possible upcoming features:
- More editor tools
- Support for Mac
- Support for other rendering APIs
- Python for a scripting language


More information and plans on the Eagle trello page: https://trello.com/b/Q6aYRSKR/eagle-engine

[![Trello](https://img.shields.io/badge/%40RaskiTech--blue.svg?style=social&logo=Trello)](https://trello.com/b/Q6aYRSKR/eagle-engine)

