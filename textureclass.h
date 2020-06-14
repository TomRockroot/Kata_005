//////////////
// Filename: textureclass.h
//////////////
#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

//////////////
// INCLUDES
//////////////
#include <stdio.h>

///////////////
// MY CLASS INCLUDES
///////////////
#include "openglclass.h"

////////////////
// Class name: Texture Class
////////////////
class TextureClass
{
private:
	// Image format .tga
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(OpenGLClass*, char*, unsigned int, bool);
	void Shutdown();

private:
	bool LoadTarga(OpenGLClass*, char*, unsigned int, bool);

private:
	bool loaded;
	unsigned int m_textureID;
};

#endif // TEXTURECLASS_H