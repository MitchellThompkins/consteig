set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Use ARM GCC's libstdc++ headers for C++ standard library support
set(ARM_GCC_ROOT "/opt/arm-gnu-toolchain")
set(ARM_CXX_INC "${ARM_GCC_ROOT}/cxx-include")
set(ARM_C_INC "${ARM_GCC_ROOT}/arm-none-eabi/include")
set(ARM_GCC_INC "${ARM_GCC_ROOT}/gcc-include")

set(CMAKE_C_FLAGS_INIT "--target=arm-none-eabi")
set(CMAKE_CXX_FLAGS_INIT "--target=arm-none-eabi -isystem ${ARM_CXX_INC} -isystem ${ARM_CXX_INC}/arm-none-eabi -isystem ${ARM_GCC_INC} -isystem ${ARM_C_INC}")

# Skip CMake's link test during compiler detection (bare-metal has no C runtime)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
