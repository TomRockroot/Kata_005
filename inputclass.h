//////////////
// Filename: inputclass.h
//////////////

#ifndef INPUTCLASS_H
#define INPUTCLASS_H

//////////////
// INCLUDES
//////////////
#include <windows.h>

////////////////////////
// Class name: InputClass
////////////////////////
class InputClass
{
public: 
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[265];
};

#endif // INPUTCLASS_H