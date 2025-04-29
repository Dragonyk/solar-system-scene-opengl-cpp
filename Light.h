#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Light{
public:
	glm::vec3 position;

	glm::vec3 lightColor;

	//temp not need
	float ambientLight;// = 0.2f;
	float diffuseLight;// = 0.5f;
	float specLight;// = 1.0f;
	//---

	float contant;
	float linear;
	float quadratic;

	//auto
	float ambientColor;
	float diffuseColor;

	Light();
};
#endif