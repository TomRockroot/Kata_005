/////////////////
// Filename: textureshaderclass.h
/////////////////
#ifndef TEXTURESHADERCLASS_H
#define TEXTURESHADERCLASS_H

//////////////
// INCLUDES
//////////////
#include <fstream>
using namespace std;
//////////////
// MY CLASS INCLUDES
//////////////
#include "openglclass.h"

////////////////
// Class name: TextureShaderClass
////////////////

class TextureShaderClass
{
public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown(OpenGLClass*);
	void SetShader(OpenGLClass*);
	bool SetShaderParameters(OpenGLClass*, float*, float*, float*);

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

#endif // TEXTURESHADERCLASS_H