FROM ubuntu:22.04

RUN apt update --yes
RUN apt install --yes ca-certificates curl gnupg

RUN sed -i 's/htt[p|ps]:\/\/archive.ubuntu.com\/ubuntu\//mirror:\/\/mirrors.ubuntu.com\/mirrors.txt/g' /etc/apt/sources.list

RUN echo "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main" > /etc/apt/sources.list.d/llvm.list
RUN curl --silent --location https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

RUN curl --silent --location https://deb.nodesource.com/setup_20.x | bash -

RUN apt update --yes

RUN apt install --yes build-essential git cmake libssl-dev pip ninja-build gdb clangd-16 nodejs

RUN pip install conan==1.60.0
RUN conan profile new default --detect
RUN conan profile update settings.compiler.libcxx=libstdc++11 default

COPY conanfile.txt /conan/

RUN conan install /conan -pr=default -s build_type=Debug --build=missing -g txt -of /conan

RUN corepack enable
RUN corepack prepare yarn@stable --activate

EXPOSE 8080
