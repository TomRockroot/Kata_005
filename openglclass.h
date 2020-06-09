//////////////////
// Filename: openglclass.h
//////////////////
#ifndef OPENGLCLASS_H
#define OPENGLCLASS_H

///////////////////
// LINKING
///////////////////
#pragma comment(lib, "opengl32.lib")

//////////////
// INCLUDES
//////////////
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "glext.h"
#include "wglext.h"
#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//////////////////
// Class name: OpenGLClass
//////////////////
class OpenGLClass
{
public:
	OpenGLClass();
	OpenGLClass(const OpenGLClass&);
	~OpenGLClass();

	bool InitializeExtensions(HWND);
	bool InitializeOpenGL(HWND, int, int, float, float, bool);
	void Shutdown(HWND);

	void BeginScene(float, float, float, float);
	void EndScene();

	glm::mat4 GetWorldMatrix();
	glm::mat4 GetProjectionMatrix();

	void SetWorldMatrix(glm::mat4);

	void GetVideoCardInfo(char*);

	void BuildPerspectiveMatrix(glm::mat4, float, float, float, float);

private:
	bool LoadExtensionList();

private:
	HDC m_deviceContext;
	HGLRC m_renderingContext;

	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

	glm::mat4 m_worldMatrix = glm::mat4(1.0f);
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
	char m_videoCardDescription[128];

public:
	PFNGLATTACHSHADERPROC glAttachShader = 0;
	PFNGLBINDBUFFERPROC glBindBuffer = 0;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0; 
	
	PFNGLBUFFERDATAPROC glBufferData = 0;
	PFNGLCOMPILESHADERPROC glCompileShader = 0;
	PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
	PFNGLCREATESHADERPROC glCreateShader = 0;

	PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
	PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
	PFNGLDELETESHADERPROC glDeleteShader = 0;
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
	
	PFNGLDETACHSHADERPROC glDetachShader = 0;
	PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray = 0;
	PFNGLGENBUFFERSPROC glGenBuffers = 0;
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;

	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
	PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
	PFNGLGETSHADERIVPROC glGetShaderiv = 0;

	PFNGLLINKPROGRAMPROC glLinkProgram = 0;
	PFNGLSHADERSOURCEPROC glShaderSource = 0;
	PFNGLUSEPROGRAMPROC glUseProgram = 0;

	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
	PFNGLACTIVETEXTUREPROC glActiveTexture = 0;

	PFNGLUNIFORM1IPROC glUniform1i = 0;
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;

	PFNGLUNIFORM3FVPROC glUniform3fv = 0;
	PFNGLUNIFORM4FVPROC glUniform4fv = 0;

	// Not part of the tutorial
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
};

#endif // OPENGLCLASS_H
