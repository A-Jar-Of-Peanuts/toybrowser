# yanbrowser

this is yanbrowser.
yansbrowser is so fragile (like my feelings) please dont feed it broken code :(

yanbrowser cannot manipulate dom tree after it has been made that's very mean thing to ask.

## Setup

```bash
mkdir -p cmake
curl -L https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/cpm.cmake -o cmake/CPM.cmake
```

Building
```bash
cd build
cmake ..
# on linux and mac
make -j$(nproc)
# on windows (assumes ur building with mvsc)
cmake --build . --config Release -- /m
```