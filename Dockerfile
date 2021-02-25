FROM ubuntu:latest

ENV TZ=Asia/Tashkent
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

COPY . /app

RUN apt-get -y update
RUN apt-get -y upgrade

# gcc, g++, make
RUN apt-get -y install build-essential

# install cmake
RUN apt-get -y install wget
RUN wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz
RUN tar -zxvf cmake-3.15.2.tar.gz
WORKDIR /cmake-3.15.2

RUN ./bootstrap
RUN make
RUN make install

WORKDIR /

#install python
RUN apt-get -y install python2
RUN apt-get -y install python-dev
RUN apt-get -y install python libpython3.8-dev
RUN apt-get -y install python3.9-distutils

#install gdb
RUN apt-get -y install gdb

WORKDIR /app/build

RUN cmake ..
RUN cmake --build . --target core

RUN g++ -g ../example.cpp -fdebug-prefix-map=..=$(readlink -f ..) -o example
RUN cp ../graph .

WORKDIR /
