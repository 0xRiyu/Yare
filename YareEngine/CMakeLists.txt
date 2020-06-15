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
# Set YareEngine Source Files
#--------------------------------------------------------------------
set (YARE_ENGINE_SOURCES
    # Application
    Source/Application/Application.cpp

    # Core
    Source/Core/Memory.cpp
    Source/Core/Transform.cpp

    # Graphics
    Source/Graphics/Mesh.cpp
    Source/Graphics/MeshFactory.cpp
    Source/Graphics/MeshInstance.cpp
    Source/Graphics/TiledMesh.cpp
    Source/Graphics/Material.cpp
    Source/Graphics/RenderManager.cpp
    Source/Graphics/Camera/FpsCamera.cpp
    Source/Graphics/Window/GlfwWindow.cpp
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
    Source/Core/Transform.h
    Source/Core/DataStructures.h

    # Graphics
    Source/Graphics/Mesh.h
    Source/Graphics/MeshFactory.h
    Source/Graphics/MeshInstance.h
    Source/Graphics/TiledMesh.h
    Source/Graphics/Material.h
    Source/Graphics/RenderManager.h
    Source/Graphics/Camera/Camera.h
    Source/Graphics/Camera/FpsCamera.h
    Source/Graphics/Window/Window.h
    Source/Graphics/Window/GlfwWindow.h
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
                PUBLIC ${CMAKE_SOURCE_DIR}/YareEngine/Libs/glfw/Windows/lib-vc2019/glfw3.lib
            PUBLIC ${CMAKE_SOURCE_DIR}/YareEngine/Libs/spdlog/msvc/x64-Release/lib/spdlog.lib)

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(${PROJECT_NAME}
        PUBLIC ${CMAKE_SOURCE_DIR}/YareEngine/Libs/glfw/Windows/lib-mingw-w64/libglfw3.a
            PUBLIC ${CMAKE_SOURCE_DIR}/YareEngine/Libs/spdlog/mingw/libspdlog.a)
    endif()
endif()

if (UNIX)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC ${CMAKE_SOURCE_DIR}/YareEngine/Libs/glfw/Unix/lib-gcc/libglfw3.a dl -lpthread -lm
        PUBLIC ${CMAKE_SOURCE_DIR}/YareEngine/Libs/spdlog/Unix/gcc/libspdlog.a)
endif()
