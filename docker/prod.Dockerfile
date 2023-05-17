FROM bxt:dev

COPY ./ /src

RUN cmake -B/build -S/src -DCMAKE_BUILD_TYPE=Release
RUN cmake --build /build
