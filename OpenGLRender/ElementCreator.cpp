#include "ElementCreator.h"


void ElementCreator::createGUIs(std::vector<float>& v)
{
	if (updateGUI) {
		GUIElements.clear();
		GUIElements.shrink_to_fit();

		GUIVerteces.clear();
		GUIVerteces.shrink_to_fit();

		if (drawLeftPanelGUI) {
			leftHandGUISection(GUIVerteces);
		}

		bottomBar(GUIVerteces);

		//tileDrawWindowScrollBars(GUIVerteces);

		tileInfoWindowCreate(GUIVerteces);

		GUILetters(GUIVerteces);

		previewTiles(GUIVerteces);

		updateGUI = false;
	}
	
}


size_t ElementCreator::createDropDownMenuSlots(float widthStart, float heightStart, float width, float height, float textWidthStart, float textHeightStart, float textWidth, float textHeight, float hoverSlotTextHeightStart, float borderSlotTextHeightStart, float borderHeight, float borderTextHeight, std::vector<float>& v, size_t size, size_t hoverSlot)
{
	size_t elements = 0;
	for (size_t i = 0; i < size; ++i) {

		if (hoverSlot != i) {
			//Create drop down menu slot
			elements += createSquare(widthStart, heightStart - (height * i), width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		}
		else {
			//Create drop down menu slot with a blue background
			elements += createSquare(widthStart, heightStart - (height * i), width, height, textWidthStart, hoverSlotTextHeightStart, textWidth, textHeight, v);
		}

		//For drawing the last border
		if (i == size - 1) {
			++i;
			//Create drop down menu last slot border
			elements += createSquare(widthStart, heightStart - (height * i), width, borderHeight, textWidthStart, borderSlotTextHeightStart, textWidth, borderTextHeight, v);
		}
	}
	return elements;
}

size_t ElementCreator::createSquare(float widthStart, float heightStart, float width, float height, float textWidthStart, float textHeightStart, float textWidth, float textHeight, vector<float>& v)
{
	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthStart + textWidth); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightStart - textHeight); // Bottom-left

																																		//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthStart + textWidth); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart + width); v.push_back(heightStart - height); v.push_back(textWidthStart + textWidth); v.push_back(textHeightStart - textHeight); // Bottom-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightStart - textHeight); // Bottom-left

	return 1;
}

size_t ElementCreator::createCharacters(float widthStart, float heightStart, float width, float height, std::string input, vector<float>& v, bool offsetBool)
{
	size_t elements = input.size();
	float offsetStart = 0.0f;
	if (offsetBool) {
		for (const auto& c : input) {
			offsetStart += OFFSET_PER_CHARACTER[c];
		}
	}
	widthStart -= ((offsetStart / 2) / 960.0f);
	float offset = 0.0f;
	std::vector<float> characterTextures = getCharacterTextures(input);
	for (size_t i = 0; i < (characterTextures.size() / 12); ++i) {
		if (i > 0)  offset += (OFFSET_PER_CHARACTER[input.at(i - 1)]) / 960;

		//First triangle
		v.push_back(widthStart + (width * i) - (offset)); v.push_back(heightStart); v.push_back(characterTextures.at(0 + (i * 12))); v.push_back(characterTextures.at(1 + (i * 12))); // Top-left
		v.push_back(widthStart + width + (width * i) - (offset)); v.push_back(heightStart); v.push_back(characterTextures.at(2 + (i * 12))); v.push_back(characterTextures.at(3 + (i * 12))); // Top-right
		v.push_back(widthStart + (width * i) - (offset)); v.push_back(heightStart - height); v.push_back(characterTextures.at(4 + (i * 12))); v.push_back(characterTextures.at(5 + (i * 12))); // Bottom-left

																																															   //Second triangle
		v.push_back(widthStart + width + (width * i) - (offset)); v.push_back(heightStart); v.push_back(characterTextures.at(6 + (i * 12))); v.push_back(characterTextures.at(7 + (i * 12))); // Top-right
		v.push_back(widthStart + width + (width * i) - (offset)); v.push_back(heightStart - height); v.push_back(characterTextures.at(8 + (i * 12))); v.push_back(characterTextures.at(9 + (i * 12))); // Bottom-right
		v.push_back(widthStart + (width * i) - (offset)); v.push_back(heightStart - height); v.push_back(characterTextures.at(10 + (i * 12))); v.push_back(characterTextures.at(11 + (i * 12))); // Bottom-left
	}
	return elements;
}

vector<float> ElementCreator::getCharacterTextures(std::string input)
{
	float textWidthStart = 0.0f;
	float textHeightStart = 1.0f;
	float textWidth = 16.0f / 2048;
	float textHeight = 16.0f / 2048;
	size_t character = 0;
	size_t texi = 0;
	size_t texj = 0;
	std::vector<float> v;
	for (const auto& c : input) {
		character = c;
		texi = 0;
		texj = 0;
		while (character >= 16) {
			++texj;
			character -= 16;
		}
		texi = character;

		//First triangle
		v.push_back(textWidthStart + (textWidth * texi)); v.push_back(textHeightStart - (textHeight * texj)); // Top-left
		v.push_back(textWidthStart + textWidth + (textWidth * texi)); v.push_back(textHeightStart - (textHeight * texj)); // Top-right
		v.push_back(textWidthStart + (textWidth * texi)); v.push_back(textHeightStart - textHeight - (textHeight * texj)); // Bottom-left

																														   //Second triangle
		v.push_back(textWidthStart + textWidth + (textWidth * texi)); v.push_back(textHeightStart - (textHeight * texj)); // Top-right
		v.push_back(textWidthStart + textWidth + (textWidth * texi)); v.push_back(textHeightStart - textHeight - (textHeight * texj)); // Bottom-right
		v.push_back(textWidthStart + (textWidth * texi)); v.push_back(textHeightStart - textHeight - (textHeight * texj)); // Bottom-left

	}
	return v;
}

void ElementCreator::selectionAreaFill(std::vector<float>& v)
{
	//-----------------------------------------------
	//				SELECTION AREA FILL
	//-----------------------------------------------

	if (GUILeftPanelElement[3] != 1000) {
		if (getItemsToRenderOnSelectionArea) {
			pos = itemAtlas.getItemTexturePositionForSelectionArea(LEFFT_PANEL_DROP_DOWN_TEXT[GUILeftPanelElement[3]]);
			getItemsToRenderOnSelectionArea = false;
		}
		max_selected = pos.size();
		// Fill selction area
		widthStart = xCameraPos + 1.0f - (464.0f / 960) + (5.0f / 960);
		heightStart = yCameraPos  + -1.0f + (899.0f / 540) - (5.0f / 540);
		width = 60.0f / 960;
		height = (60.0f / 540);
		textWidth = 64.0f / 2048;
		textHeight = 64.0f / 2048;
		textHeightStart = 1.0f;
		textWidthStart = 0.0;

		size_t i = 0;
		size_t j = 0;
		size_t iText = 0;
		size_t jText = 0;
		for (const auto& p : pos) {

			if ((contentSelectionSelected[0] == i && contentSelectionSelected[1] == j) || (contentSelectionHover[0] == i && contentSelectionHover[1] == j)) {
				// Draw selection area hover unclicked
				createSquare(widthStart + (width * i), heightStart - (height * j), width, height, 0.0f + (540.0f / 2048), textHeightStart, 60.0f / 2048, 60.0f / 2048, v);
				GUIElements.push_back(1);
			}

			// Draw selection area content
			createSquare(widthStart + (width * i), heightStart - (height * j), width, height, textWidthStart + (textWidth * iText), textHeightStart - (textHeight * jText), textWidth, textHeight, v);
			GUIElements.push_back(p);

			if (contentSelectionSelected[0] == i && contentSelectionSelected[1] == j) {
				// Draw selection area hover clicked
				createSquare(widthStart + (width * i), heightStart - (height * j), width, height, 0.0f + (480.0f / 2048), textHeightStart - (60.0f / 2048.0f), 60.0f / 2048, 60.0f / 2048, v);
				GUIElements.push_back(1);
			}
			else if (contentSelectionHover[0] == i && contentSelectionHover[1] == j) {
				// Draw selection area hover unclicked
				createSquare(widthStart + (width * i), heightStart - (height * j), width, height, 0.0f + (480.0f / 2048), textHeightStart, 60.0f / 2048, 60.0f / 2048, v);
				GUIElements.push_back(1);
			}

			if (++i >= 7) {
				i -= 7;
				++j;
			}
			if (++iText >= 32) {
				iText -= 32;
				++jText;
			}
		}
	}
	//-----------------------------------------------
	//				DONE SELECTION AREA FILL
	//-----------------------------------------------
}

void ElementCreator::leftHandGUISection(std::vector<float>& v)
{
	size_t size = 0;
	//-----------------------------------------------
	//				Left hand GUI bar
	//-----------------------------------------------
	widthStart = xCameraPos + 1.0f - 0.5f;
	heightStart = yCameraPos + 1.0f;
	width = 0.5f;
	height = 2.0f;
	textWidth = (480.0f / 2048.0f);
	textHeight = 1080.0f / 2048;
	textHeightStart = 1.0f;
	textWidthStart = 0.0f;

	// Draw the Left Panel
	createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
	GUIElements.push_back(size_t(1));

	// Create selection area
	widthStart = xCameraPos + 1.0f - 0.5f + (16.0f / 960.0f);
	heightStart = yCameraPos + 1.0f - (175.0f / 540.0f);
	width = 0.5f - (32.0f / 960.0f);
	height = (850.0f / 540.0f);
	textWidth = (448.0f / 2048.0f);
	textHeight = 850.0f / 2048.0f;
	textHeightStart = 1.0f - (230.0f / 2048.0f);
	textWidthStart = 0.0f + (480.0f / 2048.0f);

	// Draw the selection area
	createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
	GUIElements.push_back(size_t(1));

	//-----------------------------------------------
	//				SELECTION AREA FILL
	//-----------------------------------------------
	
	selectionAreaFill(v);

	//-----------------------------------------------
	//				DONE SELECTION AREA FILL
	//-----------------------------------------------

	// Left panel drop down button
	if (GUILeftPanelElement[0] == 0) {
		widthStart = xCameraPos + 1.0f - (69.0f / 960.0f);
		heightStart = yCameraPos + 1.0f - (110.0f / 540.0f);
		width = (40.0f / 960.0f);
		height = (28.0f / 540.0f);
		textWidth = (40.0f / 2048.0f);
		textHeight = 28.0f / 2048.0f;
		textHeightStart = 1.0f;
		textWidthStart = 0.0f + (1880.0f / 2048.0f);

		// Draw the left panel drop down button
		createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));

	}
	else if (GUILeftPanelElement[0] == 1) {
		widthStart = xCameraPos + 1.0f - (69.0f / 960.0f);
		heightStart = yCameraPos + 1.0f - (110.0f / 540.0f);
		width = (40.0f / 960.0f);
		height = (28.0f / 540.0f);
		textWidth = (40.0f / 2048.0f);
		textHeight = 28.0f / 2048.0f;
		textHeightStart = 1.0f;
		textWidthStart = 0.0f + (1840.0f / 2048.0f);

		// Draw the left panel drop down button
		createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));
	}

	// Left panel drop down menu
	if (GUILeftPanelElement[0] == 1 && GUILeftPanelElement[1] > 0) {

		widthStart = xCameraPos + (511.0f / 960);
		heightStart = yCameraPos + 1.0f - (145.0f / 540);
		width = 0.5f - (31.0f + 21.0f) / 960;
		height = (41.0f / 540);
		float textWidth = 427.0f / 2048;
		float textHeight = 41.0f / 2048;
		float textHeightStart = 1.0f - (28.0f / 2048);
		float textWidthStart = 0.0f + (1493.0f / 2048.0f);
		// Draw the elements of the drop down list
		size = 0;
		size += createDropDownMenuSlots(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, 1.0f - (70.0f / 2048), 1.0f - (112.0f / 2048), (5.0f / 1080) * 2,
			(5.0f / 1080), v, GUILeftPanelElement[1], GUILeftPanelElement[2]);
		for (size_t k = 0; k < size; ++k) {
			GUIElements.push_back(size_t(1));
		}

	}
	{ // Erase toggle button
		widthStart = xCameraPos + 0.5f + (31.0f / 960.0f);
		heightStart = yCameraPos + 1.0f - (40.0f / 540.0f);
		width = 79.0f / 960.0f;
		height = 28.0f / 540.0f;
		textWidth = (79.0f / 2048.0f);
		textHeight = 28.0f / 2048;
		textWidthStart = 1761.0f / 2048;
		textHeightStart = 1.0f;

		if (!eraseToggle) {
			// Draw the untoggeled button
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		}
		else // Draw the toggled button
			createSquare(widthStart, heightStart, width, height, textWidthStart - (79.0f / 2048), textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));

		widthStart += (82.0f / 960);
		if (!destroyToggle) {
			// Draw the untoggeled button
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		}
		else // Draw the toggled button
			createSquare(widthStart, heightStart, width, height, textWidthStart - (79.0f / 2048), textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));
	}


	//-----------------------------------------------
	//				DONE LEFT PANEL
	//-----------------------------------------------
}

void ElementCreator::leftHandGUILetters(std::vector<float>& v)
{
	if (GUILeftPanelElement[0] == 1 && GUILeftPanelElement[1] > 0) {
		widthStart = xCameraPos + (511.0f / 960) + (0.5f - (110.0f) / 960) / 2 - (15.0f / 960);
		heightStart = yCameraPos + 1.0f - (145.0f / 540) - 0.02f;
		width = (16.0f) / 960;
		height = (16.0f / 540);
		// Keep track of when letters start so that we may swap texture
		size = 0;
		for (size_t i = 0; i < GUILeftPanelElement[1]; ++i) {
			// Draw the content of the drop down list
			size += createCharacters(widthStart, heightStart - ((41.0f / 540) * i), width, height, LEFFT_PANEL_DROP_DOWN_TEXT[i], v, false);
		}
		GUIElements.insert(GUIElements.end(), size, 2);
		/*for (size_t k = 0; k < size; ++k) {
			GUIElements.push_back(size_t(2));
		}*/
	}
	
	if (GUILeftPanelElement[3] < SIZE_DROP_DOWN_TEXT) {
		size = 0;
		// Draw the word of the choosen category from the drop down list.
		widthStart = xCameraPos + (511.0f / 960) + (0.5f - (110.0f) / 960) / 2;
		heightStart = yCameraPos + 1.0f - (109.0f / 540) - 0.02f;
		width = (16.0f) / 960;
		height = (16.0f / 540);

		size += createCharacters(widthStart, heightStart, width, height, LEFFT_PANEL_DROP_DOWN_TEXT[GUILeftPanelElement[3]], v, true);
		GUIElements.insert(GUIElements.end(), size, 2);
		/*for (size_t k = 0; k < size; ++k) {
			GUIElements.push_back(size_t(2));
		}*/
	}
	
	{	// Draw letters for the left panel toggle buttons
		widthStart = xCameraPos + 0.5f + (67.0f / 960.0f);
		heightStart = yCameraPos + 1.0f - (46.0f / 540.0f);
		width = 16.0f / 960.0f;
		height = 16.0f / 540.0f;

		size = 0;
		// Draw "Eraser" on toggle button
		size += createCharacters(widthStart, heightStart, width, height, "Eraser", v, true);
		// Draw "Destroyer" on toggle button
		size += createCharacters(widthStart + (85.0f / 960.0f), heightStart, width - (1.0f / 960.0f), height - (1.0f / 540.0f), "Destroyer", v, true);

		GUIElements.insert(GUIElements.end(), size, 2);
		/*for (size_t k = 0; k < size; ++k) {
			GUIElements.push_back(size_t(2));
		}*/
	}
}

void ElementCreator::bottomBar(std::vector<float>& v)
{
	//-----------------------------------------------
	//				BOTTOM BAR
	//-----------------------------------------------

	{	// First half of the bar
		widthStart = xCameraPos + -1.0f;
		heightStart = yCameraPos + -1.0f + (29.0f / 540);
		width = 1.0f;
		height = (29.0f / 540);
		textWidth = (960.0f / 2048);
		textHeight = 29.0f / 2048;
		textHeightStart = 1.0f - (1051.0f / 2048);
		textWidthStart = 0.0f + (960.0f / 2048);

		// Draw the first half of the bottom bar
		createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));
	}

	{	// Second half of the bar
		widthStart = xCameraPos;
		heightStart = yCameraPos -1.0f + (29.0f / 540);
		width = 1.0f;
		height = (29.0f / 540);
		textWidth = (960.0f / 2048);
		textHeight = 29.0f / 2048;
		textHeightStart = 1.0f - (1022.0f / 2048);
		textWidthStart = 0.0f + (960.0f / 2048);

		// Draw the second half of the bottom bar
		createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));
	}
	

	//-----------------------------------------------
	//				DONE BOTTOM BAR
	//-----------------------------------------------
}

void ElementCreator::bottomBarLetters(std::vector<float>& v)
{
	{	// Draw the x, y, z cords on the first half of the bottom bar
		size = 0;

		widthStart = xCameraPos + -1.0f + (34.0f / 960) + (4.0f / 960);
		heightStart = yCameraPos + -1.0f + (16.0f / 540) + 0.011f;
		width = (16.0f) / 960;
		height = (16.0f / 540);

		// Draw X
		size += createCharacters(widthStart, heightStart, width, height, "X:", v, true);
		// Draw X coord
		size += createCharacters(widthStart + (width * 2), heightStart, width, height, std::to_string(x), v, true);
		// Draw Y
		size += createCharacters(widthStart + (width * 4) - (6.0f / 960), heightStart, width, height, "Y:", v, true);
		// Draw Y coord
		size += createCharacters(widthStart + (width * 6) - (6.0f / 960), heightStart, width, height, std::to_string(y), v, true);
		// Draw Z
		size += createCharacters(widthStart + (width * 8) - (12.0f / 960), heightStart, width, height, "Z:", v, true);
		// Draw Z coord
		size += createCharacters(widthStart + (width * 10) - (12.0f / 960), heightStart, width, height, std::to_string(currentSection), v, true);

		GUIElements.insert(GUIElements.end(), size, 2);
	}
}

void ElementCreator::tileDrawWindowScrollBars(std::vector<float>& v)
{
	//-----------------------------------------------
	//				SCROLL BARS
	//-----------------------------------------------
	{	//	Square between the two bars.
		widthStart = 1.0f - (499.0f / 960);
		heightStart = -1.0f + (48.0f / 540);
		width = (19.0f / 960);
		height = (19.0f / 540);
		textWidth = (19.0f / 2048.0f);
		textHeight = (19.0f / 2048.0f);
		textHeightStart = 1.0f - (1003.0f / 2048.0f);
		textWidthStart = 960.0f / 2048;

		// Draw square
		createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));
	}
	{	//	Vertical scroll bar
		widthStart = 1.0f - (499.0f / 960);
		heightStart = 1.0f;
		width = (19.0f / 960);
		height = (1032.0f / 540);
		textWidth = (19.0f / 2048.0f);
		textHeight = (1032.0f / 2048.0f);
		textHeightStart = 1.0f - (48.0f / 2048);
		textWidthStart = 941.0f / 2048.0f;

		// Draw vertical scroll bar
		createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));

		// Hover over up/down button
		if (scrollBarHover == 1) {
			widthStart = 1.0f - (499.0f / 960);
			heightStart = 1.0f;
			width = (19.0f / 960);
			height = (14.0f / 540);
			textWidth = (19.0f / 2048);
			textHeight = (14.0f / 2048.0f);
			textHeightStart = 1.0f - (989.0f / 2048.0f);
			textWidthStart = 960.0f / 2048.0f;

			// Draw hover up button
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
			GUIElements.push_back(size_t(1));
		}
		if (scrollBarHover == 2) {
			widthStart = 1.0f - (499.0f / 960);
			heightStart = -1.0f + (62.0f / 540);
			width = (19.0f / 960);
			height = (14.0f / 540);
			textWidth = (19.0f / 2048);
			textHeight = (14.0f / 2048.0f);
			textHeightStart = 1.0f - (975.0f / 2048.0f);
			textWidthStart = 960.0f / 2048.0f;

			// Draw hover up button
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
			GUIElements.push_back(size_t(1));
		}
	}
	{	// Draw vertical scrollbar drag
		widthStart = 0.5f - (19.0f / 960);
		heightStart = 1.0f - (verticalScrollBarY / 540.0f) + (20.0f / 540);
		width = (19.0f / 960);
		height = (40.0f / 540);
		textWidth = (19.0f / 2048);
		textHeight = (40.0f / 2048.0f);
		textHeightStart = 1.0f - (855.0f / 2048.0f);
		textWidthStart = 960.0f / 2048.0f;

		if (verticalScrollbar) {
			// Draw square
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart - (40.0f / 2048.0f), textWidth, textHeight, v);
		}
		else if (verticalScrollBarHover) {
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart - (80.0f / 2048.0f), textWidth, textHeight, v);
		}
		else {
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		}
		GUIElements.push_back(size_t(1));
	}
	{	//	Horizontal scroll bar
		widthStart = -1.0f;
		heightStart = -1.0f + (48.0f / 540);
		width = (1421.0f / 960);
		height = (19.0f / 540);
		textWidth = (941.0f / 2048.0f);
		textHeight = (19.0f / 2048.0f);
		textHeightStart = 1.0f - (1003.0f / 2048);
		textWidthStart = 979.0f / 2048.0f;

		// Draw vertical scroll bar
		createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
		GUIElements.push_back(size_t(1));


		// Hover over up/down button
		if (true) {
			widthStart = -1.0f;
			heightStart = -1.0f + (48.0f / 540);
			width = (15.0f / 960);
			height = (19.0f / 540);
			textWidth = (15.0f / 2048.0f);
			textHeight = (19.0f / 2048.0f);
			textHeightStart = 1.0f - (984.0f / 2048);
			textWidthStart = 992.0f / 2048.0f;

			// Draw hover up button
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
			GUIElements.push_back(size_t(1));
		}
		if (true) {
			widthStart = -1.0f + (1405.0f / 960);
			heightStart = -1.0f + (48.0f / 540);
			width = (16.0f / 960);
			height = (19.0f / 540);
			textWidth = (16.0f / 2048.0f);
			textHeight = (19.0f / 2048.0f);
			textHeightStart = 1.0f - (984.0f / 2048);
			textWidthStart = 979.0f / 2048.0f;

			// Draw hover up button
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
			GUIElements.push_back(size_t(1));
		}
		{	// Draw vertical scrollbar drag
			widthStart = -1.0f + (horizontalScrollBarX / 960.0f) - (20.0f / 960);
			heightStart = -1.0f + (48.0f / 540);
			width = (40.0f / 960);
			height = (19.0f / 540);
			textWidth = (40.0f / 2048.0f);
			textHeight = (19.0f / 2048.0f);
			textHeightStart = 1.0f - (984.0f / 2048);
			textWidthStart = 1088.0f / 2048.0f;

			if (horizontalScrollbar) {
				// Draw square
				createSquare(widthStart, heightStart, width, height, textWidthStart - (40.0f / 2048.0f), textHeightStart, textWidth, textHeight, v);
			}
			else if (horizontalScrollBarHover) {
				createSquare(widthStart, heightStart, width, height, textWidthStart - (80.0f / 2048.0f), textHeightStart, textWidth, textHeight, v);
			}
			else {
				createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, v);
			}
			GUIElements.push_back(size_t(1));
		}

	}
	//-----------------------------------------------
	//				DONE SCROLL BARS
	//-----------------------------------------------
}

void ElementCreator::tileInfoWindowCreate(std::vector<float>& v)
{
	//-----------------------------------------------
	//				TILE INFO WINDOW
	//-----------------------------------------------
	if (tileInfoWindow) { // Create window
		if (tileWindowUpdate) {

			tileWindowVerteces.clear();
			tileWindowElements.clear();

			tileWindowTextVerteces.clear();

			widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f);
			heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f);
			width = (750.0f / 960);
			height = (600.0f / 540);
			textWidth = (750.0f / 2048.0f);
			textHeight = (600.0f / 2048.0f);
			textHeightStart = 1.0f - (118.0f / 2048);
			textWidthStart = 1170.0f / 2048.0f;

			// Draw tile info window
			createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, tileWindowVerteces);
			tileWindowElements.push_back(size_t(1));

			if (!changeItemLeft) {
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (558.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (130.0f / 540.0f);
				width = (31.0f / 960.0f);
				height = (31.0f / 540.0f);
				textWidth = (31.0f / 2048.0f);
				textHeight = 31.0f / 2048.0f;
				textHeightStart = 1.0f;
				textWidthStart = 0.0f + (1462.0f / 2048.0f);

				// Draw the left panel drop down button
				createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, tileWindowVerteces);
				tileWindowElements.push_back(size_t(1));

			}
			else if (changeItemLeft) {
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (558.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (130.0f / 540.0f);
				width = (31.0f / 960.0f);
				height = (31.0f / 540.0f);
				textWidth = (31.0f / 2048.0f);
				textHeight = 31.0f / 2048.0f;
				textHeightStart = 1.0f;
				textWidthStart = 0.0f + (1400.0f / 2048.0f);

				// Draw the left panel drop down button
				createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, tileWindowVerteces);
				tileWindowElements.push_back(size_t(1));
			}

			if (!changeItemRight) {
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (671.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (130.0f / 540.0f);
				width = (31.0f / 960.0f);
				height = (31.0f / 540.0f);
				textWidth = (31.0f / 2048.0f);
				textHeight = 31.0f / 2048.0f;
				textHeightStart = 1.0f;
				textWidthStart = 0.0f + (1431.0f / 2048.0f);

				// Draw the left panel drop down button
				createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, tileWindowVerteces);
				tileWindowElements.push_back(size_t(1));

			}
			else if (changeItemRight) {
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (671.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (130.0f / 540.0f);
				width = (31.0f / 960.0f);
				height = (31.0f / 540.0f);
				textWidth = (31.0f / 2048.0f);
				textHeight = 31.0f / 2048.0f;
				textHeightStart = 1.0f;
				textWidthStart = 0.0f + (1369.0f / 2048.0f);

				// Draw the left panel drop down button
				createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, tileWindowVerteces);
				tileWindowElements.push_back(size_t(1));
			}

			if (!changeArticle) {
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (471.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (379.0f / 540.0f);
				width = (41.0f / 960.0f);
				height = (28.0f / 540.0f);
				textWidth = (41.0f / 2048.0f);
				textHeight = 28.0f / 2048.0f;
				textHeightStart = 1.0f;
				textWidthStart = 0.0f + (1881.0f / 2048.0f);

				// Draw the left panel drop down button
				createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, tileWindowVerteces);
				tileWindowElements.push_back(size_t(1));

			}
			else if (changeArticle) {
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (471.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (379.0f / 540.0f);
				width = (41.0f / 960.0f);
				height = (28.0f / 540.0f);
				textWidth = (40.0f / 2048.0f);
				textHeight = 28.0f / 2048.0f;
				textHeightStart = 1.0f;
				textWidthStart = 0.0f + (1840.0f / 2048.0f);

				// Draw the left panel drop down button
				createSquare(widthStart, heightStart, width, height, textWidthStart, textHeightStart, textWidth, textHeight, tileWindowVerteces);
				tileWindowElements.push_back(size_t(1));
			}

			{
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (464.0f / 960.0f);;
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (62.0f / 540.0f);;
				width = (64.0f / 960);
				height = (64.0f / 540);
				textWidth = (64.0f / 2048.0f);
				textHeight = (64.0f / 2048.0f);
				textHeightStart = 1.0f;;
				textWidthStart = 0.0f;


				size_t itemID;
				if (selectedItem == -1) {
					itemID = tileInfo->getID();
				}
				else
					itemID = tileInfo->getAllItems().at(selectedItem).getID();
				size_t id = itemAtlas.getItemTexturePosition(itemID);
				size_t i = 0, j = 0;
				while (id > 32) {
					id -= 32;
					++j;
				}
				i = id - 1;

				// Draw tile info window
				createSquare(widthStart, heightStart, width, height, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, tileWindowVerteces);
				tileWindowElements.push_back(itemAtlas.getGUITextureLocationAsNbr(itemID));
			}
		}
		v.insert(v.end(), tileWindowVerteces.begin(), tileWindowVerteces.end());
		GUIElements.insert(GUIElements.end(), tileWindowElements.begin(), tileWindowElements.end());
		/*for (auto& verteces : tileWindowVerteces) {
		v.push_back(verteces);
		}*/
		/*for (auto& e : tileWindowElements) {
		GUIElements.push_back(e);
		}*/
	}
	//-----------------------------------------------
	//				DONE TILE INFO WINDOW
	//-----------------------------------------------
}

void ElementCreator::tileInfoWindowLetters(std::vector<float>& v)
{

	if (tileInfoWindow) {
		if (tileWindowUpdate) {
			tileWindowText = 0;
			{//	Draw id
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (316.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (50.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "ID:", tileWindowTextVerteces, true);


				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (85.0f / 540.0f);


				if (selectedItem == -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getID()), tileWindowTextVerteces, true);
				}
				else
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getID()), tileWindowTextVerteces, true);
			}

			{//	Draw Number of items text
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (626.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (20.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Number of items on tile:", tileWindowTextVerteces, true);
			}

			{//	Draw Number of items number
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (626.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (57.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().size()), tileWindowTextVerteces, true);
			}

			{//	Draw current item text
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (626.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (100.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Current item:", tileWindowTextVerteces, true);
			}

			{//	Draw Number of items number
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (626.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (137.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(selectedItem + 1), tileWindowTextVerteces, true);
			}

			{//	Draw block projectile
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (99.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (160.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Block projectile: ", tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (195.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getBlockProjectile()), tileWindowTextVerteces, true);
				}
			}

			{//	Draw block pathfind
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (283.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (160.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Block pathfind:", tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (195.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getBlockPathfind()), tileWindowTextVerteces, true);
				}
				else
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getBlockPathfind()), tileWindowTextVerteces, true);
			}

			{//	Draw block object
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (467.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (160.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Block object: ", tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (195.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getBlockObject()), tileWindowTextVerteces, true);
				}
			}

			{//	Draw moveable
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (651.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (195.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Moveable: ", tileWindowTextVerteces, true);
			}

			{//	Draw Pickupable
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (99.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (232.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Pickupable: ", tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (267.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getPickupable()), tileWindowTextVerteces, true);
				}
			}

			{//	Draw Useable
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (283.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (232.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Useable: ", tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (267.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getUseable()), tileWindowTextVerteces, true);
				}
			}

			{//	Draw Hangeable
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (467.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (232.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Hangeable: ", tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (267.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getHangeable()), tileWindowTextVerteces, true);
				}
			}

			{//	Draw AlwaysOnTop
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (651.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (232.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "AlwaysOnTop: ", tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (267.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getAlwaysOnTop()), tileWindowTextVerteces, true);
				}
			}

			{//	Draw UID
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (146.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (305.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "uID: ", tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (340.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, std::to_string(tileInfo->getAllItems().at(selectedItem).getUID()), tileWindowTextVerteces, true);
				}
			}

			{//	Draw article
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (376.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (305.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Article: " + tileInfo->getArticle(), tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (340.0f / 540.0f);


				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, tileInfo->getAllItems().at(selectedItem).getArticle(), tileWindowTextVerteces, true);
				}
			}

			{//	Draw description
				widthStart = xCameraPos + -1.0f + (710.5f / 960.0f) - (375 / 960.0f) + (606.0f / 960.0f);
				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (305.0f / 540.0f);
				width = 16.0f / 960.0f;
				height = 16.0f / 540.0f;

				tileWindowText += createCharacters(widthStart, heightStart, width, height, "Description: " + tileInfo->getDescription(), tileWindowTextVerteces, true);

				heightStart = yCameraPos + 1.0f - (516.0f / 540.0f) + (300.0f / 540.0f) - (340.0f / 540.0f);

				if (selectedItem > -1) {
					tileWindowText += createCharacters(widthStart, heightStart, width, height, tileInfo->getAllItems().at(selectedItem).getArticle(), tileWindowTextVerteces, true);
				}
			}
			tileWindowUpdate = false;
		}

		v.insert(v.end(), tileWindowTextVerteces.begin(), tileWindowTextVerteces.end());
		GUIElements.insert(GUIElements.end(), tileWindowText, 2);
	}
}

void ElementCreator::GUILetters(std::vector<float>& v)
{
	//-----------------------------------------------
	//				GUI LETTERS
	//-----------------------------------------------

	if(drawLeftPanelGUI) leftHandGUILetters(v);

	bottomBarLetters(v);

	tileInfoWindowLetters(v);
	//-----------------------------------------------
	//				DONE LETTERS
	//-----------------------------------------------
}

void ElementCreator::previewTiles(std::vector<float>& v)
{
	//-----------------------------------------------
	//				PREVIEW TILES
	//-----------------------------------------------
	widthStart = xCameraPos + -1.0f;
	heightStart = yCameraPos + 1.0f;
	width = (imgScale / 960.0f);
	height = (imgScale / 540.0f);
	textWidth = (60 / 2048.0f);
	textHeight = (60 / 2048.0f);
	textHeightStart = 1.0f;
	textWidthStart = 600.0f / 2048.0f;

	createSquare(widthStart + (width * ((1920/64)/2)), heightStart - (height * ((1080 / 64) / 2)), width, height, textWidthStart + (textWidth * 0), textHeightStart + (textHeight * 0), textWidth, textHeight, v);



	if (addItem.size() != 0) {
		widthStart = xCameraPos + -1.0f;
		heightStart = yCameraPos + 1.0f;
		width = (imgScale / 960.0f);
		height = (imgScale / 540.0f);

		size_t i = 0;
		size_t j = 0;

		if (!eraseToggle && !destroyToggle) {
			textWidth = (64 / 2048.0f);
			textHeight = (64 / 2048.0f);
			textHeightStart = 1.0f;
			textWidthStart = 0.0f;

			size_t id = itemAtlas.getItemTexturePosition(preview.getID());
			while (id > 32) {
				id -= 32;
				++j;
			}
			i = id - 1;
		}
		else if (destroyToggle) {
			textWidth = (60 / 2048.0f);
			textHeight = (60 / 2048.0f);
			textHeightStart = 1.0f;
			textWidthStart = 600.0f / 2048.0f;
		}
		else {
			textWidth = (60 / 2048.0f);
			textHeight = (60 / 2048.0f);
			textHeightStart = 1.0f;
			textWidthStart = 540.0f / 2048.0f;
		}

		for (const auto& t : addItem) {

			if (!eraseToggle && !destroyToggle && isDoubleSize(preview.getID())) {

				// Draw a 2x scaled tile
				createSquare(widthStart + (width * (t.second.first - (xCameraPos / (imgScale / (1920 / 2))))) - width, heightStart - (height * (t.second.second + (yCameraPos / (imgScale / (1080 / 2))))) + height, width * 2, height * 2, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, v);
			}
			else {
				// Draw a normal tile
				createSquare(widthStart + (width * (t.second.first - (xCameraPos / (imgScale / (1920 / 2))))), heightStart - (height * (t.second.second + (yCameraPos / (imgScale / (1080 / 2))))), width, height, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, v);
			}
		}
	}


	if (isWithinTileArea && (item.size() != 0 || eraseToggle || destroyToggle)) {
		if (item.size() > 0) preview = tile(x, y, z, item.at(0).getID());
		widthStart = xCameraPos + -1.0f;
		heightStart = yCameraPos + 1.0f;
		width = (imgScale / 960.0f);
		height = (imgScale / 540.0f);

		size_t i = 0;
		size_t j = 0;

		if (!eraseToggle && !destroyToggle) {
			textWidth = (64 / 2048.0f);
			textHeight = (64 / 2048.0f);
			textHeightStart = 1.0f;
			textWidthStart = 0.0f;

			size_t id = itemAtlas.getItemTexturePosition(preview.getID());
			while (id > 32) {
				id -= 32;
				++j;
			}
			i = id - 1;
		}
		else if (destroyToggle) {
			textWidth = (60 / 2048.0f);
			textHeight = (60 / 2048.0f);
			textHeightStart = 1.0f;
			textWidthStart = 600.0f / 2048.0f;
		}
		else {
			textWidth = (60 / 2048.0f);
			textHeight = (60 / 2048.0f);
			textHeightStart = 1.0f;
			textWidthStart = 540.0f / 2048.0f;
		}


		if (brush == 0) {
			if (!eraseToggle && !destroyToggle && isDoubleSize(preview.getID())) {

				// Draw a 2x scaled tile
				createSquare(widthStart + (width * (x - (xCameraPos / (imgScale / (1920 / 2))))) - width, heightStart - (height *  (y + (yCameraPos / (imgScale / (1080 / 2))))) + height, width * 2, height * 2, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, v);
			}
			else {
				// Draw a normal tile
				createSquare(widthStart + (width * (x - (xCameraPos / (imgScale / (1920 / 2))))), heightStart - (height *  (y + (yCameraPos / (imgScale / (1080 / 2))))), width, height, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, v);
			}
		}
		else
			for (int bV = -int(brush); bV <= int(brush); ++bV) {
				for (int bH = -int(brush); bH <= int(brush); ++bH) {
					if (!eraseToggle && !destroyToggle && isDoubleSize(preview.getID())) {

						// Draw a 2x scaled tile
						createSquare(widthStart + (width * (x - (xCoord / 64.0f) + bH)) - width, heightStart - (height * (y - (yCoord / 64.0f) + bV)) + height, width * 2, height * 2, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, v);
					}
					else {
						// Draw a normal tile
						createSquare(widthStart - (64 / 960.0f) + (width), heightStart - (height * (y - (yCoord / 64.0f))), width, height, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, v);
					}
				}
			}
	}

	//-----------------------------------------------
	//				DONE PREVIEW TILES
	//-----------------------------------------------
}

