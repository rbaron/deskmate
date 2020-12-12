FROM alpine:3.12

RUN apk add --no-cache git clang cmake make binutils build-base libc-dev g++ sdl2-static sdl2_ttf-dev samurai openssl-dev

# Download, compile and install SDL2_gfx.
RUN mkdir /opt/SDL2_gfx && \
    cd /opt/SDL2_gfx && \
    wget http://www.ferzkopp.net/Software/SDL2_gfx/SDL2_gfx-1.0.4.tar.gz && \
    tar zxvf SDL2_gfx-1.0.4.tar.gz && \
    cd SDL2_gfx-1.0.4 && \
    ./configure && \
    make && \
    make install

COPY build.sh /build.sh

ENTRYPOINT ["/build.sh"]