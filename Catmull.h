#ifndef CATMULL_H
#define CATMULL_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Catmull
{
public:
	vector<glm::vec3> points;
	vector<float> curve;
	vector<float> color;

	Catmull();
	void genCurves();
	void genColors();
};
#endif