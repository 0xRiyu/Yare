#--------------------------------------------------------------------
# Set Lib Source Files
#--------------------------------------------------------------------
set (LIB_SOURCES
        # imgui
        Libs/imgui/imgui/imgui.cpp
        Libs/imgui/imgui/imgui_draw.cpp
        Libs/imgui/imgui/imgui_demo.cpp
        Libs/imgui/imgui/imgui_widgets.cpp
)

#--------------------------------------------------------------------
# Set Lib Header Files
#--------------------------------------------------------------------
set (LIB_HEADERS
        Libs/stb/stb/stb_image.h
        Libs/tinyobjloader/tinyobjloader/tiny_obj_loader.h
)

#--------------------------------------------------------------------
# Set YareZo Source Files
#--------------------------------------------------------------------
set (YAREZO_SOURCES
        # Application
        Engine/Application/Application.cpp

        # Graphics
        Engine/Graphics/Vulkan.cpp
        Engine/Graphics/Camera/Camera.cpp

        # Handlers
        Engine/Handlers/KeyHandler.cpp
        Engine/Handlers/MouseHandler.cpp

        # Utilities
        Engine/Utilities/YzLogger.cpp
        Engine/Utilities/IOHelper.cpp
        
        # Windows OS Specific
        Engine/Platform/Windows/GlfwWindow.cpp

        # Vulkan
        Engine/Platform/Vulkan/Vk_Image.cpp
        Engine/Platform/Vulkan/Vk_Buffer.cpp
        Engine/Platform/Vulkan/Vk_Shader.cpp
        Engine/Platform/Vulkan/Vk_Devices.cpp
        Engine/Platform/Vulkan/Vk_Instance.cpp
        Engine/Platform/Vulkan/Vk_Renderer.cpp
        Engine/Platform/Vulkan/Vk_Pipeline.cpp
        Engine/Platform/Vulkan/Vk_Swapchain.cpp
        Engine/Platform/Vulkan/Vk_Utilities.cpp
        Engine/Platform/Vulkan/Vk_Semaphore.cpp
        Engine/Platform/Vulkan/Vk_Renderpass.cpp
        Engine/Platform/Vulkan/Vk_Framebuffer.cpp
        Engine/Platform/Vulkan/Vk_CommandPool.cpp
        Engine/Platform/Vulkan/Vk_DescriptorSet.cpp
        Engine/Platform/Vulkan/Vk_CommandBuffer.cpp
)

#--------------------------------------------------------------------
# Set YareZo Header Files
#--------------------------------------------------------------------
set (YAREZO_HEADERS
        # API
        Engine/YareZo.h

        # Application
        Engine/Application/EntryPoint.h
        Engine/Application/Application.h

        # Core
        Engine/Core/Yzh.h
        Engine/Core/Glfw.h
        Engine/Core/Core.h
        Engine/Core/YzWindow.h
        Engine/Core/YzCamera.h
        Engine/Core/DataStructures.h
        Engine/Core/YzInputHandler.h

        # Graphics
        Engine/Graphics/Vulkan.h
        Engine/Graphics/Camera/Camera.h

        # Handlers
        Engine/Handlers/KeyHandler.h
        Engine/Handlers/MouseHandler.h

        # Utilities
        Engine/Utilities/YzLogger.h
        Engine/Utilities/IOHelper.h
        Engine/Utilities/T_Singleton.h

        # Windows OS Specific
        Engine/Platform/Windows/GlfwWindow.h

        # Vulkan
        Engine/Platform/Vulkan/Vk.h
        Engine/Platform/Vulkan/Vk_Image.h
        Engine/Platform/Vulkan/Vk_Buffer.h
        Engine/Platform/Vulkan/Vk_Shader.h
        Engine/Platform/Vulkan/Vk_Devices.h
        Engine/Platform/Vulkan/Vk_Pipeline.h
        Engine/Platform/Vulkan/Vk_Instance.h
        Engine/Platform/Vulkan/Vk_Renderer.h
        Engine/Platform/Vulkan/Vk_Swapchain.h
        Engine/Platform/Vulkan/Vk_Utilities.h
        Engine/Platform/Vulkan/Vk_Semaphore.h
        Engine/Platform/Vulkan/Vk_Renderpass.h
        Engine/Platform/Vulkan/Vk_Framebuffer.h
        Engine/Platform/Vulkan/Vk_CommandPool.h
        Engine/Platform/Vulkan/Vk_DescriptorSet.h
        Engine/Platform/Vulkan/Vk_CommandBuffer.h
)

#--------------------------------------------------------------------
# Define the library & create an alias
#--------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${YAREZO_SOURCES} ${YAREZO_HEADERS}
                                   ${LIB_SOURCES}    ${LIB_HEADERS})
add_library(YareZo::Engine ALIAS ${PROJECT_NAME})

#--------------------------------------------------------------------
# Set include directories for all build configurations
#--------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine
        PUBLIC Libs/glm
        PUBLIC Libs/imgui
        PUBLIC Libs/stb
        PUBLIC Libs/tinyobjloader)

#--------------------------------------------------------------------
# Find static libraries that are required
# These must not be relative paths since they are linked from the CMAKE_CURRENT_BINARY_DIR
#--------------------------------------------------------------------
find_package(Vulkan REQUIRED)

if(NOT TARGET spdlog)

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Yarezo/Libs/spdlog/x64-Debug/include)
        set(spdlog_DIR "${CMAKE_SOURCE_DIR}/YareZo/Libs/spdlog/x64-Debug/lib/cmake/spdlog")
        find_package(spdlog REQUIRED)
    endif()

    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/spdlog/x64-Release/include)
        set(spdlog_DIR "${CMAKE_SOURCE_DIR}/YareZo/Libs/spdlog/x64-Release/lib/cmake/spdlog")
        find_package(spdlog REQUIRED)
    endif()

endif()

#--------------------------------------------------------------------
# Link the static libraries
#--------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
        PUBLIC Vulkan::Vulkan
        PRIVATE spdlog::spdlog)

#--------------------------------------------------------------------
# Windows Specific configurations
# These must not be relative paths since they are linked from the CMAKE_CURRENT_BINARY_DIR
#--------------------------------------------------------------------
if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_include_directories(${PROJECT_NAME}
            PUBLIC  ${CMAKE_SOURCE_DIR}/YareZo/Libs/glfw-vc2019/include)

        target_link_libraries(${PROJECT_NAME}
            PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/glfw-vc2019/lib/glfw3.lib)

    elseif (CMAKE_CXX_COMPILER_I} STREQUAL "MinGW")

        target_include_directories(${PROJECT_NAME}
            PUBLIC  ${CMAKE_SOURCE_DIR}/YareZo/Libs/glfw-mingw/include)

        target_link_libraries(${PROJECT_NAME}
            PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/glfw-mingw/lib/glfw3.lib)
    endif()
endif()


