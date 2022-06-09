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
* [ ] Light source material
* [ ] Recursive tracing function
* [ ] Single threaded render loop
* [ ] Scene description parsing w/ Boost
* [x] Ray/Triangle intersection
* [ ] Dielectric material scatter function

Extended Feature Set (Scope B only)
* [ ] Bounding Volume Hierarchy
* [ ] Multithreaded render loop
* [ ] Reinhard-Jodie Tone Mapping
* [ ] Reflective/semi-reflective material
