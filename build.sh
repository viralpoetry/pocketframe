#!/bin/bash
echo "# Building container..."
docker build . -t pocketbook --build-arg CACHEBUST=$(date +%s) -f Dockerfile
CID=$(docker create pocketbook)
mkdir -p build
# copy build artifacts from the container
echo "# Copying built artifacts from the container..."
docker cp ${CID}:/home/app/pocketframe.app build/pocketframe.app
# Remove the container..."
docker rm ${CID}
