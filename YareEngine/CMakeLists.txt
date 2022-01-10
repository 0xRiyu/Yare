#--------------------------------------------------------------------
# Set Lib Source Files
#--------------------------------------------------------------------
set (LIB_SOURCES
    # imgui
    Lib/imgui/imgui.cpp
    Lib/imgui/imgui_draw.cpp
    Lib/imgui/imgui_demo.cpp
    Lib/imgui/imgui_widgets.cpp
    Lib/imgui/imgui_tables.cpp
    Lib/imgui/backends/imgui_impl_glfw.cpp
    Lib/imgui/backends/imgui_impl_vulkan.cpp
)

#--------------------------------------------------------------------
# Set Lib Header Files
#--------------------------------------------------------------------
set (LIB_HEADERS
    Lib/stb/stb_image.h
    Lib/tinyobjloader/tiny_obj_loader.h
)

#--------------------------------------------------------------------
# Set YareEngine Source Files
#--------------------------------------------------------------------
set (YARE_ENGINE_SOURCES
    # Application
    Source/Application/Application.cpp

    # Core
    Source/Core/Memory.cpp

    # Graphics
    Source/Graphics/Components/Mesh.cpp
    Source/Graphics/Components/Material.cpp
    Source/Graphics/Components/Transform.cpp
    Source/Graphics/MeshFactory.cpp
    Source/Graphics/RenderManager.cpp
    Source/Graphics/Camera/FpsCamera.cpp
    Source/Graphics/Window/GlfwWindow.cpp
    Source/Graphics/Scene/Entity.cpp
    Source/Graphics/Scene/Scene.cpp
    Source/Graphics/Renderers/Renderer.cpp
    Source/Graphics/Renderers/ImGuiRenderer.cpp
    Source/Graphics/Renderers/SkyboxRenderer.cpp
    Source/Graphics/Renderers/ForwardRenderer.cpp

    # Vulkan
    Source/Graphics/Vulkan/Image.cpp
    Source/Graphics/Vulkan/Buffer.cpp
    Source/Graphics/Vulkan/Shader.cpp
    Source/Graphics/Vulkan/Context.cpp
    Source/Graphics/Vulkan/Devices.cpp
    Source/Graphics/Vulkan/Pipeline.cpp
    Source/Graphics/Vulkan/Swapchain.cpp
    Source/Graphics/Vulkan/Utilities.cpp
    Source/Graphics/Vulkan/Semaphore.cpp
    Source/Graphics/Vulkan/Renderpass.cpp
    Source/Graphics/Vulkan/Framebuffer.cpp
    Source/Graphics/Vulkan/CommandPool.cpp
    Source/Graphics/Vulkan/DescriptorSet.cpp
    Source/Graphics/Vulkan/CommandBuffer.cpp

    # Handlers
    Source/Input/KeyHandler.cpp
    Source/Input/MouseHandler.cpp

    # Utilities
    Source/Utilities/Logger.cpp
    Source/Utilities/IOHelper.cpp
)

#--------------------------------------------------------------------
# Set YareEngine Header Files
#--------------------------------------------------------------------
set (YARE_ENGINE_HEADERS
    # API
    Source/Yare.h

    # Application
    Source/Application/EntryPoint.h
    Source/Application/Application.h
    Source/Application/GlobalSettings.h

    # Core
    Source/Core/Glfw.h
    Source/Core/Core.h
    Source/Core/Memory.h
    Source/Core/DataStructures.h

    # Graphics
    Source/Graphics/Components/Mesh.h
    Source/Graphics/Components/Material.h
    Source/Graphics/Components/Transform.h
    Source/Graphics/MeshFactory.h
    Source/Graphics/RenderManager.h
    Source/Graphics/Camera/Camera.h
    Source/Graphics/Camera/FpsCamera.h
    Source/Graphics/Window/Window.h
    Source/Graphics/Window/GlfwWindow.h
    Source/Graphics/Scene/Entity.h
    Source/Graphics/Scene/Scene.h
    Source/Graphics/Renderers/Renderer.h
    Source/Graphics/Renderers/ImGuiRenderer.h
    Source/Graphics/Renderers/SkyboxRenderer.h
    Source/Graphics/Renderers/ForwardRenderer.h

    # Vulkan
    Source/Graphics/Vulkan/Vk.h
    Source/Graphics/Vulkan/Image.h
    Source/Graphics/Vulkan/Buffer.h
    Source/Graphics/Vulkan/Shader.h
    Source/Graphics/Vulkan/Context.h
    Source/Graphics/Vulkan/Devices.h
    Source/Graphics/Vulkan/Pipeline.h
    Source/Graphics/Vulkan/Swapchain.h
    Source/Graphics/Vulkan/Utilities.h
    Source/Graphics/Vulkan/Semaphore.h
    Source/Graphics/Vulkan/Renderpass.h
    Source/Graphics/Vulkan/Framebuffer.h
    Source/Graphics/Vulkan/CommandPool.h
    Source/Graphics/Vulkan/DescriptorSet.h
    Source/Graphics/Vulkan/CommandBuffer.h

    # Handlers
    Source/Input/InputHandler.h
    Source/Input/KeyHandler.h
    Source/Input/MouseHandler.h

    # Utilities
    Source/Utilities/Logger.h
    Source/Utilities/IOHelper.h
    Source/Utilities/T_Singleton.h
)

#--------------------------------------------------------------------
# Define the library & create an alias
#--------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${YARE_ENGINE_SOURCES} ${YARE_ENGINE_HEADERS}
                                   ${LIB_SOURCES}    ${LIB_HEADERS})
add_library(YareEngine::Source ALIAS ${PROJECT_NAME})

#--------------------------------------------------------------------
# Set include directories for all build configurations
#--------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source
    PUBLIC Lib/
    PUBLIC Lib/glm
    PUBLIC Lib/imgui
    PUBLIC Lib/spdlog/include
    PUBLIC Lib/glfw/include)

#--------------------------------------------------------------------
# Find/Build libraries that are required
#--------------------------------------------------------------------
find_package(Vulkan REQUIRED)

add_subdirectory(Lib/glfw)
add_subdirectory(Lib/glm)
add_subdirectory(Lib/spdlog)

#--------------------------------------------------------------------
# Link the libraries
#--------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    PUBLIC Vulkan::Vulkan
    PUBLIC glfw
    PUBLIC glm
    PUBLIC spdlog
    )

target_precompile_headers(${PROJECT_NAME} PRIVATE [["Utilities/Logger.h"]] <memory> <string> <vector>)
