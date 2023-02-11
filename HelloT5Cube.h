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

/****************************************************************************
Main application class
*****************************************************************************/
class HelloT5Cube : public GLApp::Application
{
	/****************************************************************************
	Holds framebuffer data for the two stereo images
	*****************************************************************************/
	struct DisplaySurface
	{
		int height;
		int width;
		int layers = 1;

		Owned<GLW::Texture> texture;
		Owned<GLW::Texture> depth;
		Owned<GLW::Framebuffer> framebuffer;

		bool Initialize(int height, int width, bool hasDepth = true);
		bool InitializeMultiview(int height, int width, int layers);
		void BeginDraw();
		void EndDraw();

		void BlitToScreen(int dstHeight, int dstWidth);
		void BlitToFrameBuffer(GLW::Framebuffer& framebuffer, int dstHeight, int dstWidth);
		void BlitFromFrameBuffer(GLW::Framebuffer& framebuffer);

		Owned<GLW::Texture> GetTextureLayer(int layer);
	};

	bool glDebug;

	// Are these the right defaults? Values were pulled from other
	// source references
	int defaultWidth = 1216;
	int defaultHeight = 768;
	float defaultFOV = 48.0;

	bool isMultiviewCapable = false;
	bool useMultiview = false;
	bool copyMultviewTextures = false;

	Owned<GLW::Texture> leftLayerTexture;
	Owned<GLW::Texture> rightLayerTexture;
	Owned<GLW::Framebuffer> leftLayerFramebuffer;
	Owned<GLW::Framebuffer> rightLayerFramebuffer;

	// program and vertex buffers for rendering
	Owned<GLW::ShaderProgram> cubeShader;
	Owned<GLW::Buffer> cubeVertexBuffer;
	Owned<GLW::VertexArray> cubeVertexArrays;

	// Frame buffers for left and right eye
	DisplaySurface leftEyeDisplay;
	DisplaySurface rightEyeDisplay;

	DisplaySurface stereoEyeDisplay;

	// T5 handles
	Owned<T5W::Context> context;
	Owned<T5W::Glasses> glasses;

	// All the poses neeed to make up the view matrix
	GLApp::Transform gameboardPose;
	GLApp::Transform headPose; // Relative to game board pose
	GLApp::Transform leftEyePose; // Relative to head pose
	GLApp::Transform rightEyePose; // Relative to head pose

	// List of glasses ID we got from the T5 API
	std::vector<std::string> glassesIDList;
	// Version number form the T5 service
	std::string serviceVersion;

	// Misc
	ChangeDetector<bool> isPoseValid;
	ChangeDetector<bool> isFrameSent;

	bool isOutputingOnePoseFrame = false;
	int frameCounter = 0;

	// overrides from GLApp::Application
	bool InitializeApplication() override;
	bool InitializeContext() override;
	void OnKey(int key, int scancode, int action, int mods) override;
	void Update() override;

	bool InitializeT5();
	bool ConnectGlasses(std::string glassesID);

	void UpdateGlassesPose();
	void Render();
	void SendFramesToGlasses();

	public:
	HelloT5Cube(int width, int height, std::string title, bool multiview, bool copyTextures, bool dgl) 
	: GLApplication::Application(width, height, title), 
	useMultiview(multiview), 
	copyMultviewTextures(copyTextures),
	glDebug(dgl)
	{}
};

