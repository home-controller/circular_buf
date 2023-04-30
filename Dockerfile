# Fetch ubuntu image
FROM ubuntu:22.04

# Install prerequisites
RUN \
    apt update && \
    apt install -y git python3 && \
    apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
    
# Install Pico SDK
RUN \
    mkdir -p /project/src/ && \
    mkdir -p /project/test/ && \
#    cd /project/ && \
#    git clone https://github.com/raspberrypi/pico-sdk.git --branch master && \
#    cd pico-sdk/ && \
#    git submodule update --init && \
    cd /
    
# Set the Pico SDK environment variable
ENV PICO_SDK_PATH=/project/pico-sdk/

# Copy in our source files
COPY src/* /project/src/

# Copy in our test files
COPY test/* /project/test/


# Build project
RUN \
    mkdir -p /project/build && \
    cd /project && \
#     cmake .. && \
    make tests
    
# Command that will be invoked when the container starts
ENTRYPOINT ["/bin/bash"]