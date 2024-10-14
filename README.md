# yanbrowser

this is yanbrowser.
yansbrowser is so fragile (like my feelings) please dont feed it broken code :(

yanbrowser cannot manipulate dom tree after it has been made that's very mean thing to ask.

# examples
here are some screenshots :D
<img width="799" alt="Screenshot 2024-10-14 at 3 04 30 PM" src="https://github.com/user-attachments/assets/99af40c6-8517-475a-9000-e0f61f2aa380">
<img width="781" alt="Screenshot 2024-10-14 at 3 07 17 PM" src="https://github.com/user-attachments/assets/4fe15890-faff-4f70-892d-7e32e8f29b52">
<img width="796" alt="Screenshot 2024-10-14 at 3 08 00 PM" src="https://github.com/user-attachments/assets/bd65f79c-436a-4827-96f3-d75683aa521b">


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
