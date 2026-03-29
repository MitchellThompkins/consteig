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

# Bare-metal ARM has no C runtime, so CMake's default compile-and-link detection
# test would fail. Build a static library (.o only) instead to verify the compiler.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Prevent CMake from finding host system resources when cross-compiling:
#   PROGRAM NEVER  — use host tools (make, pkg-config), not target sysroot
#   LIBRARY ONLY   — only link against target ARM libraries, not host x86 libs
#   INCLUDE ONLY   — only search target sysroot for headers
#   PACKAGE ONLY   — only search target sysroot for CMake packages
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
