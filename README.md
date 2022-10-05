# Hello T5 Cube

Hello T5 Cube is a small OpenGL project to demonstrate using the [Tilt Five](https://www.tiltfive.com/) system's native SDK. 

## Status

- ✅ Initialize window and OpenGL context
- ✅ Rendering two framebuffers for stereo view
- ✅ Initialize the the T5 system and acquire glasses for rendering. 🙁 Requires double initialization hack to initialize the T5 graphics.
- ✅ Reading poses from the glasses
- ✅ Translating pose into view matrix for each eye. Seems to be working right. It's hard to tell until sending frames to the glasses works.
- ❌ Sending textures to the glasses to be projected. The code is there but not working. 

## Build

Currently only Windows is supported. There is a Visual Studio solution for VS 2022 in the root directory. There is also a VS2019 solution in the vs2019 directory. The VS 2019 solution would need to be copied to the root to work. 

## Dependencies

- **OpenGL 4.6** *See TODO*
- **C++20** *See TODO*
- [**glad2**](https://github.com/Dav1dde/glad/tree/glad2) OpenGL loading   
- [**GLFW**](https://www.glfw.org/) Window management and OpenGL context creation
- [**GLM**](https://github.com/g-truc/glm) Math library
- [**TiltFive**](https://www.tiltfive.com/developers) Native SDK for the TiltFive Glasses

- **GLWrapper** A lightweight OpenGL wrapper *incomplete implementation see README in the directory*
- **T5Wrapper** A lightweight TiltFive C API wrapper *incomplete implementation see README in the directory*
- **GLApplication** A lightweight wrapper around GLFW for window setup, rendering, and teardown *see README in the directory*

## TODO

- Fix sending frames to the glasses. `t5SendFrameToGlasses` returns `T5_SUCCESS` but nothing is displayed on the glasses and the console is displaying error messages like this:

        [25272:hmd/client/gl/gl_texture_splitter.cpp:240] error kicking off packet prep: invalid operation
        [10940:.\hmd/client/frame_sender.h:437] error encoding framebuffer data packet header: invalid argument
        [10940:.\hmd/client/frame_sender.h:321] error sending frame data: invalid argument

- Currently in main.cpp the application is started twice. The first one will fail in `t5InitGlassesGraphicsContext` and the second start will succeed. When the T5 1.3 API comes out this should no longer be necessary. 
- Change build to CMake to allow porting to linux
- Add wand tracking
- Handle glasses state better
    - Glasses not connected before startup
    - Glasses disconnected while running
    - Tracking lost and gained
- Better error handling. Not much error handling in place.
- Documentation
- Support older versions of OpenGL. The OpenGL DSA (Direct State Access) API is much easier to work with, which is why the application is using OpenGL 4.6 but it can be backported to earlier version with a little work.
- Maybe support earlier version of C++? Currently there is one use of std::span that could be replaced. (Would really rather just lean into modern C++) 
- GLM is just included completely everywhere. Might want to go through and more selectively include specific modules.

