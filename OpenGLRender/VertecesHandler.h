#ifndef VERTECES_HANDLER_H
#define VERTECES_HANDLER_H

#include <string>
#include <vector>
#include <algorithm>
// Include GLEW
#include <GL/glew.h>

class VertecesHandler {
public:
	VertecesHandler(std::string name = "", int amount = 0) : name(name), amount(amount) { VBO = GLuint(0); VAO = GLuint(0); };
	GLuint& getVBO() { return VBO; };
	GLuint& getVBOText() { return VBOText; };
	GLuint& getVAO() { return VAO; };
	size_t getTextureID() { return textureID; };
	int& getAmount() { return amount; };
	void setAmount(int value) { amount = value; };
	void setTextureID(size_t value) { textureID = value; };
	void setName(std::string value) { name = value; };
	std::string getName() { return name; };
	void setType(std::string value) { type = value; };
	std::string getType() { return type; };
	void addVerteces(std::vector<float>& values) { vertecesPosition = values; };
	std::vector<float>& getVerteces() { return vertecesPosition; };
	void addVertecesText(std::vector<float>& values) { vertecesTextPos = values; };
	std::vector<float>& getVertecesText() { return vertecesTextPos; };
	static VertecesHandler*& findByName(std::vector<VertecesHandler*>& list, std::string name) {
		std::vector<VertecesHandler*>::iterator it = std::find_if(list.begin(), list.end(), [name](VertecesHandler* verteces) {
			return (verteces->getName() == name);
		});
		if (it != list.end())
			return (*it);
		else
			return *list.begin();
	};
private:
	std::vector<float> vertecesPosition, vertecesTextPos;
	int amount = 0, textureID = 0;
	GLuint VBO{ 0 }, VBOText{ 0 }, VAO{ 0 };
	std::string name = "", type = "";
};

class Vertices {
public:
	Vertices(std::string name, VertecesHandler* vh, float xStartText, float yStartText, float widthText, float heightText, float width, float height);
	Vertices(std::string name, VertecesHandler* vh, float xStartText, float yStartText, float widthText, float heightText, float width, float height, int textToRend);
	Vertices(std::string name, VertecesHandler* vh, float xStartText, float yStartText, float widthText, float heightText, float width, float height, bool doubleSize);
	VertecesHandler*& getVertecesHandler() { return vh; };
	std::string getName() { return name; };
	int getID() { return ID; };
	float getXStartText() { return xStartText; };
	float getYStartText() { return yStartText; };
	float getWidthText() { return widthText; };
	float getHeightText() { return heightText; };
	float getWidth() { return width; };
	float getHeight() { return height; };
	static Vertices*& findByName(std::vector<Vertices*>& list, std::string name) {
		std::vector<Vertices*>::iterator it = std::find_if(list.begin(), list.end(), [name](Vertices*& vertices) {
			return (vertices->getName() == name);
		});
		if (it != list.end())
			return *it;
		else
			return *list.begin();
	};
private:
	std::string name;
	VertecesHandler* vh;
	int ID;
	float xStartText, yStartText, widthText, heightText, width, height;
};

void removeVertices(Vertices* v);

void getVertecesHandlerFromID(VertecesHandler*& vh, int& ID);
void getVertecesHandlerFromID(VertecesHandler*& vh, int& ID, bool& size);

void generateVetecesSquares(VertecesHandler* vh, int amount);
void generate_Palette_Modifier_Rend_To_text(VertecesHandler* vh);
void generateGUI(VertecesHandler* vh, std::string textName);
void generate_Left_Panel_GUI(VertecesHandler*& vh);
void generate_Left_Panel_Drop_Down_GUI(VertecesHandler*& vh);
void generate_Left_Panel_select(VertecesHandler*& vh);
void generate_BottomBar_Toggles(VertecesHandler*& vh);
void generate_Item_Info_Panel(VertecesHandler*& vh);
void generate_Palette_Modifier_UI(VertecesHandler*& vh);
#endif