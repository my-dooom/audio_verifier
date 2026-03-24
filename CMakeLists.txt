cmake_minimum_required(VERSION 3.10)
project(audio_verifier)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
target_sources(audio_verifier PRIVATE
    src/analyze_wav.cpp
    src/generator.cpp
    src/main.cpp
)

# vcpkg integration (for fftw3)
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Find FFTW3
find_package(FFTW3 REQUIRED)

add_executable(audio_verifier
    src/analyze_wav.cpp
    src/generator.cpp
    src/main.cpp
)

target_include_directories(audio_verifier PRIVATE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(audio_verifier PRIVATE FFTW3::fftw3)
