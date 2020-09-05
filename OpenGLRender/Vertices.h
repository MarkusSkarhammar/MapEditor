#ifndef VERTECES_HANDLER_H
#define VERTECES_HANDLER_H

#include <string>
#include <vector>
#include <algorithm>
// Include GLEW
#include <GL/glew.h>

class Vertices {
public:
	//
	Vertices(std::string name, float xStartText, float yStartText, float widthText, float heightText, float width, float height, std::vector<float>& v, std::vector<float>& vt, int textPos, GLuint VAO, GLuint VBO, int ID, bool doubleSize = false);
	Vertices(Vertices* other, std::vector<float>& v, std::vector<float>& vt, int VAO, int VBO);
	Vertices(Vertices* other, float textureSizeWidth, float textureSizeHeight, std::vector<float>& v, std::vector<float>& vt, int VAO, int VBO);
	Vertices(std::string name, float width, float height, Vertices* other);
	Vertices(Vertices* other);
	std::string getName() { return name; };
	int getID() { return ID; };
	void decrementID() { ID--; };
	float getXStartText() { return xStartText; };
	void setXStartText(float v) { xStartText = v; };
	float getYStartText() { return yStartText; };
	void setYStartText(float v) { yStartText = v; };
	float getWidthText() { return widthText; };
	void setWidthText(float v) { widthText = v; };
	float getHeightText() { return heightText; };
	void setHeightText(float v) { heightText = v; };
	float getWidth() { return width; };
	void setWidth(float v) { width = v; };
	float getHeight() { return height; };
	void setHeight(float v) { height = v; };
	float getTextureSizeWidth() { return textureSizeWidth; };
	void setTextureSizeWidth(float size) { textureSizeWidth = size; };
	float getTextureSizeHeight() { return textureSizeHeight; };
	void setTextureSizeHeight(float size) { textureSizeHeight = size; };
	bool isDoubleSize() { return doubleSize; };
	int getTextPos() { return textPos; };
	void setTextPos(int newPos) { textPos = newPos; };
	GLuint getVAO() { return VAO; };
	GLuint getVBO() { return VBO; };
	std::vector<float>& getV() { return v; };
	std::vector<float>& getVT() { return vt; };
	void copyAnotherVerticesText(Vertices* other);
	void changeSize(int width, int height);
private:
	std::string name;
	int ID{ 0 }, textPos{ 0 };
	GLuint VAO{ 0 }, VBO{ 0 };
	bool doubleSize { false };
	float xStartText{ 0.f }, yStartText{ 0.f }, widthText{ 0.f }, heightText{ 0.f }, width{ 0.f }, height{ 0.f }, textureSizeWidth{ 2048.f }, textureSizeHeight{ 2048.f };
	std::vector<float>& v, &vt;
};

void removeVertices(Vertices* v);

#endif