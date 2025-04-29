#include "Catmull.h"



Catmull::Catmull(){
}

glm::vec3 genPoint(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t) {

	float t_d = 0.5;
	float t_c = (t / 2);
	float t_b = (t*t) / 2;
	float t_a = (t*t*t) / 2;

	float h1 = (-t_a + 2 * t_b - t_c);
	float h2 = (3 * t_a - 5 * t_b + 2 * t_d);
	float h3 = (-3 * t_a + 4 * t_b + t_c);
	float h4 = (t_a - t_b);

	float x = h1*p0.x + h2*p1.x + h3*p2.x + h4*p3.x;
	float y = h1*p0.y + h2*p1.y + h3*p2.y + h4*p3.y;
	float z = h1*p0.z + h2*p1.z + h3*p2.z + h4*p3.z;
	glm::vec3 p = glm::vec3(x, y, z);

	return p;
}

void Catmull::genCurves() {
	cout << points.size() << endl;
	for (int i = 1; i < points.size() - 2; i++) {

		for (float t = 0.0f; t < 1.0f; t += 0.01f) {
			glm::vec3 p = genPoint(points[i - 1], points[i], points[i + 1], points[i + 2], t);

			curve.push_back(p.x);
			curve.push_back(p.y);
			curve.push_back(p.z);
		}
	}
	cout << curve[curve.size() - 3] << "," << curve[curve.size() - 2] << ", " << curve[curve.size() - 1] << endl;

}

void Catmull::genColors() {
	for (int i = 0; i < curve.size(); i++) {
		if ((i + 1) % 3 == 0) {
			color.push_back(1);
		}
		else
			color.push_back(0);
	}
}
