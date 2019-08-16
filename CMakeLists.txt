cmake_minimum_required(VERSION 3.14)
project(YareZo)

set(CMAKE_CXX_STANDARD 17)

add_executable(YareZo
        main.cpp)

find_package(Vulkan REQUIRED)

target_include_directories(YareZo
        PUBLIC
            ${PROJECT_SOURCE_DIR}/libs/glm
            ${PROJECT_SOURCE_DIR}/libs/glfw-3.3.bin.WIN64/include
            C:/VulkanSDK/1.1.114.0/Include
        )

target_link_libraries(YareZo
        PUBLIC
            Vulkan::Vulkan
            ${PROJECT_SOURCE_DIR}/libs/glfw-3.3.bin.WIN64/lib-mingw-w64/libglfw3.a
        )

