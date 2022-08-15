Building:
    Use CMake to build the software. The example commands given in the project
    handout will work without any changes. The default build mode is Release
    which is essentially required for reasonable performance. The program uses
    Boost and Catch2, both of which should be installed on the lab computer
    environments.

Running:
    The program uses a command line interface. The available options can be
    shown by using the --help flag. All of the options are fairly self
    explanatory.

    The program works by tracing several paths of light for each pixel in the
    image (one path is called a "sample"). The runtime of the program is
    directly proportional to the number of pixels and the number of samples per
    pixel. An image can be rendered relatively quickly with a resolution of
    400x400 and 500 samples per pixel. The runtime of the program will also
    vary based on the number of polygons in the scene, so choosing a simpler
    scene such as spheres.jsonc or spheres_dof.jsonc will render faster than a
    complex scene like monkey.jsonc.

    The scene that the program renders is described using a JSON or JSONC file.
    Several example scenes are given in the ./tests/scenes directory. There is
    no formal description of the scene file schema, but you should be able to
    get a good idea of the format based on the provided example files. The file
    describes the camera settings, materials, and objects in the scene. I have
    tried my best to provide descriptive error messages in case the scene file
    is wrong, but it's hard to do this without just writing piles and piles of
    validation code. The program will never attempt to render using an invalid
    scene file, though.
