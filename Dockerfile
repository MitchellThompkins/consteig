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
ARG ARM_GNU_TOOLCHAIN_URL="${ARM_GNU_BASE_URL}/${ARM_GNU_FULL}/binrel/arm-gnu-toolchain-${ARM_GNU_FULL}-x86_64-arm-none-eabi.tar.xz"

# ARM bare-metal cross-compilation toolchain (arm-none-eabi).
# The pre-built ARM GNU Toolchain targets glibc; gcompat provides compatibility
# on Alpine (musl).
RUN apk --no-cache add gcompat libstdc++ wget xz && \
    echo "Downloading ARM GNU Toolchain: ${ARM_GNU_TOOLCHAIN_URL}" && \
    wget -qO /tmp/arm-toolchain.tar.xz "${ARM_GNU_TOOLCHAIN_URL}" && \
    mkdir -p /opt/arm-gnu-toolchain && \
    tar xf /tmp/arm-toolchain.tar.xz -C /opt/arm-gnu-toolchain --strip-components=1 && \
    rm /tmp/arm-toolchain.tar.xz

ENV PATH="/opt/arm-gnu-toolchain/bin:${PATH}"
