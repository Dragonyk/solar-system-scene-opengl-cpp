#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Object.h"
#include "Catmull.h"

class GameObject{
public:
	Catmull catmull;

	bool isEnable = true;

	int ID;
	Object object;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	//Factores
	glm::vec3 factRotation;

	int curvePoint = 0;

	bool hasEquation = false;
	bool hasLight = false;

	//Light
	struct Light {
		glm::vec3 position;

		glm::vec3 lightColor;

		//temp not need
		float ambientLight;// = 0.2f;
		float diffuseLight;// = 0.5f;
		float specLight;// = 1.0f;
		//---

		float constant;
		float linear;
		float quadratic;

		//auto
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
	};

	Light light;

	//methods
	GameObject();
	GameObject(Object obj);

	void transform();
	void calculeLightColor();

	//void Translate(glm::vec3 trfValue);
	//void Rotate(glm::vec3 rotValue);
	//void Scale(glm::vec3 sclValue);
};
#endif