# CGAL plane detection

## Description

[CGAL](https://www.cgal.org/) region growing algorithm for plane detection.

![example](img/example.gif)

## Requirements

1. CGAL 5.0 or higher.
2. Boost: math, filesystem, program options. Project was tested with Boost 1.71.
3. CMake 3.10 or higher.
4. Optional. [VTK](https://vtk.org/). It need only for visualization.
5. Compiler with support C++14.

## CMake features

1. [PCH](https://en.wikipedia.org/wiki/Precompiled_header) used only for MSVC. It is not configured for other compilers.
2. All files include implicitly `stdafx.h` for MSVC.


## How to run

Create new directory `build`.

```
mkdir build
cd build
```

Run CMake:

```
cmake -G "Visual Studio 16 2019" -A x64 -DCGAL_DIR=<path_to_dir_with_cmake> .. 
```

Suppose that you use Visual Studio 2019. For other tools see `cmake --help`.

It is recommended to install [vcpkg](https://github.com/microsoft/vcpkg). If you have vcpkg that:
```
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>\vcpkg\scripts\buildsystems\vcpkg.cmake -DCGAL_DIR=<path_to_dir_with_cmake>  .. 

```

Run:
```
 cmake --build . --config Release 
```

Find executable.

```
.\cgal-plane-detection.exe -i input_mes.obj -o segmented.ply -c
```

Results are three files:

1. Segmented mesh `segmented.ply`.
2. Bounding planes in the `segmented-planes.ply` as mesh.
3. Coordinates of bounding planes in the `segmented-planes.txt`.

You can download example from [Sketchfab](https://skfb.ly/6tJLJ).

See `--help` for additional parameters

Some parameters [are described here](https://doc.cgal.org/latest/Shape_detection/classCGAL_1_1Shape__detection_1_1Point__set_1_1Least__squares__plane__fit__region.html#acae6e40ba29b98a433e125622ba5ec80) 

Argument `-d` allow to see results (if you compile with VTK).

## Remarks

For best result you need axis aligned model.
