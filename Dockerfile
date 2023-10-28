FROM ubuntu:20.04 as builder

# Install required packages, gcc-multilib for the 32bit ELF support
RUN apt-get update && apt-get -y install build-essential git zip gcc-multilib \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

# Add user, create home dir
# https://gist.github.com/alkrauss48/2dd9f9d84ed6ebff9240ccfa49a80662
RUN mkdir -p /home/app
RUN groupadd -r app &&\
    useradd -r -g app -d /home/app -s /sbin/nologin -c "docker user" app
ENV HOME=/home/app
WORKDIR ${HOME}

RUN git clone https://github.com/viralpoetry/pocketbook-sdk ${HOME}/development/pocketbook-sdk-fw4
ENV FRSCSDK="${HOME}/development/pocketbook-sdk-fw4/FRSCSDK"

# Build our app
FROM builder AS build_app
# dirty hack to disable caching for this stage
# https://stackoverflow.com/questions/35134713/disable-cache-for-specific-run-commands
ARG CACHEBUST
RUN echo "$CACHEBUST"

COPY app/main.cpp ./
RUN ${FRSCSDK}/bin/arm-none-linux-gnueabi-g++ \
 -Wall -Wextra -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function \
 main.cpp -o pocketframe.app -linkview -g
