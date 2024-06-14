# Example of using C++ modules with clang and cmake on Linux

An experiment with Ubuntu 24.04, CMake 3.30, and clang++ 18 showing:

1. `import std;`
2. `export module` and `import module` for `hello_lib`

The software versions are as of Jun 13, 2024.
Using `clang++-18` or later is necessary because `g++-14` does not yet have support for `import std;` yet.

### Conventions
A command executed as `sudo`:
```
# command
output
```
and as a user:
```
$ command
output
```


## Installing Ninja
```
# apt-get install ninja-build
```

## Installing CMake 3.30.0-rc2 candidate

```
# snap install cmake --candidate
```

**TIP**: VSCode does not work with snap version of cmake out-of-the box.
Set `CMake Path` to `/snap/cmake/current/bin/cmake` in "CMake Tools Extension Settings" or in `settings.json`:
```
"cmake.cmakePath": "/snap/cmake/current/bin/cmake"
```

## Installing clang and libc++

```
# apt-get install clang libc++-dev
# clang++ --version
Ubuntu clang version 18.1.3 (1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

## Building

### Configure to use clang++-18

It is important to set `-stdlib=libc++` for both the compiler and the linker so that GNU `libstdc++` is not used.
Otherwise the following error occurs:
```
The "CXX_MODULE_STD" property on the target "hello_lib" requires that the
   "__CMAKE::CXX23" target exist, but it was not provided by the toolchain.
   Reason:
 
     Only `libc++` is supported
```
Configure:
```
$ cd hello-cxx-modules
$ cmake --toolchain clang-toolchain.cmake \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=MinSizeRel \
        -S . \
        -B build
-- The CXX compiler identification is Clang 18.1.3
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/clang++ - skipped
-- Detecting CXX compile features
CMake Warning (dev) at /snap/cmake/1395/share/cmake-3.30/Modules/Compiler/CMakeCommonCompilerMacros.cmake:248 (cmake_language):
  CMake's support for `import std;` in C++23 and newer is experimental.  It
  is meant only for experimentation and feedback to CMake developers.
Call Stack (most recent call first):
  /snap/cmake/1395/share/cmake-3.30/Modules/CMakeDetermineCompilerSupport.cmake:113 (cmake_create_cxx_import_std)
  /snap/cmake/1395/share/cmake-3.30/Modules/CMakeTestCXXCompiler.cmake:83 (CMAKE_DETERMINE_COMPILER_SUPPORT)
  CMakeLists.txt:13 (project)
This warning is for project developers.  Use -Wno-dev to suppress it.

-- Detecting CXX compile features - done
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: /.../hello-cxx-modules/build
```

### Build
```
$ cmake --build build --target all
[1/14] Scanning /.../hello-cxx-modules/hello_lib.cxx for CXX dependencies
[2/14] Scanning /.../hello-cxx-modules/hello.cxx for CXX dependencies
[3/14] Scanning /usr/lib/llvm-18/share/libc++/v1/std.compat.cppm for CXX dependencies
[4/14] Scanning /usr/lib/llvm-18/share/libc++/v1/std.cppm for CXX dependencies
[5/14] Generating CXX dyndep file CMakeFiles/__cmake_cxx23.dir/CXX.dd
[6/14] Generating CXX dyndep file CMakeFiles/hello_lib.dir/CXX.dd
[7/14] Generating CXX dyndep file CMakeFiles/hello.dir/CXX.dd
[8/14] Building CXX object CMakeFiles/__cmake_cxx23.dir/usr/lib/llvm-18/share/libc++/v1/std.cppm.o
[9/14] Building CXX object CMakeFiles/hello_lib.dir/hello_lib.cxx.o
[10/14] Building CXX object CMakeFiles/hello.dir/hello.cxx.o
[11/14] Building CXX object CMakeFiles/__cmake_cxx23.dir/usr/lib/llvm-18/share/libc++/v1/std.compat.cppm.o
[12/14] Linking CXX static library lib__cmake_cxx23.a
[13/14] Linking CXX static library libhello_lib.a
[14/14] Linking CXX executable hello
```

### Execute
```
$ build/hello 
Hello World! My name is Michał
```

### Check dependencies

```
$ ldd build/hello
	linux-vdso.so.1 (0x00007ffe07b9c000)
	libc++.so.1 => /lib/x86_64-linux-gnu/libc++.so.1 (0x00007c04f3d6e000)
	libc++abi.so.1 => /lib/x86_64-linux-gnu/libc++abi.so.1 (0x00007c04f3d32000)
	libunwind.so.1 => /lib/x86_64-linux-gnu/libunwind.so.1 (0x00007c04f3d24000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007c04f3c3b000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007c04f3c0e000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007c04f3800000)
	/lib64/ld-linux-x86-64.so.2 (0x00007c04f3e86000)
```

## References

* [import CMake; the Experiment is Over!](https://www.kitware.com/import-cmake-the-experiment-is-over/)
* [import std in CMake 3.30](https://www.kitware.com/import-std-in-cmake-3-30/)