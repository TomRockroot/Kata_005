////////////////////////
// Filename: openglclass.cpp
////////////////////////
#include "openglclass.h"

OpenGLClass::OpenGLClass()
{
	OutputDebugStringA("|| Starting OpenGLClass ... \n");
	m_deviceContext = 0;
	m_renderingContext = 0;
}

OpenGLClass::OpenGLClass(const OpenGLClass&)
{
}

OpenGLClass::~OpenGLClass()
{
}

bool OpenGLClass::InitializeExtensions(HWND hwnd)
{
	HDC deviceContext;
	PIXELFORMATDESCRIPTOR pixelFormat;
	int error;
	HGLRC renderContext;
	bool result;

	OutputDebugStringA("|| Initializing OpenGL Extensions ... \n");

	// Get the device context for this window
	deviceContext = GetDC(hwnd);
	if (!deviceContext)
	{
		OutputDebugStringA("XX Failed to GetDC(hwnd) ... \n");
		return false;
	}

	// Set a temporary default pixel format
	error = SetPixelFormat(deviceContext, 1, &pixelFormat);
	if (error != 1)
	{
		OutputDebugStringA("XX Failed to SetPixelFormat() ... \n");
		return false;
	}

	// Create temporary rendering context
	renderContext = wglCreateContext(deviceContext);
	if (!renderContext)
	{
		OutputDebugStringA("XX Failed to wglCreateContext() ... \n");
		return false;
	}

	// Set the temporary rendering context as the current rendering context for this window
	error = wglMakeCurrent(deviceContext, renderContext);
	if (error != 1)
	{
		OutputDebugStringA("XX Failed to wglMakeCurrent() ... \n");
		return false;
	}

	// Initialize the OpenGL extensions needed for this application. (Note: temporary context needed to do so)
	result = LoadExtensionList();
	if (!result)
	{
		OutputDebugStringA("XX Failed to LoadExtensionList() \n");
		return false;
	}

	// Release the temporary rendering context now that the extensions have been loaded
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(renderContext);
	renderContext = NULL;

	// Release the device context for this window
	ReleaseDC(hwnd, deviceContext);
	deviceContext = 0;

	OutputDebugStringA("|| Success! \n");
	return true;
}

bool OpenGLClass::InitializeOpenGL(HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear, bool vsync)
{
	int attributeListInt[19];
	int pixelFormat[1];
	unsigned int formatCount;
	int result;
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	int attributeList[5];
	float fieldOfView, screenAspect;
	char* vendorString, * rendererString;

	// Get the device context for this window
	m_deviceContext = GetDC(hwnd);
	if (!m_deviceContext)
	{
		return false;
	}

	// Support for OpenGL rendering
	attributeListInt[0] = WGL_SUPPORT_OPENGL_ARB;
	attributeListInt[1] = TRUE;

	// Support for OpenGL rendering to a window
	attributeListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
	attributeListInt[3] = TRUE;

	// Support for hardware acceleration
	attributeListInt[4] = WGL_ACCELERATION_ARB;
	attributeListInt[5] = WGL_FULL_ACCELERATION_ARB;

	// Support for 24bit color
	attributeListInt[6] = WGL_COLOR_BITS_ARB;
	attributeListInt[7] = 24;

	// Support for 24 bit depth buffer
	attributeListInt[8] = WGL_DEPTH_BITS_ARB;
	attributeListInt[9] = 24;

	// Support for double buffer
	attributeListInt[10] = WGL_DOUBLE_BUFFER_ARB;
	attributeListInt[11] = TRUE;

	// Support for swapping front and back buffer
	attributeListInt[12] = WGL_SWAP_METHOD_ARB;
	attributeListInt[13] = WGL_SWAP_EXCHANGE_ARB;

	// Support for the RGBA pixel type
	attributeListInt[14] = WGL_PIXEL_TYPE_ARB;
	attributeListInt[15] = WGL_TYPE_RGBA_ARB;

	// Support for a 8 bit stencil buffer
	attributeListInt[16] = WGL_STENCIL_BITS_ARB;
	attributeListInt[17] = 8;

	// Null terminate the attribute list
	attributeListInt[18] = 0;

	// Query for a pixel format that fits the device attributes we want
	result = wglChoosePixelFormatARB(m_deviceContext, attributeListInt, NULL, 1, pixelFormat, &formatCount);
	if (result != 1)
	{
		OutputDebugStringA("XX Failed to ChoosePixelFormatARB() ... \n");
		return false;
	}

	// If the video card/display can handle our desired pixel format then we set it as the current one
	result = SetPixelFormat(m_deviceContext, pixelFormat[0], &pixelFormatDescriptor);
	if (result != 1)
	{
		OutputDebugStringA("XX Failed to SetPixelFormat() ... \n");
		return false;
	}

	// Set the 4.0 version of OpenGL in the attribute list
	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;

	// Null terminate the attribute list
	attributeList[4] = 0;

	// Create a OpenGL 4.0 rendereing context
	m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, 0, attributeList);
	if (m_renderingContext == NULL)
	{
		OutputDebugStringA("XX Unable to create a m_renderingContext ... \n");
		return false;
	}

	// Set the rendering context to active
	result = wglMakeCurrent(m_deviceContext, m_renderingContext);
	if (result != 1)
	{
		return false;
	}

	// Set the depth buffer to be entirely cleared to 1.0 values
	glClearDepth(1.0f);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Set the polygon winding to front facing for the left handed system
	glFrontFace(GL_CW);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Set the field of view and screen aspect ratio
	fieldOfView = 3.14159265358979323846f / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Build the perspective projection matrix
	m_projectionMatrix = glm::perspective(fieldOfView, screenAspect, screenNear, screenDepth);


	// Get the name of the video card
	vendorString = (char*)glGetString(GL_VENDOR);
	rendererString = (char*)glGetString(GL_RENDERER);

	// Store the video card name in a class member to be retrieved later
	strcpy_s(m_videoCardDescription, vendorString);
	strcat_s(m_videoCardDescription, "-");
	strcat_s(m_videoCardDescription, rendererString);

	// Turn on or off vertical sync
	if (vsync)
	{
		result = wglSwapIntervalEXT(1);
	}
	else
	{
		result = wglSwapIntervalEXT(0);
	}

	// Check if vsync was set correctly
	if (result != 1)
	{
		OutputDebugStringA("XX Failed to set vsync \n");
		return false;
	}

	return true;
}

void OpenGLClass::Shutdown(HWND hwnd)
{
	// Release the rendering context
	if (m_renderingContext)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_renderingContext);
		m_renderingContext = 0;
	}
	
	// Release the device context
	if (m_deviceContext)
	{
		ReleaseDC(hwnd, m_deviceContext);
		m_deviceContext = 0;
	}

	return;
}

void OpenGLClass::BeginScene(float red, float green, float blue, float alpha)
{
	// Set the color to clear the screen to
	glClearColor(red, green, blue, alpha);

	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return;
}

void OpenGLClass::EndScene()
{
	SwapBuffers(m_deviceContext);

	return;
}

bool OpenGLClass::LoadExtensionList()
{
	OutputDebugStringA("|| Loading OpenGL Extensions ... \n");

	// Load the OpenGL extensions that this application will be using
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglChoosePixelFormatARB)
	{
		OutputDebugStringA("1");
		return false;
	}

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB)
	{
		OutputDebugStringA("2");
		return false;
	}

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (!wglSwapIntervalEXT)
	{
		OutputDebugStringA("3");
		return false;
	}

	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	if (!glAttachShader)
	{
		OutputDebugStringA("4");
		return false;
	}

	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	if (!glBindBuffer)
	{
		OutputDebugStringA("5");
		return false;
	}

	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	if (!glBindVertexArray)
	{
		OutputDebugStringA("6");
		return false;
	}

	glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferData");
	if (!glBufferData)
	{
		OutputDebugStringA("7");
		return false;
	}

	glCompileShader = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShader");
	if (!glCompileShader)
	{
		OutputDebugStringA("8");
		return false;
	}

	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	if (!glCreateProgram)
	{
		OutputDebugStringA("9");
		return false;
	}

	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	if (!glCreateShader)
	{
		OutputDebugStringA("10");
		return false;
	}

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	if (!glDeleteBuffers)
	{
		OutputDebugStringA("11");
		return false;
	}

	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	if (!glDeleteProgram)
	{
		OutputDebugStringA("12");
		return false;
	}

	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	if (!glDeleteShader)
	{
		OutputDebugStringA("13");
		return false;
	}

	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	if (!glDeleteVertexArrays)
	{
		OutputDebugStringA("14");
		return false;
	}

	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	if (!glDetachShader)
	{
		OutputDebugStringA("15");
		return false;
	}

	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	if (!glEnableVertexAttribArray)
	{
		OutputDebugStringA("16");
		return false;
	}

	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	if (!glGenBuffers)
	{
		OutputDebugStringA("17");
		return false;
	}

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	if (!glGenVertexArrays)
	{
		OutputDebugStringA("18");
		return false;
	}

	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	if (!glGetAttribLocation)
	{
		OutputDebugStringA("19");
		return false;
	}

	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	if (!glGetProgramInfoLog)
	{
		OutputDebugStringA("20");
		return false;
	}

	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	if (!glGetProgramiv)
	{
		OutputDebugStringA("21");
		return false;
	}

	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	if (!glGetShaderInfoLog)
	{
		OutputDebugStringA("22");
		return false;
	}

	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	if (!glGetShaderiv)
	{
		OutputDebugStringA("23");
		return false;
	}

	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	if (!glLinkProgram)
	{
		OutputDebugStringA("24");
		return false;
	}

	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	if (!glShaderSource)
	{
		OutputDebugStringA("25");
		return false;
	}

	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	if (!glUseProgram)
	{
		OutputDebugStringA("26");
		return false;
	}

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	if (!glVertexAttribPointer)
	{
		OutputDebugStringA("27");
		return false;
	}

	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	if (!glBindAttribLocation)
	{
		OutputDebugStringA("28");
		return false;
	}

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	if (!glGetUniformLocation)
	{
		OutputDebugStringA("29");
		return false;
	}

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	if (!glActiveTexture)
	{
		OutputDebugStringA("30");
		return false;
	}

	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	if (!glUniform1i)
	{
		OutputDebugStringA("31");
		return false;
	}
	
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	if (!glGenerateMipmap)
	{
		OutputDebugStringA("32");
		return false;
	}

	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	if (!glDisableVertexAttribArray)
	{
		OutputDebugStringA("33");
		return false;
	}

	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	if (!glUniform3fv)
	{
		OutputDebugStringA("34");
		return false;
	}

	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	if (!glUniform4fv)
	{
		OutputDebugStringA("35");
		return false;
	}

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	if (!glUniformMatrix4fv)
	{
		OutputDebugStringA("36");
	}

	return true;
}

glm::mat4 OpenGLClass::GetWorldMatrix()
{
	return m_worldMatrix;
}

glm::mat4 OpenGLClass::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void OpenGLClass::SetWorldMatrix(glm::mat4 matrix)
{
	m_worldMatrix = matrix;
}

void OpenGLClass::GetVideoCardInfo(char* cardName)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	return;
}
