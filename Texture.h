#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/glew.h>

#include <iostream>
#include <string>


#include "stb_image.h"

using namespace std;

class Texture{
public:
	GLuint texture;
	unsigned int texType = 2;

	Texture();
	void bind();
	void release();
	void Setup2D(char* filename, GLint wrapping, GLint filtering, GLint colorFormat);
private:

};
#endif