////////////////
// Filename: systemclass.cpp
////////////////
#include "systemclass.h"

SystemClass::SystemClass()
{
	OutputDebugStringA("|| Starting System ... \n");

	m_OpenGL = 0;
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
	// intentionally empty
}

SystemClass::~SystemClass()
{
	// intentionally empty - clean up happens in Shutdown()
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero
	screenWidth = 0;
	screenHeight = 0;

	OutputDebugStringA("|| Initializing System ... \n");

	// Create OpenGL object.
	m_OpenGL = new OpenGLClass;
	if (!m_OpenGL)
	{
		return false;
	}

	// Create the window the application will be using and also initialize OpenGL
	result = InitializeWindows(m_OpenGL, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, "Could not initialize window.", "Error", MB_OK);
		return false;
	}

	// Create the input object
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}
	m_Input->Initialize();

	// Create graphics object
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object
	result = m_Graphics->Initialize(m_OpenGL, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	OutputDebugStringA("|| Shutting Down System ... \n");

	// Release the graphics object
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Release the OpenGL object
	if (m_OpenGL)
	{
		m_OpenGL->Shutdown(m_hwnd);
		delete m_OpenGL;
		m_OpenGL = 0;
	}

	// Shutdown the window
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	OutputDebugStringA("|| Running System ... \n");

	// Initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out
		if (msg.message == WM_QUIT)
		{
			OutputDebugStringA("|| Message WM_QUIT ... \n");
			done = true;
		}
		else
		{
			// Otherwise do the frame processing
			result = Frame();
			if (!result)
			{
				OutputDebugStringA("|| Frame result false ... \n");
				done = true;
			}
		}
	}

	return;
}

bool SystemClass::Frame()
{
	bool result;

	// Check if the user pressed escape and wants to exit the application
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		OutputDebugStringA("|| User input VK_ESCAPE ... \n");
		return false;
	}

	// Do the frame processing for the graphics object
	result = m_Graphics->Frame();
	if (!result)
	{
		OutputDebugStringA("|| Graphics->Frame() returned false ... \n");
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard
		case WM_KEYUP:
		{
			// If a key is released sent it to the input object etc.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

bool SystemClass::InitializeWindows(OpenGLClass* OpenGL, int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	bool result;

	OutputDebugStringA("|| Initializing Windows in System ... \n");

	// Get an external pointer to this object
	ApplicationHandle = this;

	// Get the instance of this application
	m_Instance = GetModuleHandle(NULL);

	// Give the application a name
	m_applicationName = "Kata 005";

	// Setup the windows class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_Instance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class
	RegisterClassEx(&wc);

	// Create a temporary window for the OpenGL extension setup
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_POPUP,
		0, 0, 640, 480, NULL, NULL, m_Instance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Don't show the window
	ShowWindow(m_hwnd, SW_HIDE);

	// Load the OpenGL extensions
	result = OpenGL->InitializeExtensions(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, "Could not initialize the OpenGL extensions.", "Error", MB_OK);
		return false;
	}

	// Release the temporary window now that the extensions have been initialized
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Determine the resolution of the clients desktop
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit ...
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner;
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window the screen settings and get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_Instance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Initialize OpenGL now that the correct window has been created
	result = m_OpenGL->InitializeOpenGL(m_hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);
	if (!result)
	{
		MessageBox(m_hwnd, "Could not initialize OpenGL, check if video card supports OpenGL 4.0.", "Error", MB_OK);
		return false;
	}

	// Bring the window up on the screen and set it as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor
	ShowCursor(false);
	OutputDebugStringA("|| Success! ");
	return true;
}

void SystemClass::ShutdownWindows()
{
	OutputDebugStringA("|| Shutting Down Windows in System ...");
	// Show the mouse cursoro
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance
	UnregisterClass(m_applicationName, m_Instance);
	m_Instance = NULL;

	// Release the pointer to the class
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}