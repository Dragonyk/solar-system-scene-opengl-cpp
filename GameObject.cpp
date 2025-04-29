#include "GameObject.h"

GameObject::GameObject(){
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

GameObject::GameObject(Object obj){
	object = obj;

	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

bool lesser = false;
void GameObject::transform(){
	glm::mat4 model;
	//Aplicamos movimento por formula aqui

	//if(hasEquation)
	//	position.y = sin(glfwGetTime());

	//talvez add in position axis
	if (catmull.curve.size() > 0) {
		position.x = catmull.curve[ curvePoint * 3];
		position.y = catmull.curve[(curvePoint * 3) + 1];
		position.z = catmull.curve[(curvePoint * 3) + 2];

		if ((curvePoint < (catmull.curve.size() / 3) - 1)) {
			curvePoint += 1;
		}
		//else curvePoint = 0;
	}

	if (factRotation != glm::vec3(0.0f))
		rotation += factRotation;

	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);

	if(hasLight)
		light.position = position;

	object.transform(model);
}

void GameObject::calculeLightColor(){
	light.ambientLight = 0.2f;
	light.diffuseLight = 0.5f;
	light.specLight = 1.0f;

	light.diffuseColor = light.lightColor   * glm::vec3(light.diffuseLight); // decrease the influence
	light.ambientColor = light.diffuseColor * glm::vec3(light.ambientLight); // low influence
}


//
//void GameObject::Translate(glm::vec3 trfValue){
//
//}
//
//void GameObject::Rotate(glm::vec3 rotValue){
//
//}
//
//void GameObject::Scale(glm::vec3 sclValue){
//
//}


