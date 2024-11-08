# Eagle Engine [![License](https://img.shields.io/github/license/RaskiTech/Eagle.svg)](https://github.com/RaskiTech/Eagle/blob/master/LICENSE)

![Eagle](Build/Assets/Icon.png?raw=true "Eagle")
![Engine](Build/Assets/Engine.png)

Eagle engine is a 2D graphics rendering engine that is currently only available for Windows. It can be used to make any sorts of desktop applications, including games.

The goal for the Eagle engine is to have many things done through code, while being easy and fast to deal with. You should easily be able to make a static scene or an interactive application. The engine has python support, so python programs can use it as an interface.

## Start using Eagle engine
To start using the Eagle engine, make sure you have git installed. If not, you can get it here: https://git-scm.com/downloads.
Open Command prompt and run `git clone --recursive https://github.com/RaskiTech/Eagle Eagle`.
If the repository was previously cloned without the `--recursive` flag, it doesn't have the required submodules. You get them by going into the folder and running `git submodule update --init`.

After the installation go into the folder, and inside Macros-folder click your os setup file. Now you can open the Eagle.sln and start developing.
Eagle engine docs can be found in docs.html in the main directory.

## Features include but aren't limited to
- Python
- Custom OpenGL renderer
- An interactive UI library (w/text)
- Serialization
- ECS
- Audio
- Particle system
- Profiling
- Logging
- Editor & Runtime

More information and plans on the Eagle engine trello page: https://trello.com/b/Q6aYRSKR/eagle-engine

[![Trello](https://img.shields.io/badge/%40RaskiTech--blue.svg?style=social&logo=Trello)](https://trello.com/b/Q6aYRSKR/eagle-engine)

