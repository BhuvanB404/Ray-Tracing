# Simple Ray Tracer in C++

This is a C++ implementation of a simple ray tracer, built from scratch. The project demonstrates fundamental computer graphics concepts including ray-object intersections, lighting, materials, and camera controls.

## Features
- Sphere rendering with proper lighting
- Support for different materials (metal, dielectric, lambertian)


## Example Output

![Ray Traced Image](images/raytraced_image.ppm)

## Building

```bash
mkdir -p build
cd build
cmake ..
make
```

## Running

```bash
./ray > output.ppm
```

## Dependencies
- C++17 or later
- CMake (3.10+)
