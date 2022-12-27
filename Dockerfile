FROM alpine:3.14

RUN apk --no-cache add\
    cmake\
    clang\
    clang-dev\
    make\
    gcc\
    g++\
    libc-dev\
    linux-headers
