
add_executable(Sandbox
        src/Sandbox.cpp)

include_directories(
        ${CMAKE_SOURCE_DIR}/YareZo)

target_link_libraries(Sandbox
        PUBLIC
        YareZo
        )
