# NVIDIA DeepStream with Ultralytics YOLOv8

Run the latest object-detection SOTA model YOLOv8 with DeepStream!

![FPS](resources/yolov8.gif)

## Repository setup

This is a straightforward step, however, if you are new to git, I recommend glancing threw the steps.

First, install git

```sh
sudo apt install git
```

Next, clone the repository

```sh
https://github.com/kn1ghtf1re/yolov8-deepstream-6-1.git
```

## Download the model files

You can run the `download-models.sh` script to download `onnx` yolov8 models

```sh
cd models
chmod 0777 download-models.sh
./download-models.sh
```
## Docker image build

This repository is tested and used with `docker`. For docker setup with `nvidia`, [click here](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html).

### 1. Build the image

```sh
cd docker/dgpu

docker build . -t ds-6.1:custom-1
```

### 2. Start a container

```sh
xhost +

export DISPLAY=:0

docker run -it --rm --net=host --runtime nvidia -e DISPLAY=$DISPLAY -v /tmp/.X11-unix/:/tmp/.X11-unix -v /home/kn1ght/Projects/yolov8-deepstream-6-1:/app ds-6.1:custom-1
```

## Running the Application

### 1. Build the application

```sh
make clean && make -j$(nproc)
```

### 2. Start the application


Next, create a file called `inputsources.txt` and paste the path of videos or rtsp url.

```sh
file:///home/astr1x/Videos/sample.mp4
rtsp://admin:admin%40123@192.168.1.1:554/stream
```

Now, run the application by running the following command:

```sh
# For barebones
./ds-yolov8

# For system76 hybrid graphics
__NV_PRIME_RENDER_OFFLOAD=1 ./ds-yolov8
```

## Citations

* [ultralytics/ultralytics](https://github.com/ultralytics/ultralytics)