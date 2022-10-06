# Iceoryx-example

## Dependencies

- [iceoryx](https://github.com/eclipse-iceoryx/iceoryx) (tested with v2.90.0)
- [CMake](https://cmake.org), 3.16 or later
- One of the following compilers:
    - [GCC](https://gcc.gnu.org), 8.3 or later (5.4 currently supported too)
    - [Clang](https://clang.llvm.org), 9.0 or later
    - [MSVC](https://visualstudio.microsoft.com/de/), part of Visual Studio 2019 or later

## Build

1. Clone the repository

    ```bash
    git clone https://github.com/agamal94/Iceoryx-example.git
    ```

 2. Build everything

    ```bash
    cd Iceoryx-example
    mkdir build
    cd build
    cmake ..
    make
    ```

## Running the exmaple

inside the build directory you will find the excutable of the two processes:
- publisher
- subscriber

1. Start the iox-roudi (no special config required)
2. Start the two processes in any order.
