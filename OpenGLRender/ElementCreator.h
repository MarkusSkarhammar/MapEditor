#ifndef ELEMENT_CREATOR_H
#define ELEMENT_CREATOR_H

// Include GLFW
#include <glfw3.h>

#include <vector>
#include <string>
#include "Global.h"

using std::vector;
using std::string;

class ElementCreator {
public:
	ElementCreator() {};
	void createGUIs(std::vector<float>& v);
private:
	size_t createDropDownMenuSlots(float widthStart, float heightStart, float width, float height, float textWidthStart, float textHeightStart, float textWidth, float textHeight, float hoverSlotTextHeightStart,
		float borderSlotTextHeightStart, float borderHeight, float borderTextHeight, std::vector<float>& v, size_t size, size_t hoverSlot);
	size_t createSquare(float widthStart, float heightStart, float width, float height, float textWidthStart, float textHeightStart, float textWidth, float textHeight, vector<float>& v);
	size_t createCharacters(float widthStart, float heightStart, float width, float height, std::string input, vector<float>& v, bool offsetBool);
	vector<float> getCharacterTextures(std::string input);
	//bool isDoubleSize(size_t i);

	void selectionAreaFill(std::vector<float>& v);
	void leftHandGUISection(std::vector<float>& v);
	void leftHandGUILetters(std::vector<float>& v);

	void bottomBar(std::vector<float>& v);
	void bottomBarLetters(std::vector<float>& v);

	void tileDrawWindowScrollBars(std::vector<float>& v);

	void tileInfoWindowCreate(std::vector<float>& v);
	void tileInfoWindowLetters(std::vector<float>& v);

	void GUILetters(std::vector<float>& v);

	void previewTiles(std::vector<float>& v);
};

#endif

