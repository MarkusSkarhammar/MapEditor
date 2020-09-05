#ifndef RENDERER_H_
#define	RENDERER_H_

class Instancing;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>


#include "Global.h"

// Include GLM
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


class Instancing {
public:

	Instancing() {
	};
	void insertElement(int ID, int floor, int VAO, int textPos, DrawObject* objects);
	void generateTranslations();
	void clearElements();
	void drawInstancing(int floor);
	bool doInstanceDrawing(int floor);
	
	class Element {
	public:
		Element(int ID, int floor, int VAO, int textPos);
		int getID() { return ID; };
		int getFloor() { return floor; };
		int getVAO() { return VAO; };
		int getTextPos() { return textPos; };
		void insertObject(DrawObject* ob) { objects.push_back(ob); };
		void generateTranslations();
		void clear();
		void clearTranslations();
		std::vector<DrawObject*> getObjects() { return objects; };
		std::vector<glm::vec2> getTranslations() { return translations; };
	private:
		int ID{ 0 }, floor{ 0 }, VAO{ 0 }, textPos{ 0 };
		std::vector<glm::vec2> translations;
		std::vector<DrawObject*> objects;
	};
private:
	std::vector<Element*> elements;
};

void render(game_state const& interpolated_state, GLFWwindow* window);

void prepareDraw();


#endif