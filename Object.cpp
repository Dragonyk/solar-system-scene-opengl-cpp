#include "Object.h"

Object::Object() {
}

//Setup VAO of Meshs
void Object::SetupVAO(GLuint progID) {
	program_ID = progID;

	ModelLocation = glGetUniformLocation(program_ID, "model");

	default_material.Ka.push_back(0.5f);
	default_material.Ka.push_back(0.5f);
	default_material.Ka.push_back(0.5f);

	default_material.Kd.push_back(0.9f);
	default_material.Kd.push_back(0.9f);
	default_material.Kd.push_back(0.9f);

	default_material.Ks.push_back(0.0f);
	default_material.Ks.push_back(0.0f);
	default_material.Ks.push_back(0.0f);

	default_material.Ns = 10;

	default_material.color = glm::vec3(0.0f, 0.5f, 0.0f);

	std::cout << "Meshs: " << meshs.size() << endl;
	for (unsigned int i = 0; i < meshs.size(); i++) {

		glGenVertexArrays(1, &meshs[i].VAO);
		glBindVertexArray(meshs[i].VAO);

		//Vertex
		glGenBuffers(1, &meshs[i].vVBO);
		glBindBuffer(GL_ARRAY_BUFFER, meshs[i].vVBO);
		glBufferData(GL_ARRAY_BUFFER, meshs[i].vertices.size()*sizeof(GLfloat), &meshs[i].vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		//Texture
		if (meshs[i].textures.size() > 0) {
			glGenBuffers(1, &meshs[i].tVBO);
			glBindBuffer(GL_ARRAY_BUFFER, meshs[i].tVBO);
			glBufferData(GL_ARRAY_BUFFER, meshs[i].textures.size()*sizeof(GLfloat), &meshs[i].textures[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
		}

		//Normal
		if (meshs[i].normals.size() > 0 && isLight==false) {
			glGenBuffers(1, &meshs[i].nVBO);
			glBindBuffer(GL_ARRAY_BUFFER, meshs[i].nVBO);
			glBufferData(GL_ARRAY_BUFFER, meshs[i].normals.size()*sizeof(GLfloat), &meshs[i].normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(2);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
}

//Transform Location
void Object::setModelLocation(GLuint trfLoc) {
	ModelLocation = trfLoc;
}

//Transform Object
void Object::transform(glm::mat4 transformation) {
	modelTransform = transformation;
}

//Render Meshs
void Object::render() {
	for (unsigned int i = 0; i < meshs.size(); i++) {

		glBindTexture(GL_TEXTURE_2D, 0);
		if (isLight) {
			if (materials.size() > 0) {
				materials[meshs[i].ID_material].texture.bind();
			}
		}
		else {
			if (materials.size() > 0) {
				materials[meshs[i].ID_material].texture.bind();
				glUniform3f(glGetUniformLocation(program_ID, "objectColor"), 0.0f, 0.0f, 0.0f);
				glUniform3f(glGetUniformLocation(program_ID, "material.ambient"), materials[meshs[i].ID_material].Ka[0], materials[meshs[i].ID_material].Ka[1], materials[meshs[i].ID_material].Ka[2]);
				glUniform3f(glGetUniformLocation(program_ID, "material.diffuse"), materials[meshs[i].ID_material].Kd[0], materials[meshs[i].ID_material].Kd[1], materials[meshs[i].ID_material].Kd[2]);
				glUniform3f(glGetUniformLocation(program_ID, "material.specular"), materials[meshs[i].ID_material].Ks[0], materials[meshs[i].ID_material].Ks[1], materials[meshs[i].ID_material].Ks[2]);
				glUniform1f(glGetUniformLocation(program_ID, "material.shininess"), materials[meshs[i].ID_material].Ns);
			}
			else {
				glUniform3f(glGetUniformLocation(program_ID, "objectColor"), default_material.color.x, default_material.color.y, default_material.color.z);
				glUniform3f(glGetUniformLocation(program_ID, "material.ambient"), default_material.Ka[0], default_material.Ka[1], default_material.Ka[2]);
				glUniform3f(glGetUniformLocation(program_ID, "material.diffuse"), default_material.Kd[0], default_material.Kd[1], default_material.Kd[2]);
				glUniform3f(glGetUniformLocation(program_ID, "material.specular"), default_material.Ks[0], default_material.Ks[1], default_material.Ks[2]);
				glUniform1f(glGetUniformLocation(program_ID, "material.shininess"), default_material.Ns);
			}
		}
		glBindVertexArray(meshs[i].VAO);
		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(modelTransform));

		glDrawArrays(GL_TRIANGLES, 0, meshs[i].vertices.size());

		glBindVertexArray(0);

		if (materials.size() > 0) {
			materials[meshs[i].ID_material].texture.release();
		}
	}
}

//Regenera os vertices, as coodenadas de textura e as normais 
void Object::RegenerateMeshs() {
	for (int m = 0; m < meshs.size(); m++) {
		for (int f = 0; f < meshs[m].faces.size(); f++) {
			//Loop vertex from each face
			for (int i = 0; i < 3; i++) {
				meshs[m].vertices.push_back(vVertices[meshs[m].faces[f].indices[i].position].x);
				meshs[m].vertices.push_back(vVertices[meshs[m].faces[f].indices[i].position].y);
				meshs[m].vertices.push_back(vVertices[meshs[m].faces[f].indices[i].position].z);

				if (vTextures.size() > 0) {
					meshs[m].textures.push_back(vTextures[meshs[m].faces[f].indices[i].texture].x);
					meshs[m].textures.push_back(1 - vTextures[meshs[m].faces[f].indices[i].texture].y);
				}
				if (vNormals.size() > 0) {
					meshs[m].normals.push_back(vNormals[meshs[m].faces[f].indices[i].normal].x);
					meshs[m].normals.push_back(vNormals[meshs[m].faces[f].indices[i].normal].y);
					meshs[m].normals.push_back(vNormals[meshs[m].faces[f].indices[i].normal].z);
				}
			}//end loop i
		}//end loop f
	}//end loop m
}


//Split a string in an vector by delim (Template)
template<typename Out>
void Object::split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (!item.empty()) {
			*(result++) = item;
		}
	}
}

//Split a string in an vector by delim
std::vector<std::string> Object::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

//Remove chars of string
void Object::removeCharsFromString(string &str, char* charsToRemove) {
	for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
		str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
	}
}

//Load Object Model ------
void Object::LoadObject(char * filepath) {
	std::ifstream infile(filepath);
	std::cout << "File: " << filepath << endl;
	std::string line;
	Mesh tempMesh;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		if (line.size()>0 && line.at(0) != '#') {

			//Load Material(.mtl) in .obj
			if (CheckEntry("mtllib", line)) {
				vector<string> tks = split(line, ' ');
				if (tks.size() > 1) {
					LoadMaterial(&tks[1][0u]);
				}
			}
			//Vertices
			else if (line.at(0) == 'v' && line.at(1) == ' ') {
				line.erase(0, 2);

				vector<string> tks = split(line, ' ');

				std::string::size_type sz;
				if (tks.size() == 3) {

					glm::vec3 verT;
					verT.x = std::stof(tks[0], &sz);
					verT.y = std::stof(tks[1], &sz);
					verT.z = std::stof(tks[2], &sz);

					vVertices.push_back(verT);
				}
			}
			//Vertices Texture
			else if (line.at(0) == 'v' && line.at(1) == 't') {
				line.erase(0, 2);

				vector<string> tks = split(line, ' ');

				std::string::size_type sz;
				if (tks.size() == 2) {
					glm::vec2 txdT;
					txdT.x = std::stof(tks[0], &sz);
					txdT.y = std::stof(tks[1], &sz);

					vTextures.push_back(txdT);
				}
			}
			//Vertices Normals
			else if (line.at(0) == 'v' && line.at(1) == 'n') {
				line.erase(0, 2);

				vector<string> tks = split(line, ' ');

				std::string::size_type sz;
				if (tks.size() == 3) {
					glm::vec3 norT;
					norT.x = std::stof(tks[0], &sz);
					norT.y = std::stof(tks[1], &sz);
					norT.z = std::stof(tks[2], &sz);

					vNormals.push_back(norT);
				}
			}
			//Define Group and save Mesh
			else if (line.at(0) == 'g') {

				if (tempMesh.faces.size()>0)
					meshs.push_back(tempMesh);

				tempMesh = Mesh();
				curMesh += 1;
			}
			//Define Material to Mesh
			else if (line.at(0) == 'u' && line.at(1) == 's' && line.at(2) == 'e') {
				vector<string> tks = split(line, ' ');
				if (tks.size()>1)
					tempMesh.ID_material = FindIDMaterial(tks[1]);
			}
			//Define indices(faces) to previous Mesh
			else if (line.at(0) == 'f') {
				line.erase(0, 2);
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				Face faceTemp;

				if (tks.size() > 2) {
					for (unsigned int i = 0; i < tks.size(); i++) {
						vector<string> vsep = split(tks[i], '/');

						faceTemp.indices[i].position = std::stoi(vsep[0], &sz) - 1;

						if (vsep.size() > 1) {

							faceTemp.indices[i].texture = std::stoi(vsep[1], &sz) - 1;

							if (vsep.size() == 2) {
								faceTemp.indices[i].normal = std::stoi(vsep[1], &sz) - 1;
							}
						}
						if (vsep.size() > 2) {
							faceTemp.indices[i].normal = std::stoi(vsep[2], &sz) - 1;
						}
					}//end for
					faces.push_back(faceTemp);
					tempMesh.faces.push_back(faceTemp);
				}//end tks.size()>2
			}//end 'f'
		}
	}
	if (tempMesh.faces.size()>0)
		meshs.push_back(tempMesh);

	RegenerateMeshs();
}

//Find material ID by name in material array
GLint Object::FindIDMaterial(string materialName) {
	for (unsigned int i = 0; i < materials.size(); i++) {
		if (materials[i].matName.compare(materialName) == 0) {
			return i;
		}
	}
	return -1;
}

//Load material
void Object::LoadMaterial(char * filepath) {

	char result[100];

	strcpy_s(result, "objs/");
	strcat_s(result, filepath);


	std::ifstream infile(result);

	std::string line;
	Material matTemp;
	unsigned int cMt = 0;
	std::cout << "Texture " << result << endl;
	while (std::getline(infile, line)) {
		std::istringstream iss(line);

		removeCharsFromString(line, "	");
		if (line.size() > 0 && line.at(0) != '#') {
			if (line.at(0) == 'n' && line.at(1) == 'e' && line.at(2) == 'w') {

				vector<string> tks = split(line, ' ');

				//if(matTemp.matName.empty()==false)

				matTemp = Material();
				materials.push_back(matTemp);
				cMt += 1;

				materials[cMt - 1].matName = tks[1];
				materials[cMt - 1].matNumber = curMaterial;


			}
			else if (CheckEntry("Ns", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>1) {
					materials[cMt - 1].Ns = std::stof(tks[1], &sz);
				}

			}
			else if (CheckEntry("Ka", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					materials[cMt - 1].Ka.push_back(std::stof(tks[1], &sz));
					materials[cMt - 1].Ka.push_back(std::stof(tks[2], &sz));
					materials[cMt - 1].Ka.push_back(std::stof(tks[3], &sz));
				}

			}
			else if (CheckEntry("Kd", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					materials[cMt - 1].Kd.push_back(std::stof(tks[1], &sz));
					materials[cMt - 1].Kd.push_back(std::stof(tks[2], &sz));
					materials[cMt - 1].Kd.push_back(std::stof(tks[3], &sz));
				}
			}
			else if (CheckEntry("Ks", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					materials[cMt - 1].Ks.push_back(std::stof(tks[1], &sz));
					materials[cMt - 1].Ks.push_back(std::stof(tks[2], &sz));
					materials[cMt - 1].Ks.push_back(std::stof(tks[3], &sz));
				}
			}
			else if (CheckEntry("map_Kd", line)) {
				vector<string> tks = split(line, ' ');
				if (tks.size() > 1) {
					materials[cMt - 1].map_Kd = tks[1];

					std::cout << "Texture: [" << &tks[1][0u] << "]" << endl;

					std::string txPath = tks[1];

					if (setPathTx)
						txPath = "objs/" + tks[1];

					Texture txd = Texture();
					txd.Setup2D(&txPath[0u], GL_REPEAT, GL_LINEAR, GL_RGB);
					materials[cMt - 1].texture = txd;
				}
			}
		}
	}
}

//Check if charset is in begin of string
bool Object::CheckEntry(char * entrycheck, const std::string &linecheck) {
	unsigned int maxSizeEntry = (unsigned)strlen(entrycheck);
	for (unsigned int i = 0; i < maxSizeEntry; i++) {
		if ((i >= (maxSizeEntry - 1)) && (linecheck.at(i) == entrycheck[i])) {
			return true;
		}
		if (linecheck.at(i) != entrycheck[i]) {
			return false;
		}
	}
	return false;
}

//Clear all Meshs
void Object::clear() {
	for (unsigned int i = 0; i < meshs.size(); i++) {
		glDeleteVertexArrays(1, &meshs[i].VAO);
		glDeleteBuffers(1, &meshs[i].vVBO);
		glDeleteBuffers(1, &meshs[i].tVBO);
		glDeleteBuffers(1, &meshs[i].nVBO);
	}
}