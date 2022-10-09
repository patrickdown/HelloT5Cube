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

	std::cout << "Press P to dump pose." << std::endl;

	return true;
}

bool HelloT5Cube::DisplaySurface::Initialize(int width, int height)
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

void HelloT5Cube::DisplaySurface::BeginDraw()
{
	framebuffer->Bind(GL_DRAW_FRAMEBUFFER);
	glViewport(0, 0, width, height);
}

void HelloT5Cube::DisplaySurface::EndDraw()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void HelloT5Cube::DisplaySurface::BlitToScreen(int dstWidth, int dstHeight)
{
	framebuffer->BlitToScreen(0, 0, width, height, 0, 0, dstWidth, dstHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}



bool HelloT5Cube::InitializeContext()
{
	if (!InitializeT5())
		return false;

	/****************************************************************************
	Compile shaders
	*****************************************************************************/

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

	leftEyeDisplay.Initialize(defaultWidth, defaultHeight);
	rightEyeDisplay.Initialize(defaultWidth, defaultHeight);

	float ipd = glasses->GetIpd();

	leftEyePose.SetPosition(ipd / 2.0f,0,0);
	rightEyePose.SetPosition(-ipd / 2.0f,0,0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void HelloT5Cube::OnKey(int key, int scancode, int action, int mods)
{
	Application::OnKey(key, scancode, action, mods);
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		isOutputingOnePoseFrame = true;
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
	auto poseResult = glasses->GetGlassesPose();

	T5_GlassesPose pose;
	if (poseResult.TryGet(pose))
	{
		isPoseValid = true;

		auto position = T5W::toGLM(pose.posGLS_GBD);
		auto orientation = T5W::toGLM(pose.rotToGLS_GBD);

		headPose.SetPosition(position);
		headPose.SetOrientation(orientation);

		if (isOutputingOnePoseFrame)
		{
			auto xAxis = glm::rotate(orientation, glm::vec3(1, 0, 0));
			auto yAxis = glm::rotate(orientation, glm::vec3(0, 1, 0));
			auto zAxis = glm::rotate(orientation, glm::vec3(0, 0, 1));

			std::cout << "Pos  = (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
			std::cout << "xAxis =  (" << xAxis.x << ", " << xAxis.y << ", " << xAxis.z << ")" << std::endl;
			std::cout << "yAxis =  (" << yAxis.x << ", " << yAxis.y << ", " << yAxis.z << ")" << std::endl;
			std::cout << "zAxis =  (" << zAxis.x << ", " << zAxis.y << ", " << zAxis.z << ")" << std::endl;

			isOutputingOnePoseFrame = false;
		}
	}
	else
	{
		isPoseValid = false;
	}
	if (isPoseValid.IsChanged())
	{
		std::cout << (isPoseValid ? "Gained tracking" : "Lost tracking") << std::endl;
	} 
}

void HelloT5Cube::Render()
{
	const float ratio = leftEyeDisplay.width / (float)leftEyeDisplay.height;

	// Rotate cube
	GLApp::Transform modelTran;
	modelTran.SetEuler(0, 0, (float)glfwGetTime());
	modelTran.SetScale(0.1f);

	// Model to world
	auto modelMat = modelTran.MatrixToParentFrame();
	// World to eye
	auto viewLeftMat = leftEyePose.MatrixToLocalFrame() * headPose.MatrixToLocalFrame() * gameboardPose.MatrixToLocalFrame();
	auto viewRightMat = rightEyePose.MatrixToLocalFrame() * headPose.MatrixToLocalFrame() * gameboardPose.MatrixToLocalFrame();
	// Perspective projection
	auto perspectiveProj = glm::perspective(glm::radians(defaultFOV), ratio, 0.1f, 100.0f);

	cubeShader->Use();
	cubeVertexArrays->Bind();

	// Render left eye
	auto mvpLeft = perspectiveProj * viewLeftMat * modelMat;

	cubeShader->Set("MVP", mvpLeft);
	leftEyeDisplay.BeginDraw();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	leftEyeDisplay.EndDraw();

	// Render left eye
	auto mvpRight = perspectiveProj * viewRightMat * modelMat;

	cubeShader->Set("MVP", mvpRight);
	rightEyeDisplay.BeginDraw();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	rightEyeDisplay.EndDraw();

	// Blit right eye to display
	int width = 0;
	int height = 0;
	GetFramebufferSize(width, height);
	rightEyeDisplay.BlitToScreen(width, height);
}

void HelloT5Cube::SendFramesToGlasses()
{
	T5_Result result;
	if (isPoseValid)
	{
		T5_FrameInfo frameInfo;

		frameInfo.vci.startY_VCI = -tan(glm::radians(defaultFOV) * 0.5f);
		frameInfo.vci.startX_VCI = frameInfo.vci.startY_VCI * leftEyeDisplay.width / (float)leftEyeDisplay.height;
		frameInfo.vci.width_VCI = -2.0f * frameInfo.vci.startX_VCI;
		frameInfo.vci.height_VCI = -2.0f * frameInfo.vci.startY_VCI;

		frameInfo.texWidth_PIX = leftEyeDisplay.width;
		frameInfo.texHeight_PIX = leftEyeDisplay.height;

		frameInfo.leftTexHandle = leftEyeDisplay.texture->HandleAsVoidPtr();
		frameInfo.rightTexHandle = rightEyeDisplay.texture->HandleAsVoidPtr();

		auto leftPos = leftEyePose.TransformPointToParentFrame(glm::vec3(0,0,0));
		leftPos = headPose.TransformPointToParentFrame(leftPos);

		frameInfo.posLVC_GBD = T5W::toT5(leftPos);
		frameInfo.rotToLVC_GBD = T5W::toT5(headPose.GetOrientation());

		auto rightPos = rightEyePose.TransformPointToParentFrame(glm::vec3(0, 0, 0));
		rightPos = headPose.TransformPointToParentFrame(rightPos);

		frameInfo.posRVC_GBD = T5W::toT5(rightPos);
		frameInfo.rotToRVC_GBD = T5W::toT5(headPose.GetOrientation());
		frameInfo.isUpsideDown = false;
		frameInfo.isSrgb = false;

		result = glasses->SendFrameToGlasses(frameInfo);
		isFrameSent = (result == T5_SUCCESS);

		if (isFrameSent.IsChanged())
		{ 
			if(!isFrameSent)
			{
				std::cerr << "Stopped sending frames because: " << t5GetResultMessage(result) << std::endl;
			}
			else 
			{
				std::cerr << "Started sending frames" << std::endl;
			}
		}
		frameCounter++;
	}
}
