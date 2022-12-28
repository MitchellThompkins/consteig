FROM alpine:3.14

RUN apk --no-cache add\
    cmake=3.20.3-r0\
    clang=11.1.0-r1\
    clang-dev=11.1.0-r1\
    make=4.3-r0\
    gcc=10.3.1_git20210424-r2\
    g++=10.3.1_git20210424-r2\
    libc-dev\
    linux-headers=5.10.41-r0\
    git\
    python3
