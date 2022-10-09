# Hello T5 Cube

Hello T5 Cube is a small OpenGL project to demonstrate using the [Tilt Five](https://www.tiltfive.com/) system's native SDK. 

## Build

Currently only Windows is supported. There is a Visual Studio solution for VS 2022 in the root directory. There is also a VS2019 solution in the vs2019 directory. The VS 2019 solution would need to be copied to the root to work. 

## Dependencies

- **OpenGL 4.6** *See TODO*
- **C++20** *See TODO*
- [**glad2**](https://github.com/Dav1dde/glad/tree/glad2) OpenGL loading   
- [**GLFW**](https://www.glfw.org/) Window management and OpenGL context creation
- [**GLM**](https://github.com/g-truc/glm) Math library
- [**TiltFive**](https://www.tiltfive.com/developers) Native SDK for the TiltFive Glasses

## TODO

- Currently in main.cpp the application is started twice. The first one will fail in `t5InitGlassesGraphicsContext` and the second start will succeed. When the T5 1.3 API comes out this should no longer be necessary. 
- Change build to CMake to allow porting to linux
- Add wand tracking
- Support multiple glasses
- Documentation
- Support older versions of OpenGL. The OpenGL DSA (Direct State Access) API is much easier to work with, which is why the application is using OpenGL 4.6 but it can be back ported to earlier version with a little work.

## Notes

For reference it might be helpful to see how the t5 coordinate system is set up.
![T5 coordinate system](https://github.com/patrickdown/HelloT5Cube/images/T5GameboardCoordSystem.jpg)
