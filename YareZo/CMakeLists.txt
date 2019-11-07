
add_library(YareZo
        YareZo.h
        src/Core.h
        src/EntryPoint.h
        src/Application.h
        src/Application.cpp
        src/Window.h
        Utilities/YzLogger.h
        Utilities/YzLogger.cpp
        Windows/GlfwWindow.cpp
        Windows/GlfwWindow.h src/Vulkan.cpp src/Vulkan.h)

include_directories(${CMAKE_SOURCE_DIR}/YareZo/src
                    ${CMAKE_SOURCE_DIR}/YareZo/Utilities)

find_package(Vulkan REQUIRED)

install(TARGETS YareZo
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

if(WIN32)
    target_include_directories(YareZo
            PUBLIC
            ${CMAKE_SOURCE_DIR}/libs/glm
            ${CMAKE_SOURCE_DIR}/libs/glfw-3.3.bin.WIN64/include
            ${CMAKE_SOURCE_DIR}/libs/spdlog/include
            )

    target_link_libraries(YareZo
            PUBLIC
            Vulkan::Vulkan
            ${CMAKE_SOURCE_DIR}/libs/glfw-3.3.bin.WIN64/lib-mingw-w64/libglfw3.a
            )
endif()
