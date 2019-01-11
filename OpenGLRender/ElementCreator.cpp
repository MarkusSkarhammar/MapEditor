#include "ElementCreator.h"


void generate_GUI_Bottom_Bar(Objects & ob, VertecesHandler vh)
{

	bottomBar.setDimensions(0, screenHeightPixels - 30, screenWidth, 30);
	bottomBar.setCheckIfOutside(true);
	bottomBar.clear();

	bottomBar.addElement(new GUIElement("leftCorner", vh.getVAO(), vh.getTextureID(), bottomBarBasic, 0, screenHeightPixels - 30, 64, 30, ""));
	//X
	bottomBar.addElement(new GUIElement("x", vh.getVAO(), vh.getTextureID(), bottomBarLabel, 64, screenHeightPixels - 30, 64, 30, ""));
	//Y
	bottomBar.addElement(new GUIElement("y", vh.getVAO(), vh.getTextureID(), bottomBarLabel, 64 * 2, screenHeightPixels - 30, 64, 30, ""));
	//Z
	bottomBar.addElement(new GUIElement("z", vh.getVAO(), vh.getTextureID(), bottomBarLabel, 64 * 3, screenHeightPixels - 30, 64, 30, ""));
	// Create the middle blank parts
	size_t sections = screenWidthPixels / 64 - 4;
	for (size_t i = 0; i < sections; i++) {
		bottomBar.addElement(new GUIElement("leftCorner", vh.getVAO(), vh.getTextureID(), bottomBarBasic, 64 * (i + 4), screenHeightPixels - 30, 64, 30, ""));
	}
	ToggleButtonGroup* group = new ToggleButtonGroup("toggles");
	group->addElement(new ToggleButton("eraser", vh.getVAO(), vh.getTextureID(), bottomBarEraser, bottomBarEraserHover, bottomBarEraserPressed, 64 * 5, screenHeightPixels - 30, 30, 30, "", [&]() { eraseToggle = !eraseToggle; if(!eraseToggle) getObjectByName(objects, "GUI_Preview_Tiles_").clearObjects(); }));
	group->addElement(new ToggleButton("destroyer", vh.getVAO(), vh.getTextureID(), bottomBarDestroyer, bottomBarDestroyerHover, bottomBarDestroyerPressed, 64 * 5 + 40, screenHeightPixels - 30, 30, 30, "", [&]() { destroyToggle = !destroyToggle; if (!destroyToggle) getObjectByName(objects, "GUI_Preview_Tiles_").clearObjects(); }));
	group->addElement(new ToggleButton("tileDestroyer", vh.getVAO(), vh.getTextureID(), bottomBarTileDestroyer, bottomBarTileDestroyerHover, bottomBarTileDestroyerPressed, 64 * 5 + 80, screenHeightPixels - 30, 30, 30, "", [&]() {destroyTileToggle = !destroyTileToggle; if (!destroyTileToggle) getObjectByName(objects, "GUI_Preview_Tiles_").clearObjects(); }));
	group->addElement(new ToggleButton("cut", vh.getVAO(), vh.getTextureID(), bottomBarCut, bottomBarCutHover, bottomBarCutPressed, 64 * 5 + 120, screenHeightPixels - 30, 30, 30, "", [&]() {cutToggle = !cutToggle; if (!cutToggle) getObjectByName(objects, "GUI_Preview_Tiles_").clearObjects(); }));
	group->addElement(new ToggleButton("copy", vh.getVAO(), vh.getTextureID(), bottomBarCopy, bottomBarCopyHover, bottomBarCopyPressed, 64 * 5 + 160, screenHeightPixels - 30, 30, 30, "", [&]() {copyToggle = !copyToggle; if (!copyToggle) getObjectByName(objects, "GUI_Preview_Tiles_").clearObjects(); }));
	bottomBar.addElement(group);

	bottomBar.createObjects(ob);
}

void generate_GUI_Bottom_Bar_text(Objects & ob, VertecesHandler vh, std::string xText, std::string yText, std::string zText)
{
	ob.clearObjects();
	// X
	double x = ((64 * 2 + 16) / (double(screenWidthPixels) / 2)), y = 2.0 + ((30 + 11) / (double(screenHeightPixels) / 2));
	generate_GUI_Text(ob, vh, x, y, xText);
	// Y
	x = ((64 * 3 + 16) / (double(screenWidthPixels) / 2)), y = 2.0 + ((30 + 11) / (double(screenHeightPixels) / 2));
	generate_GUI_Text(ob, vh, x, y, yText);
	// Z
	x = ((64 * 4 + 16) / (double(screenWidthPixels) / 2)), y = 2.0 + ((30 + 11) / (double(screenHeightPixels) / 2));
	generate_GUI_Text(ob, vh, x, y, zText);
}

void generate_GUI_Text(Objects& ob, VertecesHandler& vh, double x, double y, std::string text)
{
	int character = 0;
	float offset = 0.0f;
	for (auto& letter : text) {
		character = letter;
		ob.addObject(new Object(x + offset, y, character, vh.getVAO(), vh.getTextureID()));
		offset += ((OFFSET_PER_CHARACTER[character]) / (double(screenWidthPixels) / 2));
	}
}

void generate_GUI_Text(Objects& ob, VertecesHandler& vh, double x, double y, std::string text, int rowLength)
{
	int character = 0;
	float offset = 0.0f, maxOffset = (rowLength / (double(screenWidthPixels) / 2));
	for (auto& letter : text) {
		character = letter;
		ob.addObject(new Object(x + offset, y, character, vh.getVAO(), vh.getTextureID()));
		offset += ((OFFSET_PER_CHARACTER[character]) / (double(screenWidthPixels) / 2));
		if (offset > maxOffset) {
			if(character != 32) ob.addObject(new Object(x + offset, y, 45, vh.getVAO(), vh.getTextureID()));
			offset = 0.0f;
			y += (26 / (double(screenWidthPixels) / 2));
		}
	}
}

void generate_GUI_Left_Panel(Objects & ob, VertecesHandler vh, size_t displayState)
{
	ob.clearObjects();
	size_t sections = (screenHeightPixels / 90) - 2;
	size_t left = 90-40-30;
	if (screenHeightPixels % 90 != 0)
		left += screenHeightPixels % 90 - 90;
	for (size_t i = 0; i < left; i++) {
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + ((1*i) / (double(screenHeightPixels) / 2)), 2, vh.getVAO(), vh.getTextureID()));
	}

	// Create the display bar for tile palette selection area
	if (displayState == 0)
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + (left / (double(screenHeightPixels) / 2)), 3, vh.getVAO(), vh.getTextureID()));
	else if (displayState == 1)
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + (left / (double(screenHeightPixels) / 2)), 4, vh.getVAO(), vh.getTextureID()));
	else
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + (left / (double(screenHeightPixels) / 2)), 5, vh.getVAO(), vh.getTextureID()));

	// Create top tile palette selection area
	ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + ((left + 40) / (double(screenHeightPixels) / 2)), 6, vh.getVAO(), vh.getTextureID()));

	// Create middle tile palette selection area
	for (size_t i = 0; i < sections; i++) {
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + ((left + 40 + 90*i) / (double(screenHeightPixels) / 2)), 7, vh.getVAO(), vh.getTextureID()));
	}

	// Create bottom tile palette selection area
	if (paletteLeftPressed)
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + ((left + 40 + 90 * (sections)) / (double(screenHeightPixels) / 2)), 26, vh.getVAO(), vh.getTextureID()));
	else if (paletteRightPressed)
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + ((left + 40 + 90 * (sections)) / (double(screenHeightPixels) / 2)), 27, vh.getVAO(), vh.getTextureID()));
	else if (paletteLeftHover)
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + ((left + 40 + 90 * (sections)) / (double(screenHeightPixels) / 2)), 24, vh.getVAO(), vh.getTextureID()));
	else if (paletteRightHover)
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + ((left + 40 + 90 * (sections)) / (double(screenHeightPixels) / 2)), 25, vh.getVAO(), vh.getTextureID()));
	else
		ob.addObject(new Object(2 - (276 / (double(screenWidthPixels) / 2)), 0.0 + ((left + 40 + 90 * (sections)) / (double(screenHeightPixels) / 2)), 8, vh.getVAO(), vh.getTextureID()));

	vh = VertecesHandler::findByName(verteces, "Letters_");

	double x = ((screenWidth - 100) / (double(screenWidthPixels) / 2)), y = 2.0 + ((14) / (double(screenHeightPixels) / 2));
	generate_GUI_Text(ob, vh, x, y, "" + std::to_string(palettePage));

	x = ((screenWidth - 60) / (double(screenWidthPixels) / 2)), y = 2.0 + ((14) / (double(screenHeightPixels) / 2));
	generate_GUI_Text(ob, vh, x, y, "" + std::to_string(paletteMaxPage));
	
}

void generate_GUI_Left_Panel_Text_(Objects & ob, VertecesHandler vh) {
	// Create the text for tile palette selection area
	if (paletteID != -1) {
		ob.clearObjects();
		double x = ((screenWidthPixels - 125) / (double(screenWidthPixels) / 2)), y = 0.0 + ((startDropDown + 75 + 36 * -1) / (double(screenHeightPixels) / 2));
		generate_GUI_Text(ob, vh, x, y, palettes.at(paletteID).getName());
	}
}

void generate_GUI_Left_Panel_DropDown(Objects & ob, VertecesHandler vh, int hover)
{
	ob.clearObjects();
	size_t left = 90 - 40 - 30;
	if (screenHeightPixels % 90 != 0)
		left += screenHeightPixels % 90 - 90;
	startDropDown = left + 38;
	// Create the bars for each item
	for (size_t i = 0; i < palettes.size() -1; i++) {
		ob.addObject(new Object(2 - (267 / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 36 * i) / (double(screenHeightPixels) / 2)), 9, vh.getVAO(), vh.getTextureID()));
	}
	// Create the end of the drop down
	ob.addObject(new Object(2 - (267 / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 36 * (palettes.size() -1)) / (double(screenHeightPixels) / 2)), 10, vh.getVAO(), vh.getTextureID()));

	// Create hover
	if(hover != -1)
		ob.addObject(new Object(2 - (265 / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 36 * hover) / (double(screenHeightPixels) / 2)), 11, vh.getVAO(), vh.getTextureID()));
}

void generate_GUI_Left_Panel_DropDown_Text(Objects & ob, VertecesHandler vh)
{
	// text for drop down menu
	for (size_t i = 0; i < palettes.size(); i++) {
		double x = ((screenWidthPixels-125) / (double(screenWidthPixels) / 2)), y = 0.0 + ((startDropDown + 75 + 36 * i) / (double(screenHeightPixels) / 2));
		generate_GUI_Text(ob, vh, x, y, palettes.at(i).getName());
	}

}

void generate_GUI_Left_Panel_Tiles(Objects & ob)
{
	ob.clearObjects();
	VertecesHandler vh("Nothing");
	size_t increment = 64;
	if (paletteID != -1) {
		for (auto& item : palettes.at(paletteID).getPalette()) {
			if (item.second.second / paletteMaxY == palettePage) {
				if (item.first.first / 1024 == 0 && vh.getName().compare("Tiles_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Tiles_1024");
				}
				else if (item.first.first / 1024 == 1 && vh.getName().compare("Items_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Items_1024");
				}
				else if (item.first.first / 1024 == 2 && vh.getName().compare("Doodads_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Doodads_1024");
				}
				else if (item.first.first / 1024 == 3 && vh.getName().compare("Borders_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Borders_1024");
				}
				else if (item.first.first / 1024 == 5 && vh.getName().compare("Nature_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Nature_1024");
				}
				if (itemAtlas.checkIfAnimation(item.first.first)) {
					AnimationObject* a = itemAtlas.getAnimationObject( ( (screenWidthPixels - 266 + increment * (item.second.first + item.first.second)) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * (item.second.second % paletteMaxY + item.first.second)) / (double(screenHeightPixels) / 2)), item.first.first, vh.getVAO(), vh.getTextureID(), itemAtlas.getItem(item.first.first));
					a->setID(a->getID() % 1024);
					ob.addObject(a);
				}
				else
				ob.addObject(new Object(((screenWidthPixels - 266 + increment * (item.second.first + item.first.second)) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * (item.second.second % paletteMaxY + item.first.second)) / (double(screenHeightPixels) / 2)), item.first.first % 1024, vh.getVAO(), vh.getTextureID()));
			}
		}
	}
}

void generate_GUI_Left_Panel_Selector(Objects & ob, int x, int y, bool clicked)
{
	if (paletteID != -1) {
		VertecesHandler vh = VertecesHandler::findByName(verteces, "GUI_1");
		size_t increment = 64;
		ob.clearObjects();
		if (x != -1 && y != -1) {
			auto& p = palettes.at(paletteID).getPalette();
			auto it = std::find_if(p.begin(), p.end(), [x, y](std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>& p) { return (p.second.first == x && p.second.second == y); });
			if (it != p.end()) {
				if (it->first.second)
					ob.addObject(new Object(((screenWidthPixels - 266 + increment * x) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * y) / (double(screenHeightPixels) / 2)), 14, vh.getVAO(), vh.getTextureID()));
				else
					ob.addObject(new Object(((screenWidthPixels - 266 + increment * x) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * y) / (double(screenHeightPixels) / 2)), 12, vh.getVAO(), vh.getTextureID()));
				if (clicked) {
					selectedItemId = *it;
				}
			}
			else {
				size_t tempX = x, tempY = y - 1;
				it = std::find_if(p.begin(), p.end(), [tempX, tempY](std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>& p) { return (p.second.first == tempX && p.second.second == tempY); });
				if (it != p.end()) {
					if (it->first.second) {
						ob.addObject(new Object(((screenWidthPixels - 266 + increment * tempX) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * tempY) / (double(screenHeightPixels) / 2)), 14, vh.getVAO(), vh.getTextureID()));
						if (clicked) {
							selectedItemId = *it;
							selectedItemId.second.first = it->second.first;
							selectedItemId.second.second = it->second.second;
						}
					}
				}
				else {
					tempX = x - 1, tempY = y;
					it = std::find_if(p.begin(), p.end(), [tempX, tempY](std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>& p) { return (p.second.first == tempX && p.second.second == tempY); });
					if (it != p.end()) {
						if (it->first.second) {
							ob.addObject(new Object(((screenWidthPixels - 266 + increment * tempX) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * tempY) / (double(screenHeightPixels) / 2)), 14, vh.getVAO(), vh.getTextureID()));
							if (clicked) {
								selectedItemId = *it;
								selectedItemId.second.first = it->second.first;
								selectedItemId.second.second = it->second.second;
							}
						}
					}
					else {
						tempX = x - 1, tempY = y - 1;
						it = std::find_if(p.begin(), p.end(), [tempX, tempY](std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>& p) { return (p.second.first == tempX && p.second.second == tempY); });
						if (it != p.end()) {
							if (it->first.second) {
								ob.addObject(new Object(((screenWidthPixels - 266 + increment * tempX) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * tempY) / (double(screenHeightPixels) / 2)), 14, vh.getVAO(), vh.getTextureID()));
								if (clicked) {
									selectedItemId = *it;
									selectedItemId.second.first = it->second.first;
									selectedItemId.second.second = it->second.second;
								}
							}
						}
					}
				}
			}
		}

		if (selectedItemId.first.first != -1) {
			if (selectedItemId.first.second)
				ob.addObject(new Object(((screenWidthPixels - 266 + increment * selectedItemId.second.first) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * selectedItemId.second.second) / (double(screenHeightPixels) / 2)), 15, vh.getVAO(), vh.getTextureID()));
			else
				ob.addObject(new Object(((screenWidthPixels - 266 + increment * selectedItemId.second.first) / (double(screenWidthPixels) / 2)), 0.0 + ((startDropDown + 2 + increment * selectedItemId.second.second) / (double(screenHeightPixels) / 2)), 13, vh.getVAO(), vh.getTextureID()));
		}
	}
}

void generate_GUI_Preview_Tiles(Objects & ob)
{
	ob.clearObjects();
	VertecesHandler vh("Nothing");
	size_t increment = 64;
	int id(0);

	for (auto& item : thingsToDraw) {
		if (item.getId() < 0) {
			if(vh.getName().compare("GUI_1") != 0) vh = VertecesHandler::findByName(verteces, "GUI_1");
			id = 23;
			if (eraseToggle) id = 22;
			else if (destroyTileToggle) id = destroyTileToggleID;
			else if (copyToggle) id = copyToggleID;
			else if (cutToggle) id = cutToggleID;
			ob.addObject(new Object(((increment * item.getX()) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * item.getY()) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), id, vh.getVAO(), vh.getTextureID()));
		}
		else {
			if (item.getId() / 1024 == 0 && vh.getName().compare("Tiles_1024") != 0) {
				vh = VertecesHandler::findByName(verteces, "Tiles_1024");
			}
			else if (item.getId() / 1024 == 1 && vh.getName().compare("Items_1024") != 0) {
				vh = VertecesHandler::findByName(verteces, "Items_1024");
			}
			else if (item.getId() / 1024 == 2 && vh.getName().compare("Doodads_1024") != 0) {
				vh = VertecesHandler::findByName(verteces, "Doodads_1024");
			}
			else if (item.getId() / 1024 == 3 && vh.getName().compare("Borders_1024") != 0) {
				vh = VertecesHandler::findByName(verteces, "Borders_1024");
			}
			else if (item.getId() / 1024 == 5 && vh.getName().compare("Nature_1024") != 0) {
				vh = VertecesHandler::findByName(verteces, "Nature_1024");
			}
			ob.addObject(new Object(((increment * item.getX()) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * item.getY()) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), item.getId() % 1024, vh.getVAO(), vh.getTextureID()));
		}
	}
	if (eraseToggle || destroyToggle || destroyTileToggle || copyToggle || cutToggle) {
		vh = VertecesHandler::findByName(verteces, "GUI_1");
		id = 23;
		if (eraseToggle) id = 22;
		else if (destroyTileToggle) id = destroyTileToggleID;
		else if (copyToggle) id = copyToggleID;
		else if (cutToggle) id = cutToggleID;
	}
	else {
		id = selectedItemId.first.first % 1024;

		if (selectedItemId.first.first / 1024 == 0 && vh.getName().compare("Tiles_1024") != 0) {
			vh = VertecesHandler::findByName(verteces, "Tiles_1024");
		}
		else if (selectedItemId.first.first / 1024 == 1 && vh.getName().compare("Items_1024") != 0) {
			vh = VertecesHandler::findByName(verteces, "Items_1024");
		}
		else if (selectedItemId.first.first / 1024 == 2 && vh.getName().compare("Doodads_1024") != 0) {
			vh = VertecesHandler::findByName(verteces, "Doodads_1024");
		}
		else if (selectedItemId.first.first / 1024 == 3 && vh.getName().compare("Borders_1024") != 0) {
			vh = VertecesHandler::findByName(verteces, "Borders_1024");
		}
		else if (selectedItemId.first.first / 1024 == 5 && vh.getName().compare("Nature_1024") != 0) {
			vh = VertecesHandler::findByName(verteces, "Nature_1024");
		}
	}
	if (copyBuffer.size() == 0) {
		for (int yPos = y - brush; yPos <= y + brush; yPos++) {
			for (int xPos = x - brush; xPos <= x + brush; xPos++) {
				ob.addObject(new Object(((increment * xPos) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * yPos) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), id, vh.getVAO(), vh.getTextureID()));
			}
		}
	}
	else if(!copyBufferLock){
		int smallestX = -1, differenceX = -1;
		int smallestY = -1, differenceY = -1;
		if (copyBuffer.size() > 0) {
			smallestX = std::min_element(copyBuffer.begin(), copyBuffer.end(), [](std::pair<ToDraw, tile*>& lhs, std::pair<ToDraw, tile*>& rhs) { return (lhs.first.getX() < rhs.first.getX()); })->first.getX();
			differenceX = std::max_element(copyBuffer.begin(), copyBuffer.end(), [](std::pair<ToDraw, tile*>& lhs, std::pair<ToDraw, tile*>& rhs) { return (lhs.first.getX() < rhs.first.getX()); })->first.getX() - smallestX;
			smallestY = std::min_element(copyBuffer.begin(), copyBuffer.end(), [](std::pair<ToDraw, tile*>& lhs, std::pair<ToDraw, tile*>& rhs) { return (lhs.first.getY() < rhs.first.getY()); })->first.getY();
			differenceY = std::max_element(copyBuffer.begin(), copyBuffer.end(), [](std::pair<ToDraw, tile*>& lhs, std::pair<ToDraw, tile*>& rhs) { return (lhs.first.getY() < rhs.first.getY()); })->first.getY() - smallestY;
		}
		for (auto& i : copyBuffer) {
			if (i.second != nullptr) {
				id = i.second->getID();
				getVertecesHandlerFromID(vh, id);
				ob.addObject(new Object(((increment * ((i.first.getX() - smallestX - size_t(differenceX / 2)) + x)) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * ((i.first.getY() - smallestY - size_t(differenceY / 2)) + y)) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), id, vh.getVAO(), vh.getTextureID()));
				for (auto& item : i.second->getAllItems()) {
					id = item->getID();
					getVertecesHandlerFromID(vh, id);
					ob.addObject(new Object(((increment * ((i.first.getX() - smallestX - size_t(differenceX / 2)) + x)) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * ((i.first.getY() - smallestY - size_t(differenceY / 2)) + y)) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), id % 1024, vh.getVAO(), vh.getTextureID()));
				}
			}
		}
	}

}

void generate_GUI_Item_Info_Panel(Objects & ob, VertecesHandler vh) {
	ob.clearObjects();
	if (itemInfoWindow) {
		int startX = ((screenWidth - 1000 - 267) / 2), startY = ((screenHeight - 30 - 700) / 2);
		itemInfo.setDimensions(startX, startY, 1000, 700);
		itemInfo.clear();
		bool doubleSize = false;

		//Main Panel
		itemInfo.addElement(new GUIElement("MainPanel", vh.getVAO(), vh.getTextureID(), itemInfoPanelID, startX, startY, 1000, 700, ""));

		if (itemInfoCurrentPage == 0) {
			VertecesHandler vhT;
			getVertecesHandlerFromID(vhT, itemInfoTile->getID());
			// Icon
			itemInfo.addElement(new GUIElement("icon", vhT.getVAO(), vhT.getTextureID(), (itemInfoTile->getID() % 1024), (startX + 468), (startY + 9), 64, 64, ""));
		}
		else {
			VertecesHandler vhT;
			getVertecesHandlerFromID(vhT, itemInfoTile->getAllItems().at(itemInfoCurrentPage - 1)->getID(), doubleSize);
			// Icon
			itemInfo.addElement(new GUIElement("icon", vhT.getVAO(), vhT.getTextureID(), (itemInfoTile->getAllItems().at(itemInfoCurrentPage-1)->getID() % 1024), (startX + 468), (startY + 9), 64, 64, ""));
		}

		// Right arrow
		if (itemInfoCurrentPage != itemInfoMaxPage) {
			auto lambda = ([&]() { 
				if (itemInfoCurrentPage < itemInfoMaxPage) {
					itemInfoSubPage = 0;
					itemInfoCurrentPage++;
					updateItemInfo = true;
				}
			});
			itemInfo.addElement(new Button("RightArrow", vh.getVAO(), vh.getTextureID(), itemInfoRightArrowID, itemInfoRightArrowHoverID, itemInfoRightArrowPressedID, (startX + 547 + (40 * doubleSize)), (startY + 19), 30, 43, "", lambda));
		}

		// Left arrow
		if (itemInfoCurrentPage != 0) {
			auto lambda = ([&]() {
				if (itemInfoCurrentPage > 0) {
					itemInfoSubPage = 0;
					itemInfoCurrentPage--;
					updateItemInfo = true;
				}
			});
			itemInfo.addElement(new Button("LeftArrow", vh.getVAO(), vh.getTextureID(), itemInfoLeftArrowID, itemInfoLeftArrowHoverID, itemInfoLeftArrowPressedID, (startX + 423 - (40 * doubleSize)), (startY + 19), 30, 43, "", lambda));
		}

		if (itemInfoCurrentPage == 0) {
			VertecesHandler vhText = VertecesHandler::findByName(verteces, "Letters_");

			std::string text = "id: " + std::to_string(itemInfoTile->getID());
			// First row text fields
			for (int i = 0; i < 4; i++) {
				if (i == 1) text = "uid: " + std::to_string(itemInfoTile->getUID());
				else if (i == 2) text = "zone: " + std::to_string(itemInfoTile->getZone());
				else if (i == 3) text = "speed: " + std::to_string(itemInfoTile->getSpeed());

				itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + 200), 150, 40, text));
				itemInfo.addElement(new GUIElement("FirstRowTextInput_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + 240), 150, 40, ""));

				if (i == 1) {
					itemInfo.addElement(new Button("FirstRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + 280), 150, 26, "", []() {}));
				}
				else if (i == 2) {
					itemInfo.addElement(new Button("FirstRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + 280), 150, 26, "", []() {}));
				}
				else if (i == 3) {
					itemInfo.addElement(new Button("FirstRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + 280), 150, 26, "", []() {}));
				}

			}

			int id = itemInfoTextAreaID;
			text = "name: " + itemInfoTile->getName();
			// Second row text fields
			for (int i = 0; i < 4; i++) {
				if (i == 1) {
					text = "article: " + itemInfoTile->getArticle();
					id = itemInfoTextSectionID;
				}
				else if (i == 2) { 
					text = "description: " + itemInfoTile->getDescription(); 
					id = itemInfoTextAreaID;
				}
				else if (i == 3) {
					text = "blockPathfinding: " + std::to_string(itemInfoTile->getBlockPathfind());
				}

				itemInfo.addElement(new GUIElement("SecondRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), id, (startX + 50 + 250 * i), (startY + 400), 150, 40, text));

				if (i == 0) {
					itemInfo.addElement(new GUIElement("SecondRowTextInput_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + 510), 150, 40, ""));
					itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + 550), 150, 26, "", []() {}));
				}
				else if (i == 1) {
					itemInfo.addElement(new GUIElement("SecondRowTextInput_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + 440), 150, 40, ""));
					itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + 480), 150, 26, "", []() {}));
				}
				else if (i == 2) {
					itemInfo.addElement(new GUIElement("SecondRowTextInput_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + 510), 150, 40, ""));
					itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + 550), 150, 26, "", []() {}));
				}
				else if (i == 3) {
					itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + 510), 150, 26, "", [&]() { itemInfoTile->setBlockPathfind(!itemInfoTile->getBlockPathfind()); itemInfo.getElementByName("SecondRowText_3")->setText("blockPathfinding: " + std::to_string(itemInfoTile->getBlockPathfind())); }));
				}
				// Text
				generate_GUI_Text(ob, vhText, ((startX + 125 + 250 * i) / (double(screenWidthPixels) / 2)), 0.0 + ((startY + 475) / (double(screenHeightPixels) / 2)), text, 114);
			}
		}
		else {
			auto& item = itemInfoTile->getAllItems().at(itemInfoCurrentPage - 1);
			VertecesHandler vhText = VertecesHandler::findByName(verteces, "Letters_");

			if (itemInfoSubPage == 0) {
				int yFirst = 125, offset = 40;

				std::string text = "id: " + std::to_string(item->getBlockProjectile());
				// First row: booleans
				for (int i = 0; i < 2; i++) {
					if (i == 1) text = "uid: " + std::to_string(item->getBlockPathfind());

					itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

					if (i == 1) {
						itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
						itemInfo.addElement(new Button("FirstRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", []() {}));
					}

				}

				yFirst += offset * 3;
				text = "name: " + item->getName();
				// Second row: text areas
				for (int i = 0; i < 4; i++) {
					if (i == 1) text = "article: " + item->getArticle();
					else if (i == 2) text = "description: " + item->getDescription();
					else if (i == 3) text = "type: " + item->getType();

					itemInfo.addElement(new GUIElement("SecondRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextAreaID, (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

					if (i == 0) {
						itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2.75), 150, 40, ">"));
						itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 3.75), 150, 26, "", [&]() {item->setBlockProjectile(!item->getBlockProjectile()); itemInfo.getElementByName("ThirdRowText_0")->setText("blockProjectile: " + std::to_string(item->getBlockProjectile())); }));
					}
					else if (i == 1) {
						itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2.75), 150, 40, ">"));
						itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 3.75), 150, 26, "", [&]() {item->setBlockPathfind(!item->getBlockPathfind()); itemInfo.getElementByName("ThirdRowText_1")->setText("blockPathfind: " + std::to_string(item->getBlockPathfind())); }));
					}
					else if (i == 2) {
						itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2.75), 150, 40, ">"));
						itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 3.75), 150, 26, "", [&]() {item->setBlockObject(!item->getBlockObject()); itemInfo.getElementByName("ThirdRowText_2")->setText("blockObject: " + std::to_string(item->getBlockObject())); }));
					}

				}
				yFirst += offset * 4.75;
				text = "blockProjectile: " + std::to_string(item->getBlockProjectile());
				// Third row: booleans
				for (int i = 0; i < 4; i++) {
					if (i == 1) text = "blockPathfind: " + std::to_string(item->getBlockPathfind());
					else if (i == 2) text = "blockObject: " + std::to_string(item->getBlockObject());
					else if (i == 3) text = "moveable: " + std::to_string(item->getMoveable());

					itemInfo.addElement(new GUIElement("ThirdRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

					if (i == 0) {
						itemInfo.addElement(new Button("ThirdRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&]() {item->setBlockProjectile(!item->getBlockProjectile()); itemInfo.getElementByName("ThirdRowText_0")->setText("blockProjectile: " + std::to_string(item->getBlockProjectile())); }));
					}
					else if (i == 1) {
						itemInfo.addElement(new Button("ThirdRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&]() {item->setBlockPathfind(!item->getBlockPathfind()); itemInfo.getElementByName("ThirdRowText_1")->setText("blockPathfind: " + std::to_string(item->getBlockPathfind())); }));
					}
					else if (i == 2) {
						itemInfo.addElement(new Button("ThirdRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&]() {item->setBlockObject(!item->getBlockObject()); itemInfo.getElementByName("ThirdRowText_2")->setText("blockObject: " + std::to_string(item->getBlockObject())); }));
					}
					else if (i == 3) {
						itemInfo.addElement(new Button("ThirdRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&]() {item->setMoveable(!item->getMoveable()); itemInfo.getElementByName("ThirdRowText_3")->setText("moveable: " + std::to_string(item->getMoveable())); }));
					}

				}
				yFirst += offset * 2;
				text = "pickupable: " + std::to_string(item->getPickupable());
				// Fourth row: booleans
				for (int i = 0; i < 4; i++) {
					if (i == 1) text = "useable: " + std::to_string(item->getUseable());
					else if (i == 2) text = "hangeable: " + std::to_string(item->getHangeable());
					else if (i == 3) text = "alwaysOnTop: " + std::to_string(item->getAlwaysOnTop());

					itemInfo.addElement(new GUIElement("FourthRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

					if (i == 0) {
						itemInfo.addElement(new Button("FourthRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&]() {item->setPickupable(!item->getPickupable()); itemInfo.getElementByName("FourthRowText_0")->setText("pickupable: " + std::to_string(item->getPickupable())); }));
					}
					else if (i == 1) {
						itemInfo.addElement(new Button("FourthRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&]() {item->setUseable(!item->getUseable()); itemInfo.getElementByName("FourthRowText_1")->setText("useable: " + std::to_string(item->getUseable())); }));
					}
					else if (i == 2) {
						itemInfo.addElement(new Button("FourthRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&]() {item->setHangeable(!item->getHangeable()); itemInfo.getElementByName("FourthRowText_2")->setText("hangeable: " + std::to_string(item->getHangeable())); }));
					}
					else if (i == 3) {
						itemInfo.addElement(new Button("FourthRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&]() {item->setAlwaysOnTop(!item->getAlwaysOnTop()); itemInfo.getElementByName("FourthRowText_3")->setText("alwaysOnTop: " + std::to_string(item->getAlwaysOnTop())); }));
					}
				}
				if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(item)) {
					yFirst += offset * 2;
					auto lambda = ([&]() {
					itemInfoSubPage++;
					updateItemInfo = true;
					});
					itemInfo.addElement(new Button("RightArrow", vh.getVAO(), vh.getTextureID(), itemInfoRightArrowID, itemInfoRightArrowHoverID, itemInfoRightArrowPressedID, (startX + 547), (startY + 650), 30, 43, "", lambda));
				}
			}
			else {
				auto lambda = ([&]() {
					itemInfoSubPage--;
					updateItemInfo = true;
				});
				itemInfo.addElement(new Button("LeftArrow", vh.getVAO(), vh.getTextureID(), itemInfoLeftArrowID, itemInfoLeftArrowHoverID, itemInfoLeftArrowPressedID, (startX + 423 - (40 * doubleSize)), (startY + 650), 30, 43, "", lambda));

				if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(item)) {
					int yFirst = 125, offset = 40;

					std::string text = "weight: " + std::to_string(check->getWeight());
					// First row
					for (int i = 0; i < 3; i++) {
						if (i == 1) text = "stack: " + std::to_string(check->getStack());
						if (i == 2) text = "stackMax: " + std::to_string(check->getStackMax());

						itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

						if (i == 0) {
							itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
							itemInfo.addElement(new Button("FirstRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", []() {}));
						}
						else if (i == 1) {
							itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
							itemInfo.addElement(new Button("FirstRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", []() {}));
						}
						else if (i == 2) {
							itemInfo.addElement(new GUIElement("FirstRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
							itemInfo.addElement(new Button("FirstRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", []() {}));
						}

					}


					if (Armor* check = dynamic_cast<Armor*>(item)) {
						int id = itemInfoTextSectionID;
						yFirst += offset * 3;
						text = "armor: " + std::to_string(check->getArmor());
						// Second row: text areas
						for (int i = 0; i < 2; i++) {
							if (i == 1) { text = "armorType: " + check->getArmorType(); id = itemInfoTextAreaID; }

							itemInfo.addElement(new GUIElement("SecondRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), id, (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

							if (i == 0) {
								itemInfo.addElement(new GUIElement("SecondRowTextSection_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
								itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", []() {}));
							}

						}
					}
					else if (Weapon* check = dynamic_cast<Weapon*>(item)) {
						int id = itemInfoTextSectionID;
						yFirst += offset * 3;
						text = "attack: " + std::to_string(check->getAttack());
						// Second row: text areas
						for (int i = 0; i < 3; i++) {
							if (i == 1) text = "defense: " + std::to_string(check->getDefense());
							else if (i == 2) { 
								text = "WeaponType: " + check->getWeaponType();
								id = itemInfoTextAreaID;
							}

							itemInfo.addElement(new GUIElement("SecondRowText_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), id, (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

							if (i == 0) {
								itemInfo.addElement(new GUIElement("SecondRowTextSection_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
								itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", []() {}));
							}
							else if (i == 1) {
								itemInfo.addElement(new GUIElement("SecondRowTextSection_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoTextSectionID, (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
								itemInfo.addElement(new Button("SecondRowButton_" + std::to_string(i), vh.getVAO(), vh.getTextureID(), itemInfoButtonID, itemInfoButtonHoverID, itemInfoButtonPressedID, (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", []() {}));
							}

						}

					}
				}
			}
			
		}
		itemInfo.createObjects(ob);
	}
}
