#--------------------------------------------------------------------
# Set Lib Source Files
#--------------------------------------------------------------------
set (LIB_SOURCES
    # imgui
    Libs/imgui/imgui/imgui.cpp
    Libs/imgui/imgui/imgui_draw.cpp
    Libs/imgui/imgui/imgui_demo.cpp
    Libs/imgui/imgui/imgui_widgets.cpp
    Libs/imgui/imgui/imgui_impl_glfw.cpp
    Libs/imgui/imgui/imgui_impl_vulkan.cpp
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
    Engine/Graphics/RenderManager.cpp
    Engine/Graphics/Renderers/ForwardRenderer.cpp
    Engine/Graphics/Renderers/ImGuiRenderer.cpp

    # Vulkan
    Engine/Graphics/Vulkan/Vk_Image.cpp
    Engine/Graphics/Vulkan/Vk_Buffer.cpp
    Engine/Graphics/Vulkan/Vk_Shader.cpp
    Engine/Graphics/Vulkan/Vk_Devices.cpp
    Engine/Graphics/Vulkan/Vk_Instance.cpp
    Engine/Graphics/Vulkan/Vk_Renderer.cpp
    Engine/Graphics/Vulkan/Vk_Pipeline.cpp
    Engine/Graphics/Vulkan/Vk_Swapchain.cpp
    Engine/Graphics/Vulkan/Vk_Utilities.cpp
    Engine/Graphics/Vulkan/Vk_Semaphore.cpp
    Engine/Graphics/Vulkan/Vk_Renderpass.cpp
    Engine/Graphics/Vulkan/Vk_Framebuffer.cpp
    Engine/Graphics/Vulkan/Vk_CommandPool.cpp
    Engine/Graphics/Vulkan/Vk_DescriptorSet.cpp
    Engine/Graphics/Vulkan/Vk_CommandBuffer.cpp

    # Handlers
    Engine/Handlers/KeyHandler.cpp
    Engine/Handlers/MouseHandler.cpp

    # Utilities
    Engine/Utilities/YzLogger.cpp
    Engine/Utilities/IOHelper.cpp

    # Windows OS Specific
    Engine/Platform/Windows/GlfwWindow.cpp
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
    Engine/Application/GlobalSettings.h

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
    Engine/Graphics/RenderManager.h
    Engine/Graphics/Renderers/ForwardRenderer.h
    Engine/Graphics/Renderers/ImGuiRenderer.h
    Engine/Graphics/Renderers/YzRenderer.h

    # Vulkan
    Engine/Graphics/Vulkan/Vk.h
    Engine/Graphics/Vulkan/Vk_Image.h
    Engine/Graphics/Vulkan/Vk_Buffer.h
    Engine/Graphics/Vulkan/Vk_Shader.h
    Engine/Graphics/Vulkan/Vk_Devices.h
    Engine/Graphics/Vulkan/Vk_Pipeline.h
    Engine/Graphics/Vulkan/Vk_Instance.h
    Engine/Graphics/Vulkan/Vk_Renderer.h
    Engine/Graphics/Vulkan/Vk_Swapchain.h
    Engine/Graphics/Vulkan/Vk_Utilities.h
    Engine/Graphics/Vulkan/Vk_Semaphore.h
    Engine/Graphics/Vulkan/Vk_Renderpass.h
    Engine/Graphics/Vulkan/Vk_Framebuffer.h
    Engine/Graphics/Vulkan/Vk_CommandPool.h
    Engine/Graphics/Vulkan/Vk_DescriptorSet.h
    Engine/Graphics/Vulkan/Vk_CommandBuffer.h

    # Handlers
    Engine/Handlers/KeyHandler.h
    Engine/Handlers/MouseHandler.h

    # Utilities
    Engine/Utilities/YzLogger.h
    Engine/Utilities/IOHelper.h
    Engine/Utilities/T_Singleton.h

    # Windows OS Specific
    Engine/Platform/Windows/GlfwWindow.h
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
