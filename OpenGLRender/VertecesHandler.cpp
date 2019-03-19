#include "VertecesHandler.h"
#include "Global.h"
#pragma warning(disable: 4244)

void removeVertices(Vertices * v)
{
	auto& vh = v->getVertecesHandler();
	auto& verticesPos = vh->getVerteces();
	auto& verticesText = vh->getVertecesText();
	verticesPos.erase(verticesPos.begin() + v->getID(), verticesPos.begin() + v->getID() + 8);
	verticesText.erase(verticesText.begin() + v->getID(), verticesText.begin() + v->getID() + 8);
	vh->getAmount()--;
	delete v;
}

void getVertecesHandlerFromID(VertecesHandler*& vh, int& ID) {
	if (ID / 1024 == 0) {
		vh = VertecesHandler::findByName(verteces, "Tiles_1024");
	}
	else if (ID / 1024 == 1) {
		vh = VertecesHandler::findByName(verteces, "Items_1024");
	}
	else if (ID / 1024 == 2) {
		vh = VertecesHandler::findByName(verteces, "Doodads_1024");
	}
	else if (ID / 1024 == 3) {
		vh = VertecesHandler::findByName(verteces, "Borders_1024");
	}
	else if (ID / 1024 == 5) {
		vh = VertecesHandler::findByName(verteces, "Nature_1024");
	}
}

void getVertecesHandlerFromID(VertecesHandler*& vh, int& ID, bool& size) {
	if (ID / 1024 == 0) {
		vh = VertecesHandler::findByName(verteces, "Tiles_1024");
	}
	else if (ID / 1024 == 1) {
		vh = VertecesHandler::findByName(verteces, "Items_1024");
	}
	else if (ID / 1024 == 2) {
		vh = VertecesHandler::findByName(verteces, "Doodads_1024");
		size = true;
	}
	else if (ID / 1024 == 3) {
		vh = VertecesHandler::findByName(verteces, "Borders_1024");
		size = true;
	}
	else if (ID / 1024 == 5) {
		vh = VertecesHandler::findByName(verteces, "Nature_1024");
		size = true;
	}
}

void generateVetecesSquares(VertecesHandler* vh, int amount)
{
	int y = 0, x = 0;
	if (vh->getName().compare("Doodads_1024") == 0) {
		for (size_t i = 0; i < amount; i++) {
			y = floor(i / 16);
			x = i % 16;
			verticesContainer128xTiles.push_back(new Vertices(vh->getName() + "_" + std::to_string(i), vh, 0 + 128 * x, 0 + y * 128, 128, 128, 128, 128, true));
		}
	} else if (vh->getName().compare("Letters_") == 0) {
		for (size_t i = 0; i < amount; i++) {
			y = floor(i / 16);
			x = i % 16;
			verticesContainerLetters.push_back(new Vertices(vh->getName() + "_" + std::to_string(i), vh, 0 + 16 * x, 0 + y * 16, 16, 16, 16, 16));
		}
	}
	else {
		for (size_t i = 0; i < amount; i++) {
			y = floor(i / 32);
			x = i % 32;
			verticesContainer64xTiles.push_back(new Vertices(vh->getName() + "_" + std::to_string(i), vh, 0 + 64 * x, 0 + y * 64, 64, 64, 64, 64));
		}
	}
}

void generateGUI(VertecesHandler* vh, std::string textName)
{
	size_t amount = 0;
	float widthStart = 0.0f;
	float heightStart = 0.0f;
	double width = 0;
	double height = 0;
	float textHeightStart = 0;
	float textWidthStart = 0;
	float textWidthEnd = 0;
	float textHeightEnd = 0;
	float textOffset = (1 / 2048.0f);

	std::vector<float> v, vt;
	vh->setAmount(amount);
	vh->addVerteces(v);
	vh->addVertecesText(vt);

	if (textName.compare("GUI_1") == 0) {
		verticesContainer.push_back(new Vertices("bottomBarBasic", vh, 0.0, 0.0, 64.0, 30.0, 64.0, 30.0));
		verticesContainer.push_back(new Vertices("bottomBarLabel", vh, 65.0, 0.0, 64.0, 30.0, 64.0, 30.0));

		generate_BottomBar_Toggles(vh);

		generate_Left_Panel_GUI(vh);

		generate_Left_Panel_Drop_Down_GUI(vh);

		generate_Left_Panel_select(vh);

		generate_Item_Info_Panel(vh);

		generate_Palette_Modifier_UI(vh);
	}

}

void generate_Left_Panel_GUI(VertecesHandler*& vh) {

	verticesContainer.push_back(new Vertices("fillerPixel", vh, 0.0, 154.0, 275.0, 2.0, 276.0, 1.0));

	verticesContainer.push_back(new Vertices("displayBar", vh, 0.0, 31.0, 275.0, 40.0, 276.0, 40.0));

	verticesContainer.push_back(new Vertices("displayBarButton", vh, 277.0, 31.0, 21.0, 21.0, 21.0, 21.0));

	verticesContainer.push_back(new Vertices("displayBarButtonHover", vh, 299.0, 31.0, 21.0, 21.0, 21.0, 21.0));

	verticesContainer.push_back(new Vertices("displayBarButtonPressed", vh, 321.0, 31.0, 21.0, 21.0, 21.0, 21.0));

	verticesContainer.push_back(new Vertices("tileAreaTop", vh, 0.0, 157.0, 276.0, 90.0, 276.0, 90.0));

	verticesContainer.push_back(new Vertices("tileAreaMiddle", vh, 0.0, 250.0, 276.0, 90.0, 276.0, 90.0));

	verticesContainer.push_back(new Vertices("tileAreaBottom", vh, 0.0, 343.0, 276.0, 90.0, 276.0, 90.0));

	verticesContainer.push_back(new Vertices("tileAreaLeftButton", vh, 277.0, 53.0, 17.0, 23.0, 17.0, 23.0));

	verticesContainer.push_back(new Vertices("tileAreaLeftButtonHover", vh, 313.0, 53.0, 17.0, 23.0, 17.0, 23.0));

	verticesContainer.push_back(new Vertices("tileAreaLeftButtonPressed", vh, 349.0, 53.0, 17.0, 23.0, 17.0, 23.0));

	verticesContainer.push_back(new Vertices("tileAreaRightButton", vh, 295.0, 53.0, 17.0, 23.0, 17.0, 23.0));

	verticesContainer.push_back(new Vertices("tileAreaRightButtonHover", vh, 331.0, 53.0, 17.0, 23.0, 17.0, 23.0));

	verticesContainer.push_back(new Vertices("tileAreaRightButtonPressed", vh, 367.0, 53.0, 17.0, 23.0, 17.0, 23.0));
}

void generate_Left_Panel_Drop_Down_GUI(VertecesHandler*& vh)
{
	verticesContainer.push_back(new Vertices("leftPanelDropDownMiddleSection", vh, 0.0, 436.0, 259.0, 36.0, 259.0, 36.0));

	verticesContainer.push_back(new Vertices("leftPanelDropDownBottomSection", vh, 0.0, 474.0, 259.0, 38.0, 259.0, 38.0));

	verticesContainer.push_back(new Vertices("leftPanelDropDownHover", vh, 0.0, 513.0, 256.0, 36.0, 256.0, 36.0));
}

void generate_Left_Panel_select(VertecesHandler*& vh)
{
	verticesContainer.push_back(new Vertices("leftPanelYellowSquareSmall", vh, 0.0, 550.0, 64.0, 64.0, 64.0, 64.0));

	verticesContainer.push_back(new Vertices("leftPanelRedSquareSmall", vh, 0.0, 615.0, 64.0, 64.0, 64.0, 64.0));

	verticesContainer.push_back(new Vertices("leftPanelYellowSquareBig", vh, 65.0, 550.0, 128.0, 128.0, 128.0, 128.0));

	verticesContainer.push_back(new Vertices("leftPanelRedSquareBig", vh, 194.0, 550.0, 128.0, 128.0, 128.0, 128.0));

}

void generate_BottomBar_Toggles(VertecesHandler*& vh) {

	verticesContainer.push_back(new Vertices("bottomBarEraser", vh, 130.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarEraserHover", vh, 161.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarEraserPressed", vh, 192.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarDestroyer", vh, 223.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarDestroyerHover", vh, 254.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarDestroyerPressed", vh, 285.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("eraseToggleID", vh, 0.0, 680.0, 64.0, 64.0, 64.0, 64.0));

	verticesContainer.push_back(new Vertices("destroyToggleID", vh, 0.0, 745.0, 64.0, 64.0, 64.0, 64.0));

	verticesContainer.push_back(new Vertices("bottomBarTileDestroyer", vh, 316.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarTileDestroyerHover", vh, 347.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarTileDestroyerPressed", vh, 378.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarCut", vh, 409.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarCutHover", vh, 440.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarCutPressed", vh, 471.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarCopy", vh, 502.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarCopyHover", vh, 533.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("bottomBarCopyPressed", vh, 564.0, 0.0, 30.0, 30.0, 30.0, 30.0));

	verticesContainer.push_back(new Vertices("cutToggleID", vh, 65.0, 680.0, 64.0, 64.0, 64.0, 64.0));

	verticesContainer.push_back(new Vertices("copyToggleID", vh, 65.0, 745.0, 64.0, 64.0, 64.0, 64.0));

	verticesContainer.push_back(new Vertices("destroyTileToggleID", vh, 130.0, 680.0, 64.0, 64.0, 64.0, 64.0));

}

void generate_Item_Info_Panel(VertecesHandler*& vh) {
	
	verticesContainer.push_back(new Vertices("itemInfoPanelID", vh, 1048.0, 0.0, 1000.0, 700.0, 1000.0, 700.0));

	verticesContainer.push_back(new Vertices("itemInfoTextSectionID", vh, 897.0, 1.0, 150.0, 40.0, 150.0, 40.0));

	verticesContainer.push_back(new Vertices("itemInfoRightArrowID", vh, 924.0, 42.0, 30.0, 43.0, 30.0, 43.0));

	verticesContainer.push_back(new Vertices("itemInfoRightArrowHoverID", vh, 893.0, 42.0, 30.0, 43.0, 30.0, 43.0));

	verticesContainer.push_back(new Vertices("itemInfoRightArrowPressedID", vh, 862.0, 42.0, 30.0, 43.0, 30.0, 43.0));

	verticesContainer.push_back(new Vertices("itemInfoLeftArrowID", vh, 1017.0, 42.0, 30.0, 43.0, 30.0, 43.0));

	verticesContainer.push_back(new Vertices("itemInfoLeftArrowHoverID", vh, 986.0, 42.0, 30.0, 43.0, 30.0, 43.0));

	verticesContainer.push_back(new Vertices("itemInfoLeftArrowPressedID", vh, 955.0, 42.0, 30.0, 43.0, 30.0, 43.0));

	verticesContainer.push_back(new Vertices("itemInfoTextAreaID", vh, 897.0, 86.0, 150.0, 110.0, 150.0, 110.0));

	verticesContainer.push_back(new Vertices("itemInfoButtonID", vh, 897.0, 197.0, 150.0, 26.0, 150.0, 26.0));

	verticesContainer.push_back(new Vertices("itemInfoButtonHoverID", vh, 897.0, 224.0, 150.0, 26.0, 150.0, 26.0));

	verticesContainer.push_back(new Vertices("itemInfoButtonPressedID", vh, 897.0, 251.0, 150.0, 26.0, 150.0, 26.0));
}

void generate_Palette_Modifier_UI(VertecesHandler*& vh) {
	
	verticesContainer.push_back(new Vertices("paletteModifierPanel", vh, 1369.0, 701.0, 679.0, 724.0, 679.0, 724.0));

	verticesContainer.push_back(new Vertices("paletteModifierSearchIcon", vh, 1174.0, 701.0, 64.0, 64.0, 22.0, 22.0));
	
	verticesContainer.push_back(new Vertices("paletteModifierSearchIconHover", vh, 1239.0, 701.0, 64.0, 64.0, 22.0, 22.0));

	verticesContainer.push_back(new Vertices("paletteModifierSearchIconPressed", vh, 1304.0, 701.0, 64.0, 64.0, 22.0, 22.0));

	verticesContainer.push_back(new Vertices("paletteModifierDropDownUnPressed", vh, 1163.0, 766.0, 102.0, 24.0, 102.0, 24.0));

	verticesContainer.push_back(new Vertices("paletteModifierDropDownPressed", vh, 1266.0, 766.0, 102.0, 24.0, 102.0, 24.0));

	verticesContainer.push_back(new Vertices("paletteModifierDropDownElement", vh, 1163.0, 791.0, 102.0, 24.0, 102.0, 24.0));

	verticesContainer.push_back(new Vertices("paletteModifierDropDownElementHover", vh, 1266.0, 791.0, 102.0, 24.0, 102.0, 24.0));

	verticesContainer.push_back(new Vertices("paletteModifierEmptyTileMarker", vh, 1109.0, 701.0, 64.0, 64.0, 64.0, 64.0));
}

Vertices::Vertices(std::string name, VertecesHandler* vh, float xStartText, float yStartText, float widthText, float heightText, float width, float height) : name(name), vh(vh), xStartText(xStartText), yStartText(yStartText),
width(width), height(height), widthText(widthText), heightText(heightText)
{
	auto& v = vh->getVerteces();
	auto& vt = vh->getVertecesText();

	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float widthPos = (this->width / (double(screenWidthPixels) / 2));
	float heightPos = (this->height / (double(screenHeightPixels) / 2));

	float textWidthStart = (xStartText / 2048.0);
	float textHeightStart = (yStartText / 2048.0);
	float textWidthEnd = textWidthStart + (widthText / 2048.0);
	float textHeightEnd = textHeightStart + (heightText / 2048.0);

	v.push_back(widthStart); v.push_back(heightStart); vt.push_back(textWidthStart); vt.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + widthPos); v.push_back(heightStart); vt.push_back(textWidthEnd); vt.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - heightPos); vt.push_back(textWidthStart); vt.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + widthPos); v.push_back(heightStart - heightPos); vt.push_back(textWidthEnd); vt.push_back(textHeightEnd); // Bottom-right
	int& amount = vh->getAmount();
	ID = amount;
	amount++;
	
}

Vertices::Vertices(std::string name, VertecesHandler* vh, float xStartText, float yStartText, float widthText, float heightText, float width, float height, bool doubleSize) : name(name), vh(vh), xStartText(xStartText), yStartText(yStartText),
width(width), height(height), widthText(widthText), heightText(heightText)
{
	auto& v = vh->getVerteces();
	auto& vt = vh->getVertecesText();

	float widthStart = 0.0f - (64.0 / (double(screenWidthPixels) / 2));
	float heightStart = 0.0f + (64.0 / (double(screenHeightPixels) / 2));

	float widthPos = (this->width / (double(screenWidthPixels) / 2));
	float heightPos = (this->height / (double(screenHeightPixels) / 2));

	float textWidthStart = (xStartText/ 2048.0);
	float textHeightStart = (yStartText / 2048.0);
	float textWidthEnd = textWidthStart + (widthText / 2048.0);
	float textHeightEnd = textHeightStart + (heightText / 2048.0);

	v.push_back(widthStart); v.push_back(heightStart); vt.push_back(textWidthStart); vt.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + widthPos); v.push_back(heightStart); vt.push_back(textWidthEnd); vt.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - heightPos); vt.push_back(textWidthStart); vt.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + widthPos); v.push_back(heightStart - heightPos); vt.push_back(textWidthEnd); vt.push_back(textHeightEnd); // Bottom-right
	int& amount = vh->getAmount();
	ID = amount;
	amount++;

}
