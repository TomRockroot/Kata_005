/////////////////
// Filename: colorshaderclass.h
/////////////////
#ifndef COLORSHADERCLASS_H
#define COLORSHADERCLASS_H

////////////////
// INCLUDES
////////////////
#include <fstream>
using namespace std;

/////////////////
// MY CLASS INCLUDES
/////////////////
#include "openglclass.h"

///////////////////////
// Class name: ColorShaderClass
///////////////////////
class ColorShaderClass
{
public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown(OpenGLClass*);
	void SetShader(OpenGLClass*);
	bool SetShaderParameters(OpenGLClass*, glm::mat4, glm::mat4, glm::mat4);

private:
	bool InitializeShader(const char*, const char*, OpenGLClass*, HWND);
	char* LoadShaderSourceFile(const char*);
	void OutputShaderErrorMessage(OpenGLClass*, HWND, unsigned int, const char*);
	void OutputLinkerErrorMessage(OpenGLClass*, HWND, unsigned int);
	void ShutdownShader(OpenGLClass*);

private:
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_shaderProgram;
};

#endif // COLORSHADERCLASS