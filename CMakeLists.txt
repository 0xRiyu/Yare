
cmake_minimum_required(VERSION 3.14)
project(Yare VERSION 1.0 DESCRIPTION "Yare Rendering Engine" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(YZ_PLATFORM_WINDOWS
                        YZ_DYNAMIC_LINK
                        YZ_BUILD_DLL
                        )

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(YareEngine)
add_subdirectory(Sandbox)

file(COPY YareEngine/Res DESTINATION .)
