cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)

project(PAYLOAD VERSION 1.0 LANGUAGES CXX)
add_definitions(-DROOT_DIR=\"${CMAKE_SOURCE_DIR}\")
add_definitions(-DMODELS_DIR=\"${CMAKE_SOURCE_DIR}/models\")

# Check if LIBTORCH_PATH is set in the environment
if(NOT DEFINED ENV{LIBTORCH_PATH})
    message(FATAL_ERROR "LIBTORCH_PATH environment variable is not set. Please set it in your .bashrc (e.g., export LIBTORCH_PATH=/path/to/libtorch/share/cmake/Torch/).")
else()
    set(CMAKE_PREFIX_PATH $ENV{LIBTORCH_PATH})
endif()


# Find the required packages
set(SPDLOG_FMT_EXTERNAL OFF)
find_package(spdlog REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)
find_package(GTest REQUIRED)

# Include OpenCV directories
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS} ${TORCH_INCLUDE_DIRS})

# Include the directories for header files
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/external) # ignore warnings from external libraries

# Add the subdirectory containing source files
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(src)

# Add the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries to the executable
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    spdlog::spdlog
    PAYLOAD_LIB
    ${OpenCV_LIBS}
    ${TORCH_LIBRARIES}
)

# Add compiler warnings (as errors) and other warning flags
target_compile_options(${PROJECT_NAME} PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wconversion
    -Wsign-conversion
)


option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()    
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()