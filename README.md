![YareZoEngineGraphic](https://i.imgur.com/y3som7P.png)

# YareZo [![Build status](https://ci.appveyor.com/api/projects/status/ibi6um09v5j03068?svg=true)](https://ci.appveyor.com/project/Resoona/yarezo)  
A Vulkan Rendering Engine  
A Collaborative project between Resoona & PhaseRush   

This project is under very slow development :)

Cross platform support for Linux/Windows (sometimes)  

### Dependencies
- Vulkan SDK - https://www.lunarg.com/vulkan-sdk/  
- C++17 compiler (MSVC, MinGW-w64, GCC)  
- CMake  

### External Libraries
See LICENCE
- GLM
- GLFW
- Spdlog
- IMGUI
- TinyObjLoader
- STB (stb_image)

### Build
Windows - build.bat - Requires Ninja, vcvarsall.bat, and CMake in system path.  
build.bat release run regen  
  
Linux - buildlinux.sh - Requires g++ compiler, Ninja, and CMake.  
./buildlinux.sh release run regen  
  
### Screenshots  
![05.23.2020](/Screenshots/05.23.2020-sandbox.png "Multiple models rendered with a skybox pipeline")
