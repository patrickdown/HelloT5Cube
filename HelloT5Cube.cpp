#include <iostream>
#include <t5Wrapper/Conversions.h>
#include <glapplication/Transform.h>
#include "HelloT5Cube.h"
#include "VertexDefinition.h"

std::span<const VertexPC> GetCubeVertices();
const char* GetVertexShaderCode();
const char* GetFragmentShaderCode();

bool HelloT5Cube::InitializeApplication()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return true;
}

bool HelloT5Cube::RenderSurface::Initialize(int width, int height)
{
	this->width = width;
	this->height = height;

	texture = MakeOwned<GLW::Texture>();
	texture->Create(GL_TEXTURE_2D);
	texture->SetTextureStorage(GL_RGBA8, width, height);
	texture->SetMinFilter(GL_NEAREST);
	texture->SetMagFilter(GL_NEAREST);

	depth = MakeOwned<GLW::Renderbuffer>();
	depth->Create();
	depth->SetDepthStorage(width, height);

	framebuffer = MakeOwned<GLW::Framebuffer>();
	framebuffer->Create();
	framebuffer->ColorAttachment(*texture);
	framebuffer->DepthAttachment(*depth);
	framebuffer->DrawBuffers(0);

	if (!framebuffer->IsReady())
	{ 
		std::cerr << "leftEyeFramebuffer: " << framebuffer->GetErrorMessage() << std::endl;
		return false;
	}

	return true;
}

void HelloT5Cube::RenderSurface::BeginDraw()
{
	framebuffer->Bind(GL_DRAW_FRAMEBUFFER);
	glViewport(0, 0, width, height);
}

void HelloT5Cube::RenderSurface::EndDraw()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void HelloT5Cube::RenderSurface::BlitToScreen(int dstWidth, int dstHeight)
{
	framebuffer->BlitToScreen(0, 0, width, height, 0, 0, dstWidth, dstHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}



bool HelloT5Cube::InitializeContext()
{
	if (!InitializeT5())
		return false;

	cubeShader = MakeOwned<GLW::ShaderProgram>();

	if (!cubeShader->Compile(GetVertexShaderCode(), GetFragmentShaderCode()))
	{
		std::cerr << cubeShader->GetErrorMessage() << std::endl;
		return false;
	}

	/****************************************************************************
	Setup vertex arrays
	*****************************************************************************/

	cubeVertexBuffer = MakeOwned<GLW::Buffer>();
	cubeVertexBuffer->Create();
	cubeVertexBuffer->StoreData(GetCubeVertices());

	cubeVertexArrays = MakeOwned<GLW::VertexArray>();
	cubeVertexArrays->Create();
	cubeVertexArrays->AttachBuffer(*cubeVertexBuffer, cubeShader->GetAttribLocation("vPos"), VertexPC::positionMem);
	cubeVertexArrays->AttachBuffer(*cubeVertexBuffer, cubeShader->GetAttribLocation("vCol"), VertexPC::colorMem);

	/****************************************************************************
	Setup frame buffers for stereo
	*****************************************************************************/


	leftEye.Initialize(defaultWidth, defaultHeight);
	rightEye.Initialize(defaultWidth, defaultHeight);

	float ipd = glasses->GetIpd();

	leftTransform.SetPosition(-ipd / 2.0f,0,0);
	rightTransform.SetPosition(ipd / 2.0f,0,0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool HelloT5Cube::InitializeT5()
{

	context = MakeOwned<T5W::Context>();
	
	auto result = context->Create(windowTitle, "0.0.1");

	if (result != T5_SUCCESS)
	{
		std::cerr << "Error: " << t5GetResultMessage(result) << std::endl;
		return false;
	}

	serviceVersion = context->GetServiceVersion();
	std::cout << "Service version: " << serviceVersion << std::endl;

	auto glassesResult = context->GetConnectedGlasses();

	if (!glassesResult.TryGet(glassesIDList))
	{
		std::cerr << "Error: " << glassesResult.GetErrorMessage() << std::endl;
		return false;
	}

	for (auto glassesID : glassesIDList)
	{
		std::cout << "Found glasses: " << glassesID << std::endl;
	}

	if (glassesIDList.size() > 0)
	{
		if (!ConnectGlasses(glassesIDList[0]))
			return false;
	}
	else
	{
		std::cout << "Didn't find any glasses" << std::endl;
		return false;
	}

	isPreviousPoseValid = false;
	isPoseValid = false;

	isPreviousFrameSent = false;
	isFrameSent = false;


	return true;
}

bool HelloT5Cube::ConnectGlasses(std::string glassesID)
{
	glasses = MakeOwned<T5W::Glasses>();

	auto err = glasses->Create(*context, glassesID);
	if (err != T5_SUCCESS)
	{
		std::cerr << "Error: [" << t5GetResultMessage(err) << "] creating glasses " << glassesID << std::endl;
		return false;
	}

	std::cout << "Created glasses: " << glassesID << std::endl;

	std::cout << "Glasses " << glassesID << " state: " << GetConnectionText(glasses->GetConnectionState()) << std::endl;

	err = glasses->Acquire(windowTitle);
	if (err != T5_SUCCESS)
	{
		std::cerr << "Error: [" << t5GetResultMessage(err) << "] acquiring glasses " << glassesID << std::endl;
		return false;
	}

	std::cout << "Glasses " << glassesID << " state: " << GetConnectionText(glasses->GetConnectionState()) << std::endl;

	for (;;)
	{
		err = glasses->EnsureReady();
		if (err == T5_SUCCESS)
			break;
		if (err == T5_ERROR_TRY_AGAIN)
			continue;
		std::cerr << "Error: [" << t5GetResultMessage(err) << "] readying glasses " << glassesID << std::endl;
		return false;
	}

	std::cout << "Glasses " << glassesID << " state: " << GetConnectionText(glasses->GetConnectionState()) << std::endl;

	err = glasses->InitGlassesOpenGLContext();
	if (err != T5_SUCCESS)
	{
		std::cerr << "Error: [" << t5GetResultMessage(err) << "] initializing OpenGL for glasses " << glassesID << std::endl;
		return false;
	}

	std::cout << "IPD = " << glasses->GetIpd() << std::endl;

	return true;
}

void HelloT5Cube::Update()
{
	UpdateGlassesPose();
	Render();
	SendFramesToGlasses();
}

void HelloT5Cube::UpdateGlassesPose()
{
	isPreviousPoseValid = isPoseValid;

	auto poseResult = glasses->GetGlassesPose();

	T5_GlassesPose pose;
	if (poseResult.TryGet(pose))
	{
		glassesPose.SetPosition(T5W::toGLM(pose.posGLS_GBD));
		glassesPose.SetOrientation(glm::inverse(T5W::toGLM(pose.rotToGLS_GBD)));
		auto pos = glassesPose.GetPosition();
		isPoseValid = true;
	}
	else
	{
		isPoseValid = false;
	}
	if (!isPoseValid && isPreviousPoseValid)
	{
		std::cout << "Lost tracking" << std::endl;
	}
	else if (isPoseValid && !isPreviousPoseValid)
	{
		std::cout << "Gained tracking" << std::endl;
	}
}

void HelloT5Cube::Render()
{
	const float ratio = leftEye.width / (float)leftEye.height;

	GLApp::Transform modelTran;
	modelTran.SetEuler(0, 0, (float)glfwGetTime());
	modelTran.SetScale(0.1f);

	auto view = glm::translate(glm::mat4x4(1), glm::vec3(0.0f, 0.0f, -4.0f));

	auto perspectiveProj = glm::perspective(glm::radians(defaultFOV), ratio, 0.1f, 100.0f);

	auto headViewModel = glassesPose.GetInverseMatrix() * modelTran.GetMatrix();

	cubeShader->Use();
	cubeVertexArrays->Bind();

	auto mvpLeft = perspectiveProj * leftTransform.GetMatrix() * headViewModel;
	cubeShader->Set("MVP", mvpLeft);
	leftEye.BeginDraw();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	leftEye.EndDraw();

	auto mvpRight = perspectiveProj * leftTransform.GetMatrix() * headViewModel;
	cubeShader->Set("MVP", mvpRight);
	rightEye.BeginDraw();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	rightEye.EndDraw();

	int width = 0;
	int height = 0;
	GetFramebufferSize(width, height);
	rightEye.BlitToScreen(width, height);
}

void HelloT5Cube::SendFramesToGlasses()
{
	isPreviousFrameSent = isFrameSent;
	isFrameSent = false;
	T5_Result result;
	if (isPoseValid)
	{
		T5_FrameInfo frameInfo;

		frameInfo.vci.startY_VCI = -tan(glm::radians(defaultFOV) * 0.5f);
		frameInfo.vci.startX_VCI = frameInfo.vci.startY_VCI * leftEye.width / (float)leftEye.height;
		frameInfo.vci.width_VCI = -2.0f * frameInfo.vci.startX_VCI;
		frameInfo.vci.height_VCI = -2.0f * frameInfo.vci.startY_VCI;

		frameInfo.texWidth_PIX = leftEye.width;
		frameInfo.texHeight_PIX = leftEye.height;

		frameInfo.leftTexHandle = leftEye.texture->HandleAsVoidPtr();
		frameInfo.rightTexHandle = rightEye.texture->HandleAsVoidPtr();

		auto leftPos = leftTransform.InverseTranformPosition(glm::vec3(0,0,0));
		leftPos = glassesPose.InverseTranformPosition(leftPos);

		frameInfo.posLVC_GBD = T5W::toT5(leftPos);
		frameInfo.rotToLVC_GBD = T5W::toT5(glm::inverse(glassesPose.GetOrientation()));

		auto rightPos = rightTransform.InverseTranformPosition(glm::vec3(0, 0, 0));
		rightPos = glassesPose.InverseTranformPosition(rightPos);

		frameInfo.posRVC_GBD = T5W::toT5(rightPos);
		frameInfo.rotToRVC_GBD = T5W::toT5(glm::inverse(glassesPose.GetOrientation()));
		frameInfo.isUpsideDown = false;
		frameInfo.isSrgb = false;

		result = glasses->SendFrameToGlasses(frameInfo);
		isFrameSent = (result == T5_SUCCESS);

		if (!isFrameSent && isPreviousFrameSent)
		{
			std::cerr << "Stopped sending frames because: " << t5GetResultMessage(result) << std::endl;
		}
		else if (isFrameSent && !isPreviousFrameSent)
		{
			std::cerr << "Started sending frames" << std::endl;
		}

		frameCounter++;

	}
}
