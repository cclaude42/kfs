#!/bin/bash

# Allow container to connect to local X server
xhost +
# Build kfs building environment
docker build -t kfs-builder .
# Run container (with specified host and display)
docker run -it --rm -v $PWD:/workspace --net host --env DISPLAY kfs-builder
