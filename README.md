# path_tracer

Path tracing rendering engine in very poorly written C++17

## Building / Testing / Running
```bash
# Release build
make release
./build/path_tracer --help

# Debug build
make debug

# Build and run unit tests
make test
```

Basic Feature Set
* [x] CLI parsing
* [x] PPM image generation
* [x] Vec3 class implementation
* [x] Ray/Sphere intersection calculation
* [x] Diffuse material scatter function
* [x] Light source material
* [x] Recursive tracing function
* [x] Single threaded render loop
* [x] Scene description parsing w/ Boost
* [x] Ray/Triangle intersection
* [ ] Dielectric material scatter function

Extended Feature Set (Scope B only)
* [ ] Bounding Volume Hierarchy
* [ ] Multithreaded render loop
* [x] Reinhard-Jodie Tone Mapping
* [ ] Reflective/semi-reflective material

## Progress Updates

### 2022-06-12
Successfully rendered the Cornell Box exterior

| Config      | Value          |
| ---         | ---            |
| Resolution  | 512x512        |
| Samples     | 7000           |
| Threads     | 1              |
| TMO         | Reinhard Jodie |
| Render Time | 5h6m57s        |
| Commit      | b6bbaab5       |

![Cornell Box](./img/cornell_no_boxes.png "Cornell Box")
