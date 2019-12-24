![YareZoEngineGraphic](https://i.imgur.com/y3som7P.png)

# YareZo [![Build status](https://ci.appveyor.com/api/projects/status/ibi6um09v5j03068?svg=true)](https://ci.appveyor.com/project/Resoona/yarezo)  
Vulkan Renderer  
Collaborative project between Resoona (Drew Cornfield) & PhaseRush  

This project is a WIP.

### Dependencies
- Vulkan SDK Installed
- C++17 compatible compiler (MSVC, MinGW)

### External Libraries
- GLM
- GLFW
- SPDLOG

### Potential Issues:

- Could not find Vulkan

    Error: `CMake Error at C:/path/to/bin/cmake/win/share/cmake-3.14/Modules/FindPackageHandleStandardArgs.cmake:137 (message):
    Could NOT find Vulkan (missing: Vulkan_INCLUDE_DIR)`
  
    Solution: Add `Vulkan_INCLUDE_DIR` as a system environmental variable with the value `<VulkanInstallDir>\x.x.xxx.x\Include\vulkan`,
    or VulkanSDK's include directory.
