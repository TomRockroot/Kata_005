///////////////////
// Filename: systemclass.h
///////////////////
#ifndef SYSTEMCLASS_H
#define SYSTEMCLASS_H

//////////////////
// PRE-PROCESSING DIRECTIVES
//////////////////
#define WIN32_LEAN_AND_MEAN

//////////////////
// INCLUDES
//////////////////
#include <windows.h>

//////////////////
// MY CLASS INCLUDES
//////////////////
#include "openglclass.h"
#include "inputclass.h"
#include "graphicsclass.h"

//////////////////////////////////
// Class name: SystemClass
//////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool InitializeWindows(OpenGLClass*, int&, int&);
	void ShutdownWindows();

private:
	LPCSTR m_applicationName;
	HINSTANCE m_Instance;
	HWND m_hwnd;

	OpenGLClass* m_OpenGL;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

////////////////
// FUNCTION PROTOTYPES
////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/////////////
// GLOBALS
////////////
static SystemClass* ApplicationHandle = 0;

#endif // SYSTEMCLASS_H