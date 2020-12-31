#Eagle [![License](https://img.shields.io/github/license/RaskiTech/Eagle.svg)](https://github.com/RaskiTech/Eagle/blob/master/LICENSE)
Eagle is a 2D graphics rendering engine that is currently only awailable for windows. Eagle is in it's early-state, but it's being worked on reguraly. Eagle uses OpenGL to rended it's graphics.

## Start using Eagle
### Copy with git
To start using Eagle, make sure you have git installed. If not, you can get it here: https://git-scm.com/downloads.
After that open Command prompt and type `git clone --recursive https://github.com/RaskiTech/Eagle`.
If the repository was cloned without the `--recursive` flag, it doesn't have the required submodules. You can go in the project folder and use `git submodule update --init` to fix this.

### Copy from github
You can also copy Eagle with github without having to install git. Just click the green Code button at the top and from the dropdown the zip button. Unzip the dowloaded zip-file and you are good to go.


## Planned features in no order:
- An interactive UI library
- Particle system
- Entity component system
- Serialization
- Collision detection
- Physics
- Support for Mac, Linux, Android and iOS
- Support for other rendering APIs (DirectX, Vulkan)
- A scripting language
- Audio system