FROM gcc

RUN wget https://github.com/Kitware/CMake/releases/download/v3.28.3/cmake-3.28.3-linux-x86_64.sh -q -O /tmp/cmake-install.sh \
    && chmod u+x /tmp/cmake-install.sh \
    && mkdir /opt/cmake-3.28.3 \
    && /tmp/cmake-install.sh --skip-license --prefix=/opt/cmake-3.28.3 \
    && rm /tmp/cmake-install.sh \
    && ln -s /opt/cmake-3.28.3/bin/* /usr/local/bin

COPY generator /out/bin/

WORKDIR /src

COPY processor /src

RUN mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build . --target processor -j 8 \
    && cmake --install . --prefix "/out"

CMD ["/out/bin/generator", "|", "/out/bin/processor"]

