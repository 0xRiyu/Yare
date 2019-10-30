# YareZo  
Cross platform Vulkan Renderer  
Collaborative project between Resoona (Drew Cornfield) & PhaseRush  

Make sure VulkanSDK is installed and CmakeLists can find your SDK path.  
Set and environment variable named VULKAN_SDK to your vulkan root .../1.1.x.x/point-here

You will need a C++ 17 compatible compiler for this project.  
Currently builds the spdlog header, the MinGW-W64 compiler needs to be installed with posix threads not win32  
This will cause compile times to be lengthy, spdlog will soon be moved to a static lib  

Potential Errors:

- Could not find Vulkan

    Error: `CMake Error at C:/path/to/bin/cmake/win/share/cmake-3.14/Modules/FindPackageHandleStandardArgs.cmake:137 (message):
    Could NOT find Vulkan (missing: Vulkan_INCLUDE_DIR)`
  
    Solution: Add `Vulkan_INCLUDE_DIR` as a system environmental variable with the value `C:\VulkanSDK\1.1.114.0\Include\vulkan`,
    or VulkanSDK's include directory.
  
