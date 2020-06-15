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
	vertexShaderBuffer = LoadShaderSourceFile(vsFilename);
	if (!vertexShaderBuffer)
	{
		OutputDebugStringA("XX Failed to load VS shader source into buffer ... \n");
		return false;
	}
	// Load the fragment shader source file into a text buffer
	fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
	if (!fragmentShaderBuffer)
	{
		OutputDebugStringA("XX Failed to load FS shader source into buffer ... \n");
		return false;
	}

	// Create a vertex and fragment shader object
	m_vertexShader = OpenGL->glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = OpenGL->glCreateShader(GL_FRAGMENT_SHADER);

	// Copy the shader source code strings into the vertex and fragment shader objects
	OpenGL->glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
	OpenGL->glShaderSource(m_fragmentShader, 1, &fragmentShaderBuffer, NULL);

	// Release the vertex and fragment shader buffers
	delete[] vertexShaderBuffer;
	vertexShaderBuffer = 0;

	delete[] fragmentShaderBuffer;
	fragmentShaderBuffer = 0;
	
	// Compile the shaders
	OpenGL->glCompileShader(m_vertexShader);
	OpenGL->glCompileShader(m_fragmentShader);

	// Check to see if the vertex shader compiled successfully
	OpenGL->glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		// If it did not compile write the syntax error message out to a text file for review
		OutputShaderErrorMessage(OpenGL, hwnd, m_vertexShader, vsFilename);
		return false;
	}

	// Check to see if the frag shader compiled successfully
	OpenGL->glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		// If not, write syntax error message to text file
		OutputShaderErrorMessage(OpenGL, hwnd, m_fragmentShader, fsFilename);
		return false;
	}

	// Create a shader program object
	m_shaderProgram = OpenGL->glCreateProgram();

	// Attach the vertex and fragment shader to the program object
	OpenGL->glAttachShader(m_shaderProgram, m_vertexShader);
	OpenGL->glAttachShader(m_shaderProgram, m_fragmentShader);

	// Bind shader input variables
	OpenGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
	OpenGL->glBindAttribLocation(m_shaderProgram, 1, "inputTexCoord");

	// Link the program
	OpenGL->glLinkProgram(m_shaderProgram);

	// Check status of the link
	OpenGL->glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		// if not successful, write error to a text file
		OutputLinkerErrorMessage(OpenGL, hwnd, m_shaderProgram);
		return false;
	}

	return true;
}

char* TextureShaderClass::LoadShaderSourceFile(const char* filename)
{
	ifstream fin;
	int fileSize;
	char input;
	char* buffer;

	// Open shader source file

	// If could not open, exit

	// Init the size of the file

	// Read the first element in the text file

	// Count the number of elements in the text file

	// Close the file for now

	// Initialize the buffer to read the shader source file into

	// Open the source file again

	// Read the shader text file into the buffer as a block

	// Close the file

	// Null terminate the buffer

	return buffer;
}