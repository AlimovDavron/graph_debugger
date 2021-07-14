FROM ubuntu:latest

ENV TZ=Asia/Tashkent
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime &&\
    echo $TZ > /etc/timezone &&\
    apt-get -y update &&\
    apt-get -y upgrade

# gcc, g++, make
RUN apt-get -y install build-essential

# install cmake
RUN apt-get -y install wget &&\
    wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz &&\
    tar -zxvf cmake-3.15.2.tar.gz

WORKDIR /cmake-3.15.2

RUN ./bootstrap &&\
    make &&\
    make install

#install python
RUN apt-get -y install python2 &&\
    apt-get -y install python-dev &&\
    apt-get -y install python libpython3.8-dev &&\
    apt-get -y install python3.9-distutils

WORKDIR /

#install gdb
RUN apt-get -y install gdb

COPY . /app
WORKDIR /app/build

RUN cmake .. &&\
    cmake --build . --target core

RUN g++ -g ../example.cpp -fdebug-prefix-map=..=$(readlink -f ..) -o example &&\
    cp ../graph .

WORKDIR /
