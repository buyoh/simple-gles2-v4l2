# simple-gles2-v4l2

A sample implementation for OpenGL ES 2 with v4l2

## setup

```bash
sudo apt install libgles2-mesa-dev libegl1-mesa-dev xorg-dev libv4l-dev
```

## build

```bash
mkdir -p build && cd build
CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_COMPILER=clang++ -GNinja ..
ninja 
```

## run
