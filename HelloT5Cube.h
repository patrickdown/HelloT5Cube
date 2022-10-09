#pragma once
#include <glapplication/Application.h>

#include <glwrapper/Shader.h>
#include <glwrapper/Buffer.h>
#include <glwrapper/VertexArray.h>
#include <glwrapper/Framebuffer.h>
#include <t5wrapper/Context.h>
#include <t5wrapper/Glasses.h>
#include <glapplication/Transform.h>
#include <util/ChangeDetector.h>

namespace GLW = GLWrapper;
namespace T5W = T5Wrapper;
namespace GLApp = GLApplication;

class HelloT5Cube : public GLApp::Application
{
	struct DisplaySurface
	{
		int height;
		int width;

		Owned<GLW::Texture> texture;
		Owned<GLW::Renderbuffer> depth;
		Owned<GLW::Framebuffer> framebuffer;

		bool Initialize(int height, int width);
		void BeginDraw();
		void EndDraw();

		void BlitToScreen(int dstHeight, int dstWidth);
	};

	int defaultWidth = 1216;
	int defaultHeight = 768;
	float defaultFOV = 48.0;

	Owned<GLW::ShaderProgram> cubeShader;
	Owned<GLW::Buffer> cubeVertexBuffer;
	Owned<GLW::VertexArray> cubeVertexArrays;

	DisplaySurface leftEyeDisplay;
	DisplaySurface rightEyeDisplay;

	Owned<T5W::Context> context;
	Owned<T5W::Glasses> glasses;

	ChangeDetector<bool> isPoseValid;

	GLApp::Transform gameboardPose;
	GLApp::Transform headPose; // Relative to game board pose
	GLApp::Transform leftEyePose; // Relative to head pose
	GLApp::Transform rightEyePose; // Relative to head pose

	std::vector<std::string> glassesIDList;

	ChangeDetector<bool> isFrameSent;

	bool isOutputingOnePoseFrame = false;

	int frameCounter = 0;

	std::string serviceVersion;

	bool InitializeApplication() override;
	bool InitializeContext() override;
	void OnKey(int key, int scancode, int action, int mods) override;


	void Update() override;
	void UpdateGlassesPose();
	void Render();

	bool InitializeT5();
	bool ConnectGlasses(std::string glassesID);

	void SendFramesToGlasses();

	public:
	HelloT5Cube(int width, int height, std::string title) 
	: GLApplication::Application(width, height, title) {}
};

