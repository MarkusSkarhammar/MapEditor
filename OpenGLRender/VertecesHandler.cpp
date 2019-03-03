#include "VertecesHandler.h"
#include "Global.h"
#pragma warning(disable: 4244)

void getVertecesHandlerFromID(VertecesHandler& vh, int& ID) {
	if (ID / 1024 == 0 && vh.getName().compare("Tiles_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Tiles_1024");
	}
	else if (ID / 1024 == 1 && vh.getName().compare("Items_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Items_1024");
	}
	else if (ID / 1024 == 2 && vh.getName().compare("Doodads_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Doodads_1024");
	}
	else if (ID / 1024 == 3 && vh.getName().compare("Borders_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Borders_1024");
	}
	else if (ID / 1024 == 5 && vh.getName().compare("Nature_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Nature_1024");
	}
}

void getVertecesHandlerFromID(VertecesHandler& vh, int& ID, bool& size) {
	if (ID / 1024 == 0 && vh.getName().compare("Tiles_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Tiles_1024");
	}
	else if (ID / 1024 == 1 && vh.getName().compare("Items_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Items_1024");
	}
	else if (ID / 1024 == 2 && vh.getName().compare("Doodads_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Doodads_1024");
		size = true;
	}
	else if (ID / 1024 == 3 && vh.getName().compare("Borders_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Borders_1024");
		size = true;
	}
	else if (ID / 1024 == 5 && vh.getName().compare("Nature_1024") != 0) {
		vh = VertecesHandler::findByName(verteces, "Nature_1024");
		size = true;
	}
}

void generateVetecesSquares(VertecesHandler& vh)
{
	std::vector<float> v;
	float widthStart = 0.0f;
	float heightStart = 0.0f;
	double width = (imgScale / (double(screenWidthPixels) / 2));
	double height = (imgScale / (double(screenHeightPixels) / 2));
	float textWidth = (imgScale / 2048.0f);
	float textHeight = (imgScale / 2048.0f);
	float textOffset = (1 / 2048.0f);
	size_t j = 0, x = 0, y = 0;

	if (vh.getName().compare("Doodads_1024") == 0) {
		widthStart = 0.0f - width;
		heightStart = 0.0f + height;
		width = (imgScale*2 / (double(screenWidthPixels) / 2));
		height = (imgScale*2 / (double(screenHeightPixels) / 2));
		textWidth = (imgScale*2 / 2048.0f);
		textHeight = (imgScale*2 / 2048.0f);
		for (size_t i = 0; i < vh.getAmount(); i++) {
			j = floor(i / 16);
			float textHeightStart = 0.0f + (textHeight * j);
			float textWidthStart = 0.0f + (textWidth * i);
			float textWidthEnd = textWidthStart + textHeight - textOffset * 0;
			float textHeightEnd = textHeightStart + textHeight - textOffset * 0;
			float pixelsHS = textHeightStart * 2048;
			float pixelsHE = textHeightEnd * 2048;
			float pixelsW = textHeightStart * 2048 - 2048;
			//First triangle
			v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
			v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
			v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																																 //Second triangle
			v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
		}
	} else if (vh.getName().compare("Letters_") == 0) {
		widthStart = 0.0f - width;
		heightStart = 0.0f + height;
		width = (16 / (double(screenWidthPixels) / 2));
		height = (16 / (double(screenHeightPixels) / 2));
		textWidth = (16 / 2048.0f);
		textHeight = (16 / 2048.0f);
		for (size_t i = 0; i < vh.getAmount(); i++) {
			y = floor(i / 16);
			x = i % 16;
			float textHeightStart = 0.0f + (textHeight * y) + textOffset;
			float textWidthStart = 0.0f + (textWidth * x) + textOffset;
			float textWidthEnd = textWidthStart + textHeight - textOffset * 2;
			float textHeightEnd = textHeightStart + textHeight - textOffset * 2;

			//First triangle
			v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
			v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
			v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																																 //Second triangle
			v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
		}
	}
	else {
		for (size_t i = 0; i < vh.getAmount(); i++) {
			j = floor(i / 32);
			float textHeightStart = 0.0f + (textHeight * j) + textOffset;
			float textWidthStart = 0.0f + (textWidth * i) + textOffset;
			float textWidthEnd = textWidthStart + textHeight - textOffset * 2;
			float textHeightEnd = textHeightStart + textHeight - textOffset * 2;
			//First triangle
			v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
			v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
			v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																																 //Second triangle
			v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
		}
	}
	vh.addVerteces(v);
}

void generateGUI(VertecesHandler & vh, std::string textName)
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

	std::vector<float> v;

	if (textName.compare("GUI_1") == 0) {
		// Bottom Bar, 0  = left corner, 1 = middle part, 0 = right corner
		{
			width = (64 / (double(screenWidthPixels) / 2));
			height = (30 / (double(screenHeightPixels) / 2));

			textHeightStart = 0 + textOffset;
			textWidthStart = 0 + textOffset;
			textWidthEnd = textWidthStart + (63.0 / 2048.0) - textOffset * 2;
			textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
			//First triangle
			v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
			v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
			v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																																 //Second triangle
			v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
			amount++;

			bottomBarBasic = amount - 1;


			textWidthStart = (64 / 2048.0) + textOffset;
			textWidthEnd = textWidthStart + (63.0 / 2048.0) - textOffset * 2;

			//First triangle
			v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
			v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
			v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																																 //Second triangle
			v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
			amount++;

			bottomBarLabel = amount - 1;
		}
		// Left panel base, 2 = 1 pixel height section, 3 = display bar, 4 = display bar hover, 5 = display bar clicked, 6 = top tile display section show, 7 = middle tile display section show, 8 = bottom tile display section show
		{
			generate_Left_Panel_GUI(widthStart, heightStart, width, height, textHeightStart, textWidthStart, textWidthEnd, textHeightEnd, textOffset, amount, v);
		}
		// Left panel dropdown, 9 = drop down middle, 10 = drop down end, 11 = drop down hover
		{
			generate_Left_Panel_Drop_Down_GUI(widthStart, heightStart, width, height, textHeightStart, textWidthStart, textWidthEnd, textHeightEnd, textOffset, amount, v);
		}
		// Left panel select squares, 12 = Select square yellow small, 13 = Select square red small, 14 = Select square yellow Big, 15 = Select square red Big
		{
			generate_Left_Panel_select(widthStart, heightStart, width, height, textHeightStart, textWidthStart, textWidthEnd, textHeightEnd, textOffset, amount, v);
		}
		// Bottom bar eraser/destoryer toggles, 16 = Eraser, 17 = Eraser hover, 18 = Eraser selected, 19, 20, 21, Eraser/destroyer tile previews, 22 = eraser, 23 = destroyer
		// Left panel palette page buttons, 24 = left palette page hover, 25 = right palette page hover, 26 = left palette page pressed, 27 = left palette page pressed
		// Bottom bar, 28 = tileDestroyer, 29 = tileDestroyer hover, 30,
		{
			generate_BottomBar_Toggles(widthStart, heightStart, width, height, textHeightStart, textWidthStart, textWidthEnd, textHeightEnd, textOffset, amount, v);
		}
		// Item info
		{
			generate_Item_Info_Panel(widthStart, heightStart, width, height, textHeightStart, textWidthStart, textWidthEnd, textHeightEnd, textOffset, amount, v);
		}
		//
		{
			generate_Palette_Modifier_UI(widthStart, heightStart, width, height, textHeightStart, textWidthStart, textWidthEnd, textHeightEnd, textOffset, amount, v);
		}
	}

	vh.setAmount(amount);
	vh.addVerteces(v);
}

void generate_Left_Panel_GUI(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v) {
	
	// Filler 1px section
	width = (276 / (double(screenWidthPixels) / 2));
	height = (1 / (double(screenHeightPixels) / 2));

	textHeightStart = (154 / 2048.0) + textOffset;
	textWidthStart = 0 + textOffset;
	textWidthEnd = textWidthStart + (275.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (2.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	fillerPixel = amount - 1;

	// Display bar 
	height = (40 / (double(screenHeightPixels) / 2));
	textHeightStart = (31 / 2048.0) + textOffset;
	textHeightEnd = textHeightStart + (40.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	displayBar = amount - 1;

	// Display bar button normal 
	width = (21 / (double(screenWidthPixels) / 2));
	height = (21 / (double(screenHeightPixels) / 2));

	textWidthStart = (277.0 / 2048.0) + textOffset;
	textHeightStart = (31.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (21.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (21.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	displayBarButton = amount - 1;

	// Display bar button hover 
	width = (21 / (double(screenWidthPixels) / 2));
	height = (21 / (double(screenHeightPixels) / 2));

	textWidthStart = (299.0 / 2048.0) + textOffset;
	textHeightStart = (31.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (21.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (21.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	displayBarButtonHover = amount - 1;

	// Display bar button pressed 
	width = (21 / (double(screenWidthPixels) / 2));
	height = (21 / (double(screenHeightPixels) / 2));

	textWidthStart = (321.0 / 2048.0) + textOffset;
	textHeightStart = (31.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (21.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (21.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	displayBarButtonPressed = amount - 1;

	// Tile display top section
	width = (276 / (double(screenWidthPixels) / 2));
	height = (90 / (double(screenHeightPixels) / 2));

	textWidthStart = (0.0 / 2048.0) + textOffset;
	textHeightStart = (157.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (276.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (90.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaTop = amount - 1;

	// Tile display middle section
	width = (276 / (double(screenWidthPixels) / 2));
	height = (90 / (double(screenHeightPixels) / 2));

	textWidthStart = (0.0 / 2048.0) + textOffset;
	textHeightStart = (250.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (276.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (90.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaMiddle = amount - 1;

	// Tile display bottom section
	width = (276 / (double(screenWidthPixels) / 2));
	height = (90 / (double(screenHeightPixels) / 2));

	textWidthStart = (0.0 / 2048.0) + textOffset;
	textHeightStart = (343.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (276.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (90.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaBottom = amount - 1;

	// Tile display left button
	width = (17 / (double(screenWidthPixels) / 2));
	height = (23 / (double(screenHeightPixels) / 2));

	textWidthStart = (277.0 / 2048.0) + textOffset;
	textHeightStart = (53.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (17.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (23.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaLeftButton = amount - 1;

	// Tile display left button hover
	width = (17 / (double(screenWidthPixels) / 2));
	height = (23 / (double(screenHeightPixels) / 2));

	textWidthStart = (313.0 / 2048.0) + textOffset;
	textHeightStart = (53.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (17.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (23.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaLeftButtonHover = amount - 1;

	// Tile display left button Pressed
	width = (17 / (double(screenWidthPixels) / 2));
	height = (23 / (double(screenHeightPixels) / 2));

	textWidthStart = (349.0 / 2048.0) + textOffset;
	textHeightStart = (53.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (17.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (23.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaLeftButtonPressed = amount - 1;

	// Tile display right button
	width = (17 / (double(screenWidthPixels) / 2));
	height = (23 / (double(screenHeightPixels) / 2));

	textWidthStart = (295.0 / 2048.0) + textOffset;
	textHeightStart = (53.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (17.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (23.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaRightButton = amount - 1;

	// Tile display right button hover
	width = (17 / (double(screenWidthPixels) / 2));
	height = (23 / (double(screenHeightPixels) / 2));

	textWidthStart = (331.0 / 2048.0) + textOffset;
	textHeightStart = (53.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (17.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (23.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaRightButtonHover = amount - 1;

	// Tile display right button Pressed
	width = (17 / (double(screenWidthPixels) / 2));
	height = (23 / (double(screenHeightPixels) / 2));

	textWidthStart = (367.0 / 2048.0) + textOffset;
	textHeightStart = (53.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (17.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (23.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	tileAreaRightButtonPressed = amount - 1;
}

void generate_Left_Panel_Drop_Down_GUI(float & widthStart, float & heightStart, double & width, double & height, float & textHeightStart, float & textWidthStart, float & textWidthEnd, float & textHeightEnd, float & textOffset, size_t & amount, std::vector<float>& v)
{
	// Filler drop down middle
	width = (259 / (double(screenWidthPixels) / 2));
	height = (36 / (double(screenHeightPixels) / 2));

	textWidthStart = (0.0 / 2048.0) + textOffset;
	textHeightStart = (436.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (259.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (36.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	leftPanelDropDownMiddleSection = amount - 1;

	// Filler drop down middle
	width = (259 / (double(screenWidthPixels) / 2));
	height = (38 / (double(screenHeightPixels) / 2));

	textWidthStart = (0.0 / 2048.0) + textOffset;
	textHeightStart = (474.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (259.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (38.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	leftPanelDropDownBottomSection = amount - 1;

	// Filler drop down end
	width = (256 / (double(screenWidthPixels) / 2));
	height = (36 / (double(screenHeightPixels) / 2));

	textWidthStart = (0.0 / 2048.0) + textOffset;
	textHeightStart = (513.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (256.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (36.0 / 2048.0) - textOffset * 2;

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	leftPanelDropDownHover = amount - 1;
}

void generate_Left_Panel_select(float & widthStart, float & heightStart, double & width, double & height, float & textHeightStart, float & textWidthStart, float & textWidthEnd, float & textHeightEnd, float & textOffset, size_t & amount, std::vector<float>& v)
{
	// Select square yellow small
	width = (64 / (double(screenWidthPixels) / 2));
	height = (64 / (double(screenHeightPixels) / 2));

	textHeightStart = (550 / 2048.0) + textOffset;
	textWidthStart = 0 + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	leftPanelYellowSquareSmall = amount - 1;

	// Select square red small
	width = (64 / (double(screenWidthPixels) / 2));
	height = (64 / (double(screenHeightPixels) / 2));

	textHeightStart = (615 / 2048.0) + textOffset;
	textWidthStart = 0 + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	leftPanelRedSquareSmall = amount - 1;

	// Select square yellow Big
	width = (128 / (double(screenWidthPixels) / 2));
	height = (128 / (double(screenHeightPixels) / 2));

	textHeightStart = (550 / 2048.0) + textOffset;
	textWidthStart = (65 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (128.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (128.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	leftPanelYellowSquareBig = amount - 1;

	// Select square red Big
	width = (128 / (double(screenWidthPixels) / 2));
	height = (128 / (double(screenHeightPixels) / 2));

	textHeightStart = (550 / 2048.0) + textOffset;
	textWidthStart = (194 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (128.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (128.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	leftPanelRedSquareBig = amount - 1;
}

void generate_BottomBar_Toggles(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v) {
	// Eraser
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (130 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
		//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarEraser = amount - 1;

	// Eraser hover
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (161 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
		//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarEraserHover = amount - 1;

	// Eraser toggle
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (192 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
		//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarEraserPressed = amount - 1;

	// Destroyer
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (223 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
		//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarDestroyer = amount - 1;

	// Destroyer hover
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (254 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
		//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarDestroyerHover = amount - 1;

	// Destroyer toggle
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (285 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
		//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarDestroyerPressed = amount - 1;

	// Eraser
	width = (64 / (double(screenWidthPixels) / 2));
	height = (64 / (double(screenHeightPixels) / 2));

	textHeightStart = (680 / 2048.0) + textOffset;
	textWidthStart = 0 + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	eraseToggleID = amount - 1;

	// Destroyer
	width = (64 / (double(screenWidthPixels) / 2));
	height = (64 / (double(screenHeightPixels) / 2));

	textHeightStart = (745 / 2048.0) + textOffset;
	textWidthStart = 0 + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	destroyToggleID = amount - 1;

	// tileDestroyer
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (316 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarTileDestroyer = amount - 1;

	// tileDestroyer hover
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (347 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarTileDestroyerHover = amount - 1;

	// tileDestroyer pressed
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (378 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarTileDestroyerPressed = amount - 1;

	// cut
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (409 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarCut = amount - 1;

	// cut hover
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (440 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarCutHover = amount - 1;

	// cut pressed
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (471 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarCutPressed = amount - 1;

	// Copy
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (502 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarCopy = amount - 1;

	// Copy hover
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (533 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarCopyHover = amount - 1;

	// Copy pressed
	width = (30 / (double(screenWidthPixels) / 2));
	height = (30 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (564 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (30.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	bottomBarCopyPressed = amount - 1;

	// Cut pressed
	width = (64 / (double(screenWidthPixels) / 2));
	height = (64 / (double(screenHeightPixels) / 2));

	textHeightStart = (680 / 2048.0) + textOffset;
	textWidthStart = (65 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	cutToggleID = amount - 1;

	// Copy pressed
	width = (64 / (double(screenWidthPixels) / 2));
	height = (64 / (double(screenHeightPixels) / 2));

	textHeightStart = (745 / 2048.0) + textOffset;
	textWidthStart = (65 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	copyToggleID = amount - 1;

	// tileDestroyer pressed
	width = (64 / (double(screenWidthPixels) / 2));
	height = (64 / (double(screenHeightPixels) / 2));

	textHeightStart = (680 / 2048.0) + textOffset;
	textWidthStart = (130 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	destroyTileToggleID = amount - 1;
}

void generate_Item_Info_Panel(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v) {
	// Base panel
	width = (1000 / (double(screenWidthPixels) / 2));
	height = (700 / (double(screenHeightPixels) / 2));

	textHeightStart = 0 + textOffset;
	textWidthStart = (1048 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (1000.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (700.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoPanelID = amount - 1;

	// Text section
	width = (150 / (double(screenWidthPixels) / 2));
	height = (40 / (double(screenHeightPixels) / 2));

	textHeightStart = (1.0 / 2048.0) + textOffset;
	textWidthStart = (897.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (150.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (40.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoTextSectionID = amount - 1;

	// Right arrow
	width = (30 / (double(screenWidthPixels) / 2));
	height = (43 / (double(screenHeightPixels) / 2));

	textHeightStart = (42.0 / 2048.0) + textOffset;
	textWidthStart = (924.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (43.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoRightArrowID = amount - 1;

	// Right arrow hover
	width = (30 / (double(screenWidthPixels) / 2));
	height = (43 / (double(screenHeightPixels) / 2));

	textHeightStart = (42.0 / 2048.0) + textOffset;
	textWidthStart = (893.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (43.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoRightArrowHoverID = amount - 1;

	// Right arrow pressed
	width = (30 / (double(screenWidthPixels) / 2));
	height = (43 / (double(screenHeightPixels) / 2));

	textHeightStart = (42.0 / 2048.0) + textOffset;
	textWidthStart = (862.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (43.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoRightArrowPressedID = amount - 1;

	// Left arrow
	width = (30 / (double(screenWidthPixels) / 2));
	height = (43 / (double(screenHeightPixels) / 2));

	textHeightStart = (42.0 / 2048.0) + textOffset;
	textWidthStart = (1017.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (43.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoLeftArrowID = amount - 1;

	// Left arrow hover
	width = (30 / (double(screenWidthPixels) / 2));
	height = (43 / (double(screenHeightPixels) / 2));

	textHeightStart = (42.0 / 2048.0) + textOffset;
	textWidthStart = (986.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (43.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoLeftArrowHoverID = amount - 1;

	// Left arrow pressed
	width = (30 / (double(screenWidthPixels) / 2));
	height = (43 / (double(screenHeightPixels) / 2));

	textHeightStart = (42.0 / 2048.0) + textOffset;
	textWidthStart = (955.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (30.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (43.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoLeftArrowPressedID = amount - 1;

	// Text area
	width = (150 / (double(screenWidthPixels) / 2));
	height = (110 / (double(screenHeightPixels) / 2));

	textHeightStart = (86.0 / 2048.0) + textOffset;
	textWidthStart = (897.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (150.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (110.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoTextAreaID = amount - 1;

	// Button
	width = (150 / (double(screenWidthPixels) / 2));
	height = (26 / (double(screenHeightPixels) / 2));

	textHeightStart = (197.0 / 2048.0) + textOffset;
	textWidthStart = (897.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (150.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (26.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoButtonID = amount - 1;

	// Button hover
	width = (150 / (double(screenWidthPixels) / 2));
	height = (26 / (double(screenHeightPixels) / 2));

	textHeightStart = (224.0 / 2048.0) + textOffset;
	textWidthStart = (897.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (150.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (26.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoButtonHoverID = amount - 1;

	// Button pressed
	width = (150 / (double(screenWidthPixels) / 2));
	height = (26 / (double(screenHeightPixels) / 2));

	textHeightStart = (251.0 / 2048.0) + textOffset;
	textWidthStart = (897.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (150.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (26.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	itemInfoButtonPressedID = amount - 1;
}

void generate_Palette_Modifier_UI(float& widthStart, float& heightStart, double& width, double& height, float& textHeightStart, float& textWidthStart, float &textWidthEnd, float &textHeightEnd, float &textOffset, size_t &amount, std::vector<float> &v) {
	// Base panel
	width = (679 / (double(screenWidthPixels) / 2));
	height = (724 / (double(screenHeightPixels) / 2));

	textWidthStart = (1369.0 / 2048.0) + textOffset;
	textHeightStart = (701.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (679.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (724.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierPanel = amount - 1;

	// Search Icon base
	width = (22 / (double(screenWidthPixels) / 2));
	height = (22 / (double(screenHeightPixels) / 2));

	textWidthStart = (1174.0 / 2048.0) + textOffset;
	textHeightStart = (701.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierSearchIcon = amount - 1;

	// Search Icon hover
	width = (22 / (double(screenWidthPixels) / 2));
	height = (22 / (double(screenHeightPixels) / 2));

	textWidthStart = (1239.0 / 2048.0) + textOffset;
	textHeightStart = (701.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierSearchIconHover = amount - 1;

	// Search Icon pressed
	width = (22 / (double(screenWidthPixels) / 2));
	height = (22 / (double(screenHeightPixels) / 2));

	textWidthStart = (1304.0 / 2048.0) + textOffset;
	textHeightStart = (701.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (64.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (64.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierSearchIconPressed = amount - 1;

	// DropDop toggle unpressed
	width = (102 / (double(screenWidthPixels) / 2));
	height = (24 / (double(screenHeightPixels) / 2));

	textWidthStart = (1163.0 / 2048.0) + textOffset;
	textHeightStart = (766.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (102.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (24.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierDropDownUnPressed = amount - 1;

	// DropDop toggle pressed
	width = (102 / (double(screenWidthPixels) / 2));
	height = (24 / (double(screenHeightPixels) / 2));

	textWidthStart = (1266.0 / 2048.0) + textOffset;
	textHeightStart = (766.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (102.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (24.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierDropDownPressed = amount - 1;

	// DropDopElement
	width = (102 / (double(screenWidthPixels) / 2));
	height = (24 / (double(screenHeightPixels) / 2));

	textWidthStart = (1163.0 / 2048.0) + textOffset;
	textHeightStart = (791.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (102.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (24.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierDropDownElement = amount - 1;

	// DropDopElement hover
	width = (102 / (double(screenWidthPixels) / 2));
	height = (24 / (double(screenHeightPixels) / 2));

	textWidthStart = (1266.0 / 2048.0) + textOffset;
	textHeightStart = (791.0 / 2048.0) + textOffset;
	textWidthEnd = textWidthStart + (102.0 / 2048.0) - textOffset * 2;
	textHeightEnd = textHeightStart + (24.0 / 2048.0) - textOffset * 2;
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierDropDownElementHover = amount - 1;

	// Empty tile marker
	width = (64 / (double(screenWidthPixels) / 2));
	height = (64 / (double(screenHeightPixels) / 2));

	textWidthStart = (1109.0 / 2048.0);
	textHeightStart = (701.0 / 2048.0);
	textWidthEnd = textWidthStart + (64.0 / 2048.0);
	textHeightEnd = textHeightStart + (64.0 / 2048.0);
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthEnd); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightEnd); // Bottom-left

																														 //Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthEnd); v.push_back(textHeightEnd); // Bottom-right
	amount++;

	paletteModifierEmptyTileMarker = amount - 1;
}