#include <iostream>
#include <string>
#include <ctime>

using namespace std;

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

// GLEW
#define GLEW_STATIC
#include <gl/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "camera.h"
//#include "ObjectModel.h"
#include "Object.h"
#include "GameObject.h"
#include "GameConfigurator.h"

float screenWidth = 1024.0f;
float screenHeight = 768.0f;

//float screenWidth = 800.0f;
//float screenHeight = 600.0f;

//float screenWidth = 600.0f;
//float screenHeight = 400.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//timing variables
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float lastX = screenWidth / 2.0f, lastY = screenHeight / 2.0f;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 2.0f, 8.0f));

GLfloat alphahappy = 1.0f;
GLboolean keyAlpha = GL_FALSE;

GLboolean canMove = false;

//Obj control
GLfloat objPos[] = { 0.0f, 0.0f, 0.0f };
GLfloat objTranslFact = 1.0f;

GLfloat objRotAngleX, objRotAngleY, objRotAngleZ;
GLfloat objRadAngleX, objRadAngleY, objRadAngleZ;
GLfloat objRotFact = 20.0f;

GLfloat objScale = 1.0f;
GLfloat objDefaultScale = 1.0f;
GLfloat objScaleFact = 1.0f;

GLfloat ambientLight = 0.2f;
GLfloat diffuseLight = 0.5f;
GLfloat specLight = 1.0f;

GLfloat illumination = 1.0f;
GLint shiny = 32;

float skyColor[] = { 0.0f,0.0f,0.0f };
glm::vec3 objcColor(0.0f, 0.0f, 0.0f);

glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

double constrainAngle(double x) {
	x = fmod(x, 360);
	if (x < 0)
		x += 360;
	return x;
}

void ResetObjValues() {
	objScale = objDefaultScale;

	objPos[0] = 0.0f;
	objPos[1] = 0.0f;
	objPos[2] = 0.0f;

	objRotAngleX = 0.0f;
	objRotAngleY = 0.0f;
	objRotAngleZ = 0.0f;

	illumination = 1.0f;
	ambientLight = 0.2f;
	diffuseLight = 0.5f;
	specLight = 1.0f;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static int oldState = GLFW_RELEASE;
	int newState = glfwGetKey(window, GLFW_KEY_F);
	if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
		canMove = !canMove;
	}
	oldState = newState;

	if (canMove) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	//POSITION
	//X
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		objPos[0] -= objTranslFact * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		objPos[0] += objTranslFact * deltaTime;
	}
	//Y
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		objPos[1] -= objTranslFact * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		objPos[1] += objTranslFact * deltaTime;
	}
	//Z
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		objPos[2] += objTranslFact * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		objPos[2] -= objTranslFact * deltaTime;
	}

	//ROTATION ----------------
	//X
	if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) {
		objRadAngleX -= objRotFact * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) {
		objRadAngleX += objRotFact * deltaTime;
	}
	//Y
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
		objRadAngleY -= objRotFact * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		objRadAngleY += objRotFact * deltaTime;
	}
	//Z
	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
		objRadAngleZ -= objRotFact * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
		objRadAngleZ += objRotFact * deltaTime;
	}
	//Constrain Angles
	if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) {
		objRotAngleX = constrainAngle(objRadAngleX);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		objRotAngleY = constrainAngle(objRadAngleY);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
		objRotAngleZ = constrainAngle(objRadAngleZ);
	}

	//SCALE -------------------
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		objScale += objScaleFact * deltaTime;
		if (objScale >= 3.0f)
			objScale = 3.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		objScale -= objScaleFact * deltaTime;
		if (objScale <= 0.01f)
			objScale = 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		ResetObjValues();
	}
}

glm::vec3 lightPos(0.0f, 10.0f, 0.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OBJ Loader Doido", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader lightingShader("res/lightning3.vshad", "res/lightning3.fshad");
	//Shader lampShader("res/lamp.vshad", "res/lamp.fshad");
	Shader lightSource("res/lightSource.vshad", "res/lightSource.fshad");

	//----------- Load model -----------
	//vector<GameObject> gameObjects;

	////Object Model
	//Object objModel = Object();
	//objModel.setPathTx = true;
	//objModel.LoadObject("objs/Gardevoir.obj");
	//objModel.SetupVAO(lightingShader.ID);

	////Sphere Light
	//Object objLamp = Object();
	//objLamp.LoadObject("objs/sphere.obj");
	//objLamp.SetupVAO(lampShader.ID);

	////Plane
	//Object objPlane = Object();
	//objPlane.LoadObject("objs/plane.obj");
	//objPlane.SetupVAO(lightingShader.ID);

	////Game Objects


	//gameObjects.push_back(GameObject(objModel));
	//gameObjects[0].scale = glm::vec3(0.1f);
	//gameObjects[0].hasEquation = true;

	//gameObjects.push_back(GameObject(objModel));
	//gameObjects[1].position.x -= 2;
	//gameObjects[1].scale = glm::vec3(0.1f);

	//gameObjects.push_back(GameObject(objModel));
	//gameObjects[2].position.x += 2;
	//gameObjects[2].scale = glm::vec3(0.1f);

	//gameObjects.push_back(GameObject(objModel));
	//gameObjects[3].position.x -= 4;
	//gameObjects[3].scale = glm::vec3(0.1f);
	//gameObjects[3].hasEquation = true;

	//gameObjects.push_back(GameObject(objModel));
	//gameObjects[4].position.x += 4;
	//gameObjects[4].scale = glm::vec3(0.1f);
	//gameObjects[4].hasEquation = true;

	//gameObjects.push_back(GameObject(objPlane));

	//gameObjects.push_back(GameObject(objLamp));
	//gameObjects[1].position = lightPos;
	//gameObjects[1].scale = glm::vec3(0.5f);

	//----------------------------------

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);
	bool showOptionsGUI = true;

	glEnable(GL_DEPTH_TEST);

	ImVec2 size_optGui(300.0f, 500.0f);

	//Normalize scale
	objScale = 1.0f;

	GameConfigurator gameConfig = GameConfigurator();

	gameConfig.obj_Program_ID = lightingShader.ID;
	gameConfig.ls_Program_ID = lightSource.ID;

	gameConfig.LoadDefinitions("scene.olist");
	gameConfig.LoadGameObjects("gameobjects.glist");
	
	camera.Position = gameConfig.cameraPosition;

	std::cout << "game objs: " << gameConfig.gameObjects.size() << endl;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		ImGui_ImplGlfwGL3_NewFrame();

		if (showOptionsGUI)
		{
			ImGui::SetNextWindowPos(ImVec2(screenWidth - size_optGui.x, 0.0f));
			ImGui::SetNextWindowSize(size_optGui);

			ImGui::Begin("Object Controler");
			ImGui::GetIO().MouseDrawCursor = canMove;
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::SliderFloat("Escala", &objScale, 0.01f, 3.0f);
			ImGui::NewLine();
			ImGui::DragFloat3("Position", objPos, 0.1f);
			ImGui::NewLine();
			ImGui::SliderFloat("Rotation X", &objRotAngleX, 0.0f, 360.0f);
			ImGui::SliderFloat("Rotation Y", &objRotAngleY, 0.0f, 360.0f);
			ImGui::SliderFloat("Rotation Z", &objRotAngleZ, 0.0f, 360.0f);
			ImGui::NewLine();
			if (ImGui::Button("Reset"))
				ResetObjValues();
			ImGui::NewLine();

			ImGui::SliderFloat("Iluminacao", &illumination, 0.0f, 3.0f);
			ImGui::SliderFloat("Ambient Light", &ambientLight, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Light", &diffuseLight, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular Light", &specLight, 0.0f, 1.0f);
			ImGui::ColorEdit3("Sky Color", skyColor);
			ImGui::ColorEdit3("Obj Color", glm::value_ptr(objcColor));
			ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));

			if (canMove)
				ImGui::TextColored(ImColor(0, 255, 80), "Movimento habilitado! (pressione F)");
			else
				ImGui::TextColored(ImColor(255, 0, 0), "Movimento desabilitado! (pressione F)");
			ImGui::End();
		}

		//timing
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		//lightPos = glm::vec3(objPosX, objPosY, objPosZ);
		//lightPos.x = 0.0f + 2.0f *sin(glfwGetTime());
		//lightPos.y = 0.0f + 2.0f *cos(glfwGetTime());

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);

		//Render
		glClearColor(skyColor[0], skyColor[1], skyColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//lightingShader.use();

		//Set lights and light positions----
		lightingShader.setFloat("illumi", illumination);
		//glm::vec3 diffuseColor = lightColor   * glm::vec3(diffuseLight); // decrease the influence
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(ambientLight); // low influence

		lightingShader.setInt("size_lights", gameConfig.lightObjects.size());
		for (int go = 0; go < gameConfig.lightObjects.size(); go++) {
			if (gameConfig.lightObjects[go].isEnable) {
				lightingShader.setVec3("lights[" + std::to_string(go) + "].ambient", gameConfig.lightObjects[go].light.ambientColor);
				lightingShader.setVec3("lights[" + std::to_string(go) + "].diffuse", gameConfig.lightObjects[go].light.diffuseColor);
				lightingShader.setVec3("lights[" + std::to_string(go) + "].specular", glm::vec3(gameConfig.lightObjects[go].light.specLight));

				lightingShader.setFloat("lights[" + std::to_string(go) + "].constant", gameConfig.lightObjects[go].light.constant);
				lightingShader.setFloat("lights[" + std::to_string(go) + "].linear", gameConfig.lightObjects[go].light.linear);
				lightingShader.setFloat("lights[" + std::to_string(go) + "].quadratic", gameConfig.lightObjects[go].light.quadratic);

				lightingShader.setVec3("lights[" + std::to_string(go) + "].position", gameConfig.lightObjects[go].light.position);
			}
		}

		lightingShader.setVec3("viewPos", camera.Position);

		int modelLoc = glGetUniformLocation(lightingShader.ID, "model");
		int viewLoc = glGetUniformLocation(lightingShader.ID, "view");
		int projectionLoc = glGetUniformLocation(lightingShader.ID, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//-----------------------------------
		//lightSource.use();

		

		//lightingShader.use();

		/*for (int go = 0; go < gameObjects.size(); go++) {
			gameObjects[go].transform();
			gameObjects[go].object.render();
		}*/
		//lightingShader.setInt("size_lights", gameConfig.lightObjects.size());
		for (int go = 0; go < gameConfig.lightObjects.size(); go++) {
			if (gameConfig.lightObjects[go].isEnable) {
				//lightingShader.setVec3("lights[" + std::to_string(go) + "].ambient", gameConfig.lightObjects[go].light.ambientColor);
				//lightingShader.setVec3("lights[" + std::to_string(go) + "].diffuse", gameConfig.lightObjects[go].light.diffuseColor);
				//lightingShader.setVec3("lights[" + std::to_string(go) + "].specular", glm::vec3(gameConfig.lightObjects[go].light.specLight));

				//lightingShader.setFloat("lights[" + std::to_string(go) + "].constant", gameConfig.lightObjects[go].light.constant);
				//lightingShader.setFloat("lights[" + std::to_string(go) + "].linear", gameConfig.lightObjects[go].light.linear);
				//lightingShader.setFloat("lights[" + std::to_string(go) + "].quadratic", gameConfig.lightObjects[go].light.quadratic);

				//lightingShader.setVec3("lights[" + std::to_string(go) + "].position", gameConfig.lightObjects[go].light.position);

				lightSource.use();
				glUniformMatrix4fv(glGetUniformLocation(lightSource.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(lightSource.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
				gameConfig.lightObjects[go].transform();
				gameConfig.lightObjects[go].object.render();
				
				//cout << "num light = " << gameConfig.lightObjects[go].light.position.y << endl;

				/*lightingShader.setVec3("lights[0].ambient", gameConfig.lightObjects[go].light.ambientColor);
				lightingShader.setVec3("lights[0].diffuse", gameConfig.lightObjects[go].light.diffuseColor);
				lightingShader.setVec3("lights[0].specular", glm::vec3(gameConfig.lightObjects[go].light.specLight));

				lightingShader.setFloat("lights[0].constant", gameConfig.lightObjects[go].light.constant);
				lightingShader.setFloat("lights[0].linear", gameConfig.lightObjects[go].light.linear);
				lightingShader.setFloat("lights[0].quadratic", gameConfig.lightObjects[go].light.quadratic);

				lightingShader.setVec3("lights[0].position", gameConfig.lightObjects[go].light.position);*/

			}
		}

		for (int go = 0; go < gameConfig.gameObjects.size(); go++) {
			if (gameConfig.gameObjects[go].isEnable) {
				lightingShader.use();
				gameConfig.gameObjects[go].transform();
				gameConfig.gameObjects[go].object.render();
			}
		}

		//---------------------------------

		ImGui::Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//-------- CLEAR VALUES -------------

	//objModel.clear();
	//objLamp.clear();
	//objPlane.clear();


	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	if (canMove)
		camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}