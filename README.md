# `egb-lang`
## Compiler front end for LLVM

`egb-lang` is a simple, procedural language that targets LLVM.

## Installation
### Requirements
- CMake 3.14.5 or newer
- LLVM 13.0.x

### Windows
The prebuilt LLVM binary for Windows doesn't support [CMake configuration](https://stackoverflow.com/questions/48947973/use-llvm-in-a-cmake-build). In
order to build `egb-lang` on Windows, you must [download and build LLVM from
source.](https://github.com/llvm/llvm-project/releases/tag/llvmorg-13.0.1)

```bash
$ cmake -S src -B build -G "Visual Studio 1x 20xx" -DLLVM_DIR=C:\path\to\lib\cmake\llvm\

$ cmake --build build
```

### Linux and MacOS
On *nix systems, `egb-lang` can be built with either GNU Make or Ninja:

```bash
$ cmake -S src -B build -G Ninja

$ cmake --build build
```

## Usage
If you wish, you can run the test suite located in
`\build\test\Debug\el_test.exe` (Windows) or `/build/test/Debug/el_test` (*nix).

The main executable is located in `\build\Debug\el.exe` (Windows) or `/build/el`
