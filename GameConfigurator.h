#ifndef GAMECONFIGURATOR_H
#define GAMECONFIGURATOR_H

#include "GameObject.h"


class GameConfigurator{
public:
	glm::vec3 cameraPosition;
	vector<Object> objects;

	vector<GameObject> gameObjects;
	vector<GameObject> lightObjects;

	GLuint program_ID;
	GLuint obj_Program_ID;
	GLuint ls_Program_ID;

	GameConfigurator();
	void LoadDefinitions(char * filepath);
	void LoadGameObjects(char * filepath);
	int FindObjectIndex(int go_id);
	bool CheckEntry(char * entrycheck, const std::string & linecheck);
private:
	vector<std::string> split(const std::string &s, char delim);
	template<typename Out>
	void split(const std::string &s, char delim, Out result);

};
#endif