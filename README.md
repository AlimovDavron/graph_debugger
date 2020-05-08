# Usage
## build core 
```bash
git clone https://github.com/pybind/pybind11.git
mkdir build
cd build 
cmake ..
cmake --build . --target core
```

## build example
```bash
g++ -g ../example.cpp -o example
cp ../graph .
```

## Try to debug example program
```
./core
set-target example
```
See the list of commands on [wiki](../../wiki)
