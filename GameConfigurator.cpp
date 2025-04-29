#include "GameConfigurator.h"



GameConfigurator::GameConfigurator()
{
}

void GameConfigurator::LoadDefinitions(char * filepath){
	std::ifstream infile(filepath);
	std::string line;
	Object objTemp;

	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		//std::cout << "cheguei aqui: " << endl;

		if (line.size() > 0 && line.at(0) != '#') {
			if (CheckEntry("cam_pos", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					cameraPosition = glm::vec3(std::stof(tks[1], &sz), std::stof(tks[2], &sz), std::stof(tks[3], &sz));
				}
			}
			else if (CheckEntry("obj", line)) {
				objTemp = Object();

				vector<string> tks = split(line, ' ');
				if (tks.size() > 1) {
					objTemp.setPathTx = true;
					objTemp.LoadObject(&tks[1][0u]);
				}
			}
			else if (CheckEntry("id ", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>1) {
					objTemp.ID = std::stoi(tks[1], &sz);
				}
			}
			else if (CheckEntry("end_", line)) {
				objects.push_back(objTemp);
			}
		}
	}
}

void GameConfigurator::LoadGameObjects(char * filepath) {
	std::ifstream infile(filepath);
	std::string line;
	GameObject gameObjTemp;

	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		//std::cout << "cheguei aqui: " << endl;

		if (line.size() > 0 && line.at(0) != '#') {
			if (CheckEntry("go", line)) {
				gameObjTemp = GameObject();
				std::string::size_type sz;
				vector<string> tks = split(line, ' ');
				if (tks.size() > 1) {
					int obj_index =  FindObjectIndex(std::stoi(tks[1], &sz));
					if (obj_index >= 0) {
						gameObjTemp.object = objects[obj_index];
					}
					else {
						gameObjTemp.isEnable = false;
					}
				}
			}
			else if (CheckEntry("lcolor ", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					gameObjTemp.hasLight = true;
					gameObjTemp.light.lightColor = glm::vec3(std::stof(tks[1], &sz), std::stof(tks[2], &sz), std::stof(tks[3], &sz));
				}
			}
			else if (CheckEntry("latten ", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					gameObjTemp.light.quadratic = std::stof(tks[1], &sz);
					gameObjTemp.light.linear = std::stof(tks[2], &sz);
					gameObjTemp.light.constant = std::stof(tks[3], &sz);
				}
			}
			else if (CheckEntry("anim", line)) {
				//DETECT ANIMATION TYPE
				/*vector<string> tks = split(line, ' ');
				if (tks.size() > 1) {

				gameObjTemp.catmull.curve.push_back();
				}*/
			}
			else if (CheckEntry("ap ", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					glm::vec3 pointEq(std::stof(tks[1], &sz), std::stof(tks[2], &sz), std::stof(tks[3], &sz));
					gameObjTemp.catmull.points.push_back(pointEq);
				}
			}
			else if (CheckEntry("p ", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					gameObjTemp.position = glm::vec3(std::stof(tks[1], &sz), std::stof(tks[2], &sz), std::stof(tks[3], &sz));
				}
			}
			else if (CheckEntry("r ", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					gameObjTemp.rotation = glm::vec3(std::stof(tks[1], &sz), std::stof(tks[2], &sz), std::stof(tks[3], &sz));
				}
			}
			else if (CheckEntry("s ", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					gameObjTemp.scale = glm::vec3(std::stof(tks[1], &sz), std::stof(tks[2], &sz), std::stof(tks[3], &sz));
				}
			}
			else if (CheckEntry("r_axis ", line)) {
				vector<string> tks = split(line, ' ');
				std::string::size_type sz;
				if (tks.size()>3) {
					gameObjTemp.factRotation = glm::vec3(std::stof(tks[1], &sz), std::stof(tks[2], &sz), std::stof(tks[3], &sz));
				}
			}
			else if (CheckEntry("end_anim", line)) {
				if (gameObjTemp.catmull.points.size() >= 4)
					gameObjTemp.catmull.genCurves();
			}
			else if (CheckEntry("end_go", line)) {
				if (gameObjTemp.isEnable) {
					if (gameObjTemp.hasLight) {
						gameObjTemp.calculeLightColor();
						gameObjTemp.object.SetupVAO(ls_Program_ID);
						lightObjects.push_back(gameObjTemp);
					}
					else {
						gameObjTemp.object.SetupVAO(obj_Program_ID);
						gameObjects.push_back(gameObjTemp);
					}
				}
			}
		}
	}
}

int GameConfigurator::FindObjectIndex(int go_id) {
	for (int o = 0; o < objects.size(); o++) {
		if (objects[o].ID == go_id) {
			return o;
		}
	}
	return -1;
}

//Check if charset is in begin of string
bool GameConfigurator::CheckEntry(char * entrycheck, const std::string &linecheck) {
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

//Split a string in an vector by delim (Template)
template<typename Out>
void GameConfigurator::split(const std::string &s, char delim, Out result) {
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
std::vector<std::string> GameConfigurator::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}
