
add_library(YareZo
        YareZo.h
        src/Core.h
        src/EntryPoint.h
        src/Application.h
        src/Application.cpp
        src/Window.h
        src/Vulkan.cpp
        src/Vulkan.h
        Utilities/YzLogger.h
        Utilities/YzLogger.cpp
        Utilities/IOHelper.h
        Utilities/IOHelper.cpp
        Windows/GlfwWindow.cpp
        Windows/GlfwWindow.h)

include_directories(${CMAKE_SOURCE_DIR}/YareZo/src
                    ${CMAKE_SOURCE_DIR}/YareZo/Utilities
                    ${CMAKE_SOURCE_DIR}/YareZo/Windows)


find_package(Vulkan REQUIRED)

if(NOT TARGET spdlog)
    # Stand-alone build
    set(spdlog_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs/spdlog/lib/cmake/spdlog")
    find_package(spdlog REQUIRED)
endif()


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
            ${CMAKE_SOURCE_DIR}/libs/glfw-3.3.bin.WIN64/lib-vc2019/glfw3.lib
            )
    target_link_libraries(YareZo
            PRIVATE
            spdlog::spdlog)


endif()


