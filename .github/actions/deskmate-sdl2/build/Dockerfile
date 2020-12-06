FROM ubuntu:20.04

ENV TZ=America/New_York
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && \
    apt-get -y install git make cmake clang clang-format ninja-build libsdl2-dev  libsdl2-ttf-dev libsdl2-gfx-dev

# Paho dependencies.
RUN apt-get -y install build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev

COPY . /app

WORKDIR /app