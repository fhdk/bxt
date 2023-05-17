FROM ubuntu:22.04

RUN apt update --yes
RUN apt install --yes ca-certificates

RUN sed -i 's/htt[p|ps]:\/\/archive.ubuntu.com\/ubuntu\//mirror:\/\/mirrors.ubuntu.com\/mirrors.txt/g' /etc/apt/sources.list
RUN apt update --yes

RUN apt install --yes build-essential git cmake libssl-dev pip curl clangd-15 ninja-build

RUN pip install conan==1.60.0
RUN conan profile new default --detect
RUN conan profile update settings.compiler.libcxx=libstdc++11 default

COPY conanfile.txt /conan/

RUN conan install /conan -s build_type=Debug

RUN curl --silent --location https://deb.nodesource.com/setup_20.x | bash -
RUN apt install --yes nodejs
RUN corepack enable
RUN corepack prepare yarn@stable --activate

EXPOSE 8080
