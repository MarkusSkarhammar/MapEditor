#ifndef VERTECES_HANDLER_H
#define VERTECES_HANDLER_H

#include <string>
#include <vector>
#include <algorithm>
// Include GLEW
#include <GL/glew.h>

class VertecesHandler {
public:
	VertecesHandler(std::string name = "", size_t amount = 0) : name(name), amount(amount) { VBO = GLuint(0); VAO = GLuint(0); };
	GLuint& getVBO() { return VBO; };
	GLuint& getVAO() { return VAO; };
	size_t getTextureID() { return textureID; };
	size_t getAmount() { return amount; };
	void setAmount(size_t& value) { amount = value; };
	void setTextureID(size_t value) { textureID = value; };
	void setName(std::string value) { name = value; };
	std::string getName() { return name; };
	void setType(std::string value) { type = value; };
	std::string getType() { return type; };
	void addVerteces(std::vector<float>& values) { verteces = values; };
	std::vector<float>& getVerteces() { return verteces; };
	static VertecesHandler findByName(std::vector<VertecesHandler>& list, std::string name) {
		std::vector<VertecesHandler>::iterator it = std::find_if(list.begin(), list.end(), [name](VertecesHandler& verteces) {
			return (verteces.getName() == name);
		});
		if (it != list.end())
			return *it;
		else
			return VertecesHandler("wrong");
	};
private:
	std::vector<float> verteces;
	size_t amount = 0, textureID = 0;
	GLuint VBO = 0, VAO = 0;
	std::string name = "", type = "";
};

void getVertecesHandlerFromID(VertecesHandler& vh, int& ID);
void getVertecesHandlerFromID(VertecesHandler& vh, int& ID, bool& size);

void generateVetecesSquares(VertecesHandler& vh);
void generateGUI(VertecesHandler& vh, std::string textName);
void generate_Left_Panel_GUI(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v);
void generate_Left_Panel_Drop_Down_GUI(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v);
void generate_Left_Panel_select(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v);
void generate_BottomBar_Toggles(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v);
void generate_Left_Panel_Palette_Page(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v);
void generate_Item_Info_Panel(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v);
#endif