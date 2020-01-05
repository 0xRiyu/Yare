#--------------------------------------------------------------------
# Set Engine Source Files
#--------------------------------------------------------------------
set (YAREZOENGINE_SOURCES
        # Source
        src/Application.cpp
        src/Vulkan.cpp
        src/YzInput.cpp
        src/YzCamera.cpp

        # Utilities
        Utilities/YzLogger.cpp
        Utilities/IOHelper.cpp

        # Windows Platform Specific
        Windows/GlfwWindow.cpp

        # We need to compile imgui files
        ${CMAKE_SOURCE_DIR}/libs/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
        ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
)

#--------------------------------------------------------------------
# Set Engine Header Files
#--------------------------------------------------------------------
set (YAREZOENGINE_HEADERS
        # API
        YareZo.h

        # Source
        src/Core.h
        src/EntryPoint.h
        src/Application.h
        src/Window.h
        src/Vulkan.h
        src/YzInput.h
        src/YzCamera.h

        # Utilities
        Utilities/YzLogger.h
        Utilities/IOHelper.h

        # Windows Platform Specific
        Windows/GlfwWindow.h
)

#--------------------------------------------------------------------
# Define the library & create an alias
#--------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${YAREZOENGINE_SOURCES} ${YAREZOENGINE_HEADERS})
add_library(YareZo::Engine ALIAS ${PROJECT_NAME})

#--------------------------------------------------------------------
# Set include directories for all build configurations
#--------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_SOURCE_DIR}/libs/glm
        PRIVATE ${CMAKE_SOURCE_DIR}/libs/imgui)

#--------------------------------------------------------------------
# Find static libraries that are required
#--------------------------------------------------------------------
find_package(Vulkan REQUIRED)


if(NOT TARGET spdlog)

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/spdlog/x64-Debug/include)

        set(spdlog_DIR "${CMAKE_SOURCE_DIR}/libs/spdlog/x64-Debug/lib/cmake/spdlog")
        find_package(spdlog REQUIRED)
    endif()

    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/spdlog/x64-Release/include)
        set(spdlog_DIR "${CMAKE_SOURCE_DIR}/libs/spdlog/x64-Release/lib/cmake/spdlog")
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
#--------------------------------------------------------------------
if(WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

        target_include_directories(${PROJECT_NAME}
            PUBLIC  ${CMAKE_SOURCE_DIR}/libs/glfw-vc2019/include)

        target_link_libraries(${PROJECT_NAME}
            PUBLIC ${CMAKE_SOURCE_DIR}/libs/glfw-vc2019/lib/glfw3.lib)

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MinGW")

        target_include_directories(${PROJECT_NAME}
            PUBLIC  ${CMAKE_SOURCE_DIR}/libs/glfw-mingw/include)

        target_link_libraries(${PROJECT_NAME}
            PUBLIC ${CMAKE_SOURCE_DIR}/libs/glfw-mingw/lib/glfw3.lib)
    endif()
endif()


