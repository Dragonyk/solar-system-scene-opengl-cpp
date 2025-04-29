#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture(){
}

void Texture::bind(){
	if (texType == 2)
		glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::release(){
	if(texType == 2)
		glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Setup2D(char* filename, GLint wrapping, GLint filtering, GLint colorFormat){
	texType = 2;
	glGenTextures(1, &texture);
	//Vincula as texturas(texture)
	glBindTexture(GL_TEXTURE_2D, texture);

	//Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	//Carrega imagem da textura
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

	if (data) {
		//Gera uma textura para os dados carregados(data)
		glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//Libera a imagem da memoria(boa pratica)
	stbi_image_free(data);
}