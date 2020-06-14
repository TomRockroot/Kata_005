/////////////////////
// Filename: textureshaderclass.cpp
/////////////////////
#include "textureshaderclass.h"

TextureShaderClass::TextureShaderClass()
{
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass&)
{
}

TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(OpenGLClass* OpenGL, HWND hwnd)
{
	bool result;

	OutputDebugStringA("|| Initializing TextureShaderClass ... \n");

	// Initialize vert and frag shader
	result = InitializeShader("../Kata_005/shaders/texture.vs", "../Kata_005/shaders/texture.ps", OpenGL, hwnd);
	if (!result)
	{
		OutputDebugStringA("XX Failed to initialize shaders ... \n");
		return false;
	}

	return true;
}

void TextureShaderClass::Shutdown(OpenGLClass* OpenGL)
{
	ShutdownShader(OpenGL);

	return;
}

void TextureShaderClass::SetShader(OpenGLClass* OpenGL)
{
	// Install the sahder program as part of current rendering state
	OpenGL->glUseProgram(m_shaderProgram);

	return;
}

bool TextureShaderClass::InitializeShader(const char* vsFilename, const char* fsFilename, OpenGLClass* OpenGL, HWND hwnd)
{
	const char* vertexShaderBuffer;
	const char* fragmentShaderBuffer;
	int status;

	// Load the vertex shader source file into a text buffer

	// Load the fragment shader source file into a text buffer

	// Create a vertex and fragment shader object

	// Copy the shader source code strings into the vertex and fragment shader objects

	// Release the vertex and fragment shader buffers

	// Compile the shaders

	// Check to see if the vertex shader compiled successfully
		// If it did not compile write the syntax error message out to a text file for review

	// Check to see if the frag shader compiled successfully
		// If not, write syntax error message to text file

	// Create a shader program object

	// Attach the vertex and fragment shader to the program object

	// Link the program

	// Check status of the link
		// if not successful, write error to a text file


	return true;
}