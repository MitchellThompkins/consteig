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
    tree\
    python3
