/////////////
// Filename: textureclass.cpp
/////////////
#include "textureclass.h"

TextureClass::TextureClass()
{
	loaded = false;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(OpenGLClass* OpenGL, char* filename, unsigned int textureUnit, bool wrap)
{
	bool result;

	OutputDebugStringA("|| Initializing TextureClass ... \n");

	// Load the targa file
	result = LoadTarga(OpenGL, filename, textureUnit, wrap);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	// Release a loaded texture on shutdown
	if (loaded)
	{
		glDeleteTextures(1, &m_textureID);
		loaded = false;
	}

	return;
}

bool TextureClass::LoadTarga(OpenGLClass* OpenGL, char* filename, unsigned int textureUnit, bool wrap)
{
	int error, width, height, bpp, imageSize;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	OutputDebugStringA("|| Loading Targa Image... \n");

	// Open the targa file for binary reading
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		OutputDebugStringA("XX Failed to load file ... \n");
		return false;
	}

	// Read in file header
	count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		OutputDebugStringA("XX Failed to read header ... \n");
		return false;
	}

	// Get the info from the header
	width = (int)targaFileHeader.width;
	height = (int)targaFileHeader.height;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit, not 64 bit
	if (bpp != 32)
	{
		OutputDebugStringA("XX Fileformat is not 32 bits as intended ... \n");
		return false;
	}

	// Calculate the size of the 32 bit image data
	imageSize = width * height * 4;

	// Alloc memory for targa image data
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		OutputDebugStringA("XX Unable to allocate memory ... \n");
		return false;
	}

	// Read in the targa image data
	count = fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		OutputDebugStringA("XX Image data size differs from calculated size ... \n");
		return false;
	}

	// Close the file
	error = fclose(filePtr);
	if (error != 0)
	{
		OutputDebugStringA("XX Unable to close the file ... \n");
		return false;
	}

	// Set the unique texture unit in which to store the data
	OpenGL->glActiveTexture(GL_TEXTURE0 + textureUnit);

	// Generate an ID for the texture
	glGenTextures(1, &m_textureID);

	// Bind the texture as a 2D texture
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// Load the image data into the texture unit
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, targaImage);

	// Set the texture color to either wrap around or clamp to edge
	if (wrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	// Set the texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate mipmaps for the texture
	OpenGL->glGenerateMipmap(GL_TEXTURE_2D);

	// Release the targa image data
	delete[] targaImage;
	targaImage = 0;

	// Set that the texture is loaded
	loaded = true;

	return true;
}