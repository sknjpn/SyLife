FROM emscripten/emsdk:latest

WORKDIR /app

COPY ./ ./

RUN mkdir -p /app/web/build && cd /app/web/build && cmake -DCMAKE_BUILD_TYPE=Release .. && make
