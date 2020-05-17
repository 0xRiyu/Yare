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
    Engine/Graphics/Mesh.cpp
    Engine/Graphics/Model.cpp
    Engine/Graphics/Material.cpp
    Engine/Graphics/Camera/Camera.cpp
    Engine/Graphics/Renderers/ForwardRenderer.cpp

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
    Engine/Core/Memory.h
    Engine/Core/YzWindow.h
    Engine/Core/YzCamera.h
    Engine/Core/DataStructures.h
    Engine/Core/YzInputHandler.h

    # Graphics
    Engine/Graphics/Mesh.h
    Engine/Graphics/Model.h
    Engine/Graphics/Material.h
    Engine/Graphics/RenderCommand.h
    Engine/Graphics/Camera/Camera.h
    Engine/Graphics/Renderers/ForwardRenderer.h
    Engine/Graphics/Renderers/YzRenderer.h

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
    PUBLIC Libs/tinyobjloader
    PUBLIC Libs/spdlog/include
    PUBLIC Libs/glfw/include)

#--------------------------------------------------------------------
# Find static libraries that are required
# These must not be relative paths since they are linked from the CMAKE_CURRENT_BINARY_DIR
#--------------------------------------------------------------------
find_package(Vulkan REQUIRED)

#--------------------------------------------------------------------
# Link the libraries
#--------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    PUBLIC Vulkan::Vulkan)

if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
          target_link_libraries(${PROJECT_NAME}
                PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/glfw/Windows/lib-vc2019/glfw3.lib
            PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/spdlog/msvc/x64-Release/lib/spdlog.lib)

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(${PROJECT_NAME}
        PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/glfw/Windows/lib-mingw-w64/libglfw3.a
            PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/spdlog/mingw/libspdlog.a)
    endif()
endif()

if (UNIX)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/glfw/Unix/lib-gcc/libglfw3.a dl -lpthread -lm
        PUBLIC ${CMAKE_SOURCE_DIR}/YareZo/Libs/spdlog/Unix/gcc/libspdlog.a)
endif()
