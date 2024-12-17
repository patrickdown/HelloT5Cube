
APP = HelloT5Cube
OBJS = HelloT5Cube.o InlineShaderCode.o  InlineVertexData.o main.o glad.o \
	Application.o Transform.o \
	Framebuffer.o Shader.o VertexArray.o \
	Context.o Glasses.o 

TARGET = $(APP)

CXX = g++
CXXFLAGS = -g -Wall -std=c++20
CXXFLAGS += -MMD -MP -MF .$$(basename $<).d
CXXFLAGS += -It5wrapper
CXXFLAGS += -Iglm
CXXFLAGS += -Iglad/OpenGL_4_6_Core/include
CXXFLAGS += -Iglwrapper
CXXFLAGS += -Iglapplication
CXXFLAGS += -Iutil
CXXFLAGS += -ITiltFive/include

LDFLAGS = -lGL -lGLU -lglut -lglfw -lX11 -lpthread -ldl -LTiltFive/lib/linux/x86_64/ -lTiltFiveNative

all: $(TARGET)

glad.o:	glad/OpenGL_4_6_Core/src/gl.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# glapplication
Application.o: glapplication/src/Application.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Transform.o: glapplication/src/Transform.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# glwrapper
Framebuffer.o:	glwrapper/src/Framebuffer.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Shader.o: glwrapper/src/Shader.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

VertexArray.o: glwrapper/src/VertexArray.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# t5wrapper
Context.o: t5wrapper/src/Context.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Glasses.o: t5wrapper/src/Glasses.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET) .*.d

.%.d:	;

-include .*.d
