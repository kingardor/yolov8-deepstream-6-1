# Build

```sh
docker build . -t ds-6.1:custom-1
```

# Run

```sh
xhost +

export DISPLAY=:0

docker run -it --rm --net=host --runtime nvidia -e DISPLAY=$DISPLAY -v /tmp/.X11-unix/:/tmp/.X11-unix -v /home/kn1ght/Projects/yolov8-deepstream-6-1:/app ds-6.1:custom-1

# For barebones
./ds-yolov8

# For system76 hybrid graphics
__NV_PRIME_RENDER_OFFLOAD=1 ./ds-yolov8
```
