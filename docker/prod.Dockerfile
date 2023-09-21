FROM bxt:dev

COPY ./ /src

RUN cmake -B/build -S/src -DCMAKE_BUILD_TYPE=Release
RUN cmake --build /build
RUN cp /src/daemon/box.yml /build/bin/
ENV LD_LIBRARY_PATH=./

WORKDIR /build/bin/

CMD ["./bxtd"]
