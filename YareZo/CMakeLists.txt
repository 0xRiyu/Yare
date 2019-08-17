add_library(YareZo
        src/EntryPoint.h
        src/Core.h
        YareZo.h
        src/Application.cpp
        src/Application.h)

include_directories(${CMAKE_SOURCE_DIR}/YareZo/src)

find_package(Vulkan REQUIRED)

install(TARGETS YareZo
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

if(WIN32)
    target_include_directories(YareZo
            PUBLIC
            ${CMAKE_SOURCE_DIR}/libs/glm
            ${CMAKE_SOURCE_DIR}/libs/glfw-3.3.bin.WIN64/include
            )

    target_link_libraries(YareZo
            PUBLIC
            Vulkan::Vulkan
            ${CMAKE_SOURCE_DIR}/libs/glfw-3.3.bin.WIN64/lib-mingw-w64/libglfw3.a
            )
endif()
