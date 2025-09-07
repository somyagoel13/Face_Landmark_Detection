cmake_minimum_required(VERSION 3.10)
project(FaceLandmarkDetection CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------
# Option: Enable Obfuscation
# -------------------------
option(OBFUSCATE "Build with OLLVM obfuscation" OFF)

if(OBFUSCATE)
    message(STATUS "⚡ Building with OLLVM obfuscation")

    # Path to your OLLVM build (adjust as needed)
    set(OLLVM_BIN /home/yankee/Desktop/Final/obfuscator-llvm-4.0/build/bin/)

    set(CMAKE_C_COMPILER ${OLLVM_BIN}/clang)
    set(CMAKE_CXX_COMPILER ${OLLVM_BIN}/clang++)

    # Obfuscation passes (remove -bcf if it causes crashes)
    set(OBFUSCATION_FLAGS "-mllvm -fla -mllvm -sub")
else()
    message(STATUS "🚀 Building without obfuscation")
    set(OBFUSCATION_FLAGS "")
endif()

# -------------------------
# Source files
# -------------------------
set(SOURCES
    main.cpp
    src/client.cpp
    src/faceDetector.cpp
    src/faceLandmark.cpp
    src/FaceLandMarkDetectionManager.cpp
    src/jsonParser.cpp
    src/server.cpp
    src/timeUtil.cpp
)

# -------------------------
# OpenCV (from build dir)
# -------------------------
set(OpenCV_DIR "$ENV{HOME}/opencv-4.7.0/build")
find_package(OpenCV REQUIRED)

# -------------------------
# Includes
# -------------------------
include_directories(
    ./include
    /usr/include/json-c
    /usr/include
)

# -------------------------
# Executable
# -------------------------
add_executable(faceDetectorApp ${SOURCES})

# -------------------------
# Linking
# -------------------------
target_link_libraries(faceDetectorApp
    ${OpenCV_LIBS}
    json-c
    pthread
    zmq
)

# -------------------------
# Apply Obfuscation Flags
# -------------------------
target_compile_options(faceDetectorApp PRIVATE ${OBFUSCATION_FLAGS})

# -------------------------
# Strip binary after build (optional)
# -------------------------
add_custom_command(TARGET faceDetectorApp POST_BUILD
    COMMAND strip --strip-all $<TARGET_FILE:faceDetectorApp>
    COMMENT "Stripping symbols from faceDetectorApp"
)

