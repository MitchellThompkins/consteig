FROM alpine:3.23

RUN apk --no-cache add\
    cmake=4.1.3-r0\
    clang21=21.1.2-r2\
    clang21-dev=21.1.2-r2\
    clang21-extra-tools=21.1.2-r2\
    make=4.4.1-r3\
    gcc=15.2.0-r2\
    g++=15.2.0-r2\
    musl-dev=1.2.5-r21\
    linux-headers=6.16.12-r0\
    git=2.52.0-r0\
    octave=10.3.0-r1\
    python3=3.12.12-r0

ARG ARM_GNU_VERSION="15.2"
ARG ARM_GNU_RELEASE="rel1"
ARG ARM_GNU_FULL="${ARM_GNU_VERSION}.${ARM_GNU_RELEASE}"
ARG ARM_GNU_BASE_URL="https://developer.arm.com/-/media/Files/downloads/gnu"
ARG ARM_GNU_TOOLCHAIN_FILE="arm-gnu-toolchain-${ARM_GNU_FULL}-x86_64-arm-none-eabi.tar.xz"
ARG ARM_GNU_TOOLCHAIN_URL="${ARM_GNU_BASE_URL}/${ARM_GNU_FULL}/binrel/${ARM_GNU_TOOLCHAIN_FILE}"

# ARM bare-metal cross-compilation toolchain (arm-none-eabi).
# The pre-built ARM GNU Toolchain targets glibc; gcompat provides compatibility
# on Alpine (musl).
RUN apk --no-cache add gcompat=1.1.0-r4 libstdc++=15.2.0-r2 wget=1.25.0-r2 xz=5.8.2-r0 && \
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
