FROM emscripten/emsdk:3.1.20

RUN apt update && apt install -y ninja-build nginx

WORKDIR /app

COPY ./ ./

RUN mkdir -p /app/web/build && cd /app/web/build && cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .. && ninja

RUN cp -r /app/web/build/html/* /usr/share/nginx/html/

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
