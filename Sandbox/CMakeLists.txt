
add_executable(Sandbox
               src/Sandbox.cpp)

include_directories(${CMAKE_SOURCE_DIR}/YareZo/src)

target_link_libraries(Sandbox YareZo)

install(TARGETS Sandbox RUNTIME DESTINATION bin)