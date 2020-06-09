/////////////////////
// Tutorial by RenderTek.com/gl40tut02.html
// Filename: main.cpp
/////////////////////
#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	OutputDebugStringA("|| Starting Program ... \n");

	// Create the system object
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// Initialize and run the system object
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object
	System->Shutdown();
	delete System;
	System = 0;

	OutputDebugStringA("|| Program Done ...\n");

	return 0;
}