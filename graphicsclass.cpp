/////////////////
// Filename: graphicsclass.cpp
/////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	OutputDebugStringA("|| Starting Graphics Class ... \n");
	m_OpenGL = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(OpenGLClass* openGL, HWND hwnd)
{
	bool result;
	OutputDebugStringA("|| Initializing Graphics Class ... \n");
	m_OpenGL = openGL;

	// Create the camera object
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera
	m_Camera->SetPosition(0.0f, 2.0f, 10.0f);
	
// Create the model object
	m_Model = new ModelClass;
	if (!m_Model)
	{
		OutputDebugStringA("XX Failed to create model object ...\n");
		return false;
	}

	// Initialize the model object
	result = m_Model->Initialize(m_OpenGL);
	if (!result)
	{
		OutputDebugStringA("XX Failed to Initialize() model ...\n");
		return false;
	}

	// Create the color shader object
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		OutputDebugStringA("XX Failed to create ColorShaderObject ... \n");
		return false;
	}

	// Initialize the color shader object
	result = m_ColorShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		OutputDebugStringA("XX Failed to Initialize() color shader ... \n");
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	OutputDebugStringA("|| Shutting Down Graphics Class ... \n");

	if (m_ColorShader)
	{
		m_ColorShader->Shutdown(m_OpenGL);
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown(m_OpenGL);
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	m_OpenGL = 0;

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	m_OpenGL->SetWorldMatrix(glm::rotate( m_OpenGL->GetWorldMatrix(), 0.1f, glm::vec3(0.0f, 1.0f, 0.0f)));

	// Render the graphics scene
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	glm::mat4 worldMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	// Clear the buffers
	m_OpenGL->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

	// Generate the view matrix based on the camera's position
	m_Camera->Render();

	// Get the world, view and projection matrices from opengl and camera objects
	worldMatrix = m_OpenGL->GetWorldMatrix();
	viewMatrix = m_Camera->GetViewMatrix();
	projectionMatrix =m_OpenGL->GetProjectionMatrix();

	/*
	projectionMatrix = glm::mat4(glm::vec4(1.8106f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 2.4142f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0001f, 1.0f),
		glm::vec4(0.0f, 0.0f, -0.1f, 0.0f));
		*/

	// Set the color shader as the current shader program and set the matrices that it will use for rendering
	m_ColorShader->SetShader(m_OpenGL);
	if (!m_ColorShader->SetShaderParameters(m_OpenGL, worldMatrix, viewMatrix, projectionMatrix))
	{
		OutputDebugStringA("WHAT?!");
	}

	// Render the model
	m_Model->Render(m_OpenGL);

	// Present the rendered scene to the screen
	m_OpenGL->EndScene();

	return true;
}