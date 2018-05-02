FROM debian:buster
LABEL maintainer "Paweł Jasiak <contact@jasiak.xyz>"

RUN apt-get update && apt-get install -y \
        gcc \
        g++ \
        libc6-dev \
        make \
        libseccomp-dev \
        seccomp \
        sudo \
        python3 \
        --no-install-recommends \
        && rm -rf /var/lib/apt/lists/*

COPY . /usr/local/src

WORKDIR /usr/local/src

RUN make all

CMD ["make", "test"]
