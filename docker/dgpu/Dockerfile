FROM nvcr.io/nvidia/deepstream:6.1.1-triton
# Add open GL libraries
RUN apt-get update && \
        DEBIAN_FRONTEND=noninteractive  apt-get install -y --no-install-recommends \
        pkg-config \
        libgnutls28-dev \
        libboost-all-dev && \
        rm -rf /var/lib/apt/lists/* && \
        apt autoremove

WORKDIR /app
# To get video driver libraries at runtime (libnvidia-encode.so/libnvcuvid.so)
ENV NVIDIA_DRIVER_CAPABILITIES $NVIDIA_DRIVER_CAPABILITIES,video,compute,graphics,utility

ENV CUDA_HOME=/usr/local/cuda
ENV CFLAGS="-I$CUDA_HOME/include $CFLAGS"
