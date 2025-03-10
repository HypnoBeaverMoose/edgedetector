cmake_minimum_required(VERSION 3.10)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")


# Project name
project(edgedetector)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(edgedetector
                main.cpp
                image.cpp
                file-utils.cpp
                edge-detector.cpp
)

# Include directories
target_include_directories(edgedetector PRIVATE
    ${PROJECT_SOURCE_DIR}
)

# Set output directories
set_target_properties(edgedetector PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# Add compile options
target_compile_options(edgedetector PRIVATE
    $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic -O0 -g>
    $<$<CONFIG:Release>:-Wall -Wextra -Wpedantic -O3>
)

# Link libraries (if any)
# target_link_libraries(edgedetector <libraries>)
