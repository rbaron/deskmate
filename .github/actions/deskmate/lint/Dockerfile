FROM alpine:3.12

RUN apk add --no-cache clang cmake make binutils libc-dev g++

COPY build.sh /build.sh

ENTRYPOINT ["/build.sh"]