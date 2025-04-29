#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>

#include "Texture.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Object {

public:
	bool setPathTx = false;
	bool isLight = false;

	int ID;

	struct Material
	{
		unsigned int matNumber;
		std::string matName;

		float Ns;
		float Ni, d;
		vector<int> Tf;
		int illum;
		vector<float> Ka;
		vector<float> Kd;
		vector<float> Ks;
		std::string map_Kd;
		Texture texture;

		glm::vec3 color;
	};

	vector<glm::vec3> vVertices;
	vector<glm::vec2> vTextures;
	vector<glm::vec3> vNormals;

	struct IndexStruct{
		int position;
		int texture;
		int normal;
	};

	struct Face
	{
		//0/0/0 1/1/1 2/2/2
		IndexStruct indices[3];
	};

	struct Mesh {
		vector<float> vertices;
		vector<float> textures;
		vector<float> normals;

		vector<Face> faces;

		GLuint VAO, vVBO, tVBO, nVBO;

		//Default -1 dont has
		GLint ID_material = -1;
	};

	Material default_material;

	vector<Material> materials;
	vector<Mesh> meshs;
	vector<Face> faces;

	GLuint curMaterial = 0;
	GLuint curMesh = 0;

	GLuint ModelLocation;
	unsigned int program_ID;
	glm::mat4 modelTransform;

	//Construtor inicial do Objeto
	Object();
	
	//Define VAO para cada mesh do objeto carregado
	//progID define o shader que esta sendo usado
	void SetupVAO(GLuint progID);

	void setModelLocation(GLuint trfLoc);

	void transform(glm::mat4 transformation);
	//Rendeniza o objeto
	void render();
	//Regenera as meshs replicandos vertices
	void RegenerateMeshs();
	//Carrega o arquivo .obj
	void LoadObject(char * filepath);
	//Busca o ID de um material a partir do nome
	GLint FindIDMaterial(string materialName);
	//Carrega todos os materiais a partir do .mtl carregado no .obj
	void LoadMaterial(char * filepath);
	//Verifica uma cadeira de caracteres em um string
	bool CheckEntry(char * entrycheck, const std::string & linecheck);
	//Limpa o VAO e os VBOs
	void clear();
private:

	vector<std::string> split(const std::string &s, char delim);
	void removeCharsFromString(string & str, char * charsToRemove);
	template<typename Out>
	void split(const std::string &s, char delim, Out result);
};

#endif