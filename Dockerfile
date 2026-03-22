FROM alpine:3.23

RUN apk --no-cache add\
    cmake\
    clang\
    clang-dev\
    clang-extra-tools\
    make\
    gcc\
    g++\
    libc-dev\
    linux-headers\
    git\
    octave\
    python3

ARG ARM_GNU_VERSION="15.2"
ARG ARM_GNU_RELEASE="rel1"
ARG ARM_GNU_FULL="${ARM_GNU_VERSION}.${ARM_GNU_RELEASE}"
ARG ARM_GNU_BASE_URL="https://developer.arm.com/-/media/Files/downloads/gnu"
ARG ARM_GNU_TOOLCHAIN_FILE="arm-gnu-toolchain-${ARM_GNU_FULL}-x86_64-arm-none-eabi.tar.xz"
ARG ARM_GNU_TOOLCHAIN_URL="${ARM_GNU_BASE_URL}/${ARM_GNU_FULL}/binrel/${ARM_GNU_TOOLCHAIN_FILE}"

# ARM bare-metal cross-compilation toolchain (arm-none-eabi).
# The pre-built ARM GNU Toolchain targets glibc; gcompat provides compatibility
# on Alpine (musl).
RUN apk --no-cache add gcompat libstdc++ wget xz && \
    echo "Downloading ARM GNU Toolchain: ${ARM_GNU_TOOLCHAIN_URL}" && \
    wget -qO /tmp/${ARM_GNU_TOOLCHAIN_FILE} "${ARM_GNU_TOOLCHAIN_URL}" && \
    wget -qO /tmp/${ARM_GNU_TOOLCHAIN_FILE}.sha256asc "${ARM_GNU_TOOLCHAIN_URL}.sha256asc" && \
    cd /tmp && sha256sum -c ${ARM_GNU_TOOLCHAIN_FILE}.sha256asc && \
    mkdir -p /opt/arm-gnu-toolchain && \
    tar xf /tmp/${ARM_GNU_TOOLCHAIN_FILE} -C /opt/arm-gnu-toolchain --strip-components=1 && \
    rm /tmp/${ARM_GNU_TOOLCHAIN_FILE} /tmp/${ARM_GNU_TOOLCHAIN_FILE}.sha256asc && \
    # Clang doesn't ship its own C++ standard library for bare-metal ARM targets.
    # Symlink GCC's libstdc++ and compiler-provided headers to stable paths so the
    # clang toolchain file can add them via -isystem without embedding the GCC
    # version number. We only compile to .o files (no linking), but the headers
    # are still needed because the tests include standard library headers like
    # <cstddef> for types such as std::size_t.
    ln -s /opt/arm-gnu-toolchain/arm-none-eabi/include/c++/* /opt/arm-gnu-toolchain/cxx-include && \
    ln -s /opt/arm-gnu-toolchain/lib/gcc/arm-none-eabi/*/include /opt/arm-gnu-toolchain/gcc-include

ENV PATH="/opt/arm-gnu-toolchain/bin:${PATH}"
