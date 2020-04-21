
cmake_minimum_required(VERSION 3.14)
project(YareZo VERSION 1.0 DESCRIPTION "Yarezo Rendering Engine" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(YZ_PLATFORM_WINDOWS
        YZ_DYNAMIC_LINK
        YZ_BUILD_DLL
        )


add_subdirectory(YareZo)
add_subdirectory(Sandbox)

file(COPY YareZo/Resources DESTINATION YareZo)
