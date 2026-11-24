# Build environment for Toy OS: i686-elf cross-compiler + grub-mkrescue.
FROM debian:bookworm-slim AS toolchain

ARG BINUTILS_VERSION=2.42
ARG GCC_VERSION=13.2.0
ENV PREFIX=/opt/cross TARGET=i686-elf

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential bison flex libgmp-dev libmpc-dev libmpfr-dev texinfo \
        wget ca-certificates xz-utils \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
RUN wget -q https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz \
    && wget -q https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz \
    && tar xf binutils-${BINUTILS_VERSION}.tar.xz \
    && tar xf gcc-${GCC_VERSION}.tar.xz

RUN mkdir build-binutils && cd build-binutils \
    && ../binutils-${BINUTILS_VERSION}/configure --target=$TARGET --prefix=$PREFIX \
        --with-sysroot --disable-nls --disable-werror \
    && make -j"$(nproc)" && make install

ENV PATH=$PREFIX/bin:$PATH
RUN mkdir build-gcc && cd build-gcc \
    && ../gcc-${GCC_VERSION}/configure --target=$TARGET --prefix=$PREFIX \
        --disable-nls --enable-languages=c --without-headers \
    && make -j"$(nproc)" all-gcc all-target-libgcc \
    && make install-gcc install-target-libgcc

# Final image: only the toolchain plus what's needed to make the ISO.
FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y --no-install-recommends \
        make libgmp10 libmpc3 libmpfr6 \
        grub-pc-bin grub-common xorriso mtools \
    && rm -rf /var/lib/apt/lists/*
COPY --from=toolchain /opt/cross /opt/cross
ENV PATH=/opt/cross/bin:$PATH
WORKDIR /work
