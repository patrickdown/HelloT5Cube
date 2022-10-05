# GLApplication

This is a simple OpenGL application framework around GLFW 

## Notes

- Custom application is derived from `GLApplication::Application`
- Functions called in this order:
    - **InitializeApplication** called before window creation. It's a good place to call `glfwWindowHint` for setup
	- **InitializeContext** called after the window is created and the OpenGL context is current. It's a place to set up resources before the main render loop starts.
    - **Update** Called every frame while the window is open
- `OnKey` is currently the only event handler implemented. The default closes the application when ESC is pressed.
- Needs a lot more work to be a complete OpenGL application framework



