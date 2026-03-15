FROM alpine:3.14

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

# ARM bare-metal cross-compilation toolchain (arm-none-eabi).
# The pre-built ARM GNU Toolchain targets glibc; gcompat provides compatibility
# on Alpine (musl). Used with CONSTEIG_COMPILE_ONLY to validate static_asserts
# without linking. Clang can reuse this toolchain's headers via --target=arm-none-eabi.
ARG ARM_GNU_TOOLCHAIN_URL=https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz
RUN apk --no-cache add gcompat libstdc++ wget xz && \
    wget -qO /tmp/arm-toolchain.tar.xz "${ARM_GNU_TOOLCHAIN_URL}" && \
    mkdir -p /opt/arm-gnu-toolchain && \
    tar xf /tmp/arm-toolchain.tar.xz -C /opt/arm-gnu-toolchain --strip-components=1 && \
    rm /tmp/arm-toolchain.tar.xz

ENV PATH="/opt/arm-gnu-toolchain/bin:${PATH}"
