/////////////////
// Filename:
/////////////////
#include "inputclass.h"

InputClass::InputClass()
{
	OutputDebugStringA("|| Starting InputClass ... \n");
}

InputClass::InputClass(const InputClass&)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	OutputDebugStringA("|| Initializing InputClass ... \n");
	int i;

	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}