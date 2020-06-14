cmake_minimum_required(VERSION 3.14)
project(Sandbox)

#--------------------------------------------------------------------
# Set sources
#--------------------------------------------------------------------
set (SANDBOX_SOURCES
        src/Sandbox.cpp
)

set (SANDBOX_HEADERS
)

#--------------------------------------------------------------------
# Create executable project
#--------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SANDBOX_SOURCES} ${SANDBOX_HEADERS})

#--------------------------------------------------------------------
# Include directories from the Engine - Soon to restrict to just the interface
#--------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/YareEngine)

#--------------------------------------------------------------------
# Link to the Engine
#--------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} YareEngine::Source)
