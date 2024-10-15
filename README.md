# yanbrowser

This is Yanbrowser. Yanbrowser is a toy browser built using C++.

## features
- HTML parser & DOM tree builder
- CSS parser & CSSOM tree builder
- Style processor via selector & element matching
- Render & Box layout tree builder
- ImGUI based UI
- curl URL content retriever

## examples

here are some screenshots :D
<img width="912" alt="Screenshot 2024-10-15 at 12 22 34 AM" src="https://github.com/user-attachments/assets/8d4a9d27-aa65-4964-a7af-87fb8c874a9f">

## setup

clone repo (duh!)

```bash
mkdir -p cmake
curl -L https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/cpm.cmake -o cmake/CPM.cmake
```

Building
```bash
mkdir build
cd build
cmake ..
# on linux and mac
make -j$(nproc)
# on windows (assumes ur building with mvsc)
cmake --build . --config Release -- /m

cd ..
./build/toybrowser
```


