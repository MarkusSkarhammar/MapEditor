#include "Inputs.h"
#include <thread>
#include "Serializer.h"



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {

			if ((xPos >= screenWidth - 26 && xPos <= screenWidth - 10) && (yPos >= 33 && yPos <= 50)) {
				if (!clickPaletteDropDown) {
					clickPaletteDropDown = true;
					leftPanelState = 2;
					generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
				}
				else {
					clickPaletteDropDown = false;
					leftPanelState = 1;
					generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
				}
			}
			else if (clickPaletteDropDown && (xPos >= screenWidth - 267 && xPos <= screenWidth - 29) && (yPos >= 54 && yPos <= startDropDown + SIZE_DROP_DOWN_TEXT * 36)) {
				size_t item = (yPos - startDropDown) / 36;
				paletteID = item;
				leftPanelState = 0;
				generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
				generate_GUI_Left_Panel_Text_(getObjectByName(objects, "GUI_LeftPanel_Text_"), VertecesHandler::findByName(verteces, "Letters_"));
				clickPaletteDropDown = false;
				generate_GUI_Left_Panel_Tiles(getObjectByName(objects, "GUI_LeftPanel_Tiles_"));
				selectedItemId = std::pair<std::pair<int, bool>, std::pair<int, int>>(std::pair<int, bool>(-1, false), std::pair<int, int>(-1, -1));
				palettePage = 0;
				if (palettes.at(paletteID).getPalette().size() > 0) {
					paletteMaxPage = palettes.at(paletteID).getMaxPage(paletteMaxY);
				}else
					paletteMaxPage = 0;
			}
			else if (!clickPaletteDropDown && (xPos >= screenWidth - 267 && xPos <= screenWidth - 10) && (yPos >= startDropDown + 2 && yPos <= screenHeight - 30)) {
				size_t x = (xPos - (screenWidth - 267)) / 64;
				size_t y = (yPos - (startDropDown)) / 64;
				generate_GUI_Left_Panel_Selector(getObjectByName(objects, "GUI_LeftPanel_select_"), x, y, true);
				if ((xPos >= screenWidth - 188 && xPos <= screenWidth - 170) && (yPos >= startDropDown + 2 + (paletteMaxY * 64) + 7 && yPos <= screenHeight - 35)) {
					paletteLeftPressed = true;
					generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
					if (palettePage > 0) { 
						palettePage--; 
						generate_GUI_Left_Panel_Tiles(getObjectByName(objects, "GUI_LeftPanel_Tiles_"));
					}
				}
				else if ((xPos >= screenWidth - 103 && xPos <= screenWidth - 85) && (yPos >= startDropDown + 2 + (paletteMaxY * 64) + 7 && yPos <= screenHeight - 35)) {
					paletteRightPressed = true;
					generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
					if (palettePage < paletteMaxPage) {
						palettePage++;
						generate_GUI_Left_Panel_Tiles(getObjectByName(objects, "GUI_LeftPanel_Tiles_"));
					}
				}
			}
			else if ((xPos >= 0.0 && xPos <= screenWidth - 276) && (yPos >= 0.0 && yPos <= screenHeight - 30)) {
				if (lControl && !itemInfoWindow) {
					itemInfoTile = world.getFloor(z).getTile((x / 50 + (y / 50) * 40), x, y);
					if (itemInfoTile != nullptr) {
						itemInfoWindow = true;
						itemInfoCurrentPage = 0;
						itemInfoMaxPage = itemInfoTile->getAllItems().size();
						generate_GUI_Item_Info_Panel(getObjectByName(objects, "GUI_Item_Info_Panel_"), VertecesHandler::findByName(verteces, "GUI_1"));
					}
				}
				else if (!itemInfoWindow && (selectedItemId.first.first != -1 || eraseToggle || destroyToggle || destroyTileToggle || copyToggle || cutToggle) && thingsToDraw.size() == 0) {
					if (cutToggle && copyBuffer.size() > 0) {
						thingsToDraw.clear();
						thingsToDraw.insert(ToDraw(-1 ,-1, -1, -1, -1));
					}
					else {
						int tempID(0);
						if (eraseToggle)
							tempID = -1;
						else if (destroyToggle)
							tempID = -2;
						else if (destroyTileToggle)
							tempID = -3;
						else if (cutToggle)
							tempID = -4;
						else if (copyToggle)
							tempID = -5;
						else
							tempID = selectedItemId.first.first;
						for (int yPos = y - brush; yPos <= y + brush; yPos++) {
							for (int xPos = x - brush; xPos <= x + brush; xPos++) {
								thingsToDraw.insert(ToDraw(tempID, xPos, yPos, z, (xPos / 50 + (yPos / 50) * 40)));
							}
						}
					}
				}
				else if (itemInfoWindow) {
					itemInfo.checkClicked(getObjectByName(objects, "GUI_Item_Info_Panel_"), xPos, yPos, 1);
				}
			}
			else if (bottomBarShow) {
				bottomBar.checkClicked(getObjectByName(objects, "GUI_BottomBar_"), xPos, yPos, 1);
			}

			lbutton_down = true;
		}else if (GLFW_RELEASE == action) {
			lbutton_down = false;
			if (itemInfoWindow) {
				itemInfo.checkClicked(getObjectByName(objects, "GUI_Item_Info_Panel_"), xPos, yPos, 0);
				if (updateItemInfo) {
					updateItemInfo = false;
					generate_GUI_Item_Info_Panel(getObjectByName(objects, "GUI_Item_Info_Panel_"), VertecesHandler::findByName(verteces, "GUI_1"));
				}
			}
			if (paletteLeftPressed) paletteLeftPressed = false;
			if (paletteRightPressed) paletteRightPressed = false;
			if (descriptionButtonPressed) {
				descriptionButtonPressed = false;
				generate_GUI_Item_Info_Panel(getObjectByName(objects, "GUI_Item_Info_Panel_"), VertecesHandler::findByName(verteces, "GUI_1"));
			}
		}

	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (clickPaletteDropDown) {
			clickPaletteDropDown = false;
			generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), 0);
		}
		if (selectedItemId.first.first != -1) {
			thingsToDraw.clear();
			selectedItemId = std::pair<std::pair<int, bool>, std::pair<int, int>>(std::pair<int, bool>(-1, false), std::pair<int, int>(-1, -1));
			getObjectByName(objects, "GUI_LeftPanel_select_").clearObjects();
		}
		if (ToggleButtonGroup* check = dynamic_cast<ToggleButtonGroup*>(bottomBar.getElementByName("toggles"))) {
			check->resetAll();
			thingsToDraw.clear();
			getObjectByName(objects, "GUI_Preview_Tiles_").clearObjects();
		}
		rbutton_down = true;
	}
	else if (GLFW_RELEASE == action)
		rbutton_down = false;

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		zoom = 1.0;
	}

	if (button == GLFW_MOUSE_BUTTON_5 && action == GLFW_PRESS) {
		if (!itemInfoWindow && z < 13) {
			newZ += 1;
			generate_GUI_Bottom_Bar_text(getObjectByName(objects, "GUI_BottomBar_Text_"), VertecesHandler::findByName(verteces, "Letters_"), "X:" + std::to_string(x), "Y:" + std::to_string(y), "Z:" + std::to_string(z));
		}
	}

	if (button == GLFW_MOUSE_BUTTON_4 && action == GLFW_PRESS) {
		if (!itemInfoWindow && z > 0) {
			newZ -= 1;
			generate_GUI_Bottom_Bar_text(getObjectByName(objects, "GUI_BottomBar_Text_"), VertecesHandler::findByName(verteces, "Letters_"), "X:" + std::to_string(x), "Y:" + std::to_string(y), "Z:" + std::to_string(z));
		}
	}
}

void mouse_scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	
	
	if (lControl && yOffset != 0) {
		zoom -= (yOffset / 10);
		if (zoom < 0.2) zoom = 0.2;
		yCameraPos = yCameraPos;
		current_state.yGoal = yCameraPos;
		xCameraPos = xCameraPos;
		current_state.xGoal = xCameraPos;
		//setCameraZoom(zoom / 90);
	}
	else if (!rbutton_down && !isWithinTileArea && yOffset != 0) {
		yCameraPos += yOffset * (imgScale / (screenHeightPixels / 2.0));
		current_state.yGoal = yCameraPos;
		if (yCameraPos > -1.0f) {
			yCameraPos = -1.0f;
			current_state.yGoal = yCameraPos;
		}
	}
	else if (!isWithinTileArea && yOffset != 0) {
		xCameraPos += yOffset * (imgScale / (screenWidthPixels / 2.0));
		current_state.xGoal = xCameraPos;
		if (xCameraPos < 1.0f) {
			xCameraPos = 1.0f;
			current_state.xGoal = xCameraPos;

		}
	}
}

void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT_CONTROL) {
		if (action == GLFW_PRESS)
			lControl = true;
		else if (GLFW_RELEASE == action)
			lControl = false;
	}

	if (key == GLFW_KEY_Z) {
		if (lControl && action == GLFW_PRESS) {
			
		}
	}

	if (key == GLFW_KEY_Y) {
		if (lControl && action == GLFW_PRESS) {
			
		}
	}

	if (key == GLFW_KEY_S) {
		if (lControl && action == GLFW_PRESS) {
			serializer.saveWorld(world);
		}
	}

	/*
	* Loading map hotkey
	*/
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		if (lControl && !updateWorld) {
			updateWorld = true;
			std::thread(&Serialize::loadWorld, serializer, std::ref(world)).detach();
		}
		//else
			//once = false;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		if (lControl) {
			drawMiddle = !drawMiddle;
		}
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		if (lControl) {
			drawGUI = !drawGUI;
		}
		else {
			drawLeftPanelGUI = !drawLeftPanelGUI;
		}
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (lControl) {
		}
	}

	if (key == GLFW_KEY_R) {
		if (action == GLFW_PRESS) {
			for (auto& i : copyBuffer) {
				delete i.second;
			}
			copyBuffer.clear();
		}
	}

	if (key == GLFW_KEY_ESCAPE) {
		if (itemInfoWindow) {
			itemInfoWindow = false;
			generate_GUI_Item_Info_Panel(getObjectByName(objects, "GUI_Item_Info_Panel_"), VertecesHandler::findByName(verteces, "GUI_1"));
		}
	}

	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			if (!itemInfoWindow) {
				moveRight = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveRight = false;
		}
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		if (lControl) {
			//drawTiles = !drawTiles;
			//world.getFloor(7).getSection(0).push_back(tile(0, 0, 0, 1));
			/*for (size_t floor = 0; floor < MAX_FLOORS; floor++) {
				for (size_t section = 0; section < SECTIONS_WIDTH*SECTIONS_WIDTH; section++) {
					for (size_t y = 0 + (section % 40) * 50; y < 50 + (section % 40) * 50; y++) {
						for (size_t x = 0 + section * 50; x < 50 + section * 50; x++) {
							world.getFloor(floor).getSection(section).push_back(tile(x, y, floor, 1));
						}
					}
				}
				printf("floor: %i\n", floor);
			}*/
		}

	}

	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			if (!itemInfoWindow) {
				//updateCameraPosition(-(imgScale / (1920 / 2)), 0);
				moveLeft = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveLeft = false;
		}
	}

	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			if (lControl) {
				drawWalls = !drawWalls;
			}
			else if (!itemInfoWindow) {
				//updateCameraPosition(0, (imgScale / (1080 / 2)));
				moveUp = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveUp = false;
		}
	}

	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			if (!itemInfoWindow) {
				//updateCameraPosition(0, -(imgScale / (1080 / 2)));
				moveDown = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveDown = false;
		}
	}

	if (key == GLFW_KEY_1) {
		if (action == GLFW_PRESS) {
			brush = 0;
		}
	}

	if (key == GLFW_KEY_2) {
		if (action == GLFW_PRESS) {
			brush = 1;
		}
	}

	if (key == GLFW_KEY_3) {
		if (action == GLFW_PRESS) {
			brush = 2;
		}
	}

	if (key == GLFW_KEY_4) {
		if (action == GLFW_PRESS) {
			brush = 3;
		}
	}

	if (key == GLFW_KEY_5) {
		if (action == GLFW_PRESS) {
			brush = 4;
		}
	}

}

void handelHover() {

	if ( (xPos >= 0.0 && xPos <= screenWidth - 276) && (yPos >= 0.0 && yPos <= screenHeight - 30) ) {
		x = /*(xCoord / imgScale)*/ ((xCameraPos - 1.0f) / (imgScale / (screenWidth / 2))) + (xPos / imgScale);
		y = -((yCameraPos + 1.f) / (imgScale / (screenHeight / 2))) + (yPos / imgScale);
		if (itemInfoWindow) {
			itemInfo.checkHover(getObjectByName(objects, "GUI_Item_Info_Panel_"), xPos, yPos);
		}
		else {
			generate_GUI_Bottom_Bar_text(getObjectByName(objects, "GUI_BottomBar_Text_"), VertecesHandler::findByName(verteces, "Letters_"), "X:" + std::to_string(x), "Y:" + std::to_string(y), "Z:" + std::to_string(z));
			if (eraseToggle || destroyToggle || destroyTileToggle || copyToggle || cutToggle || selectedItemId.first.first != -1) {
				if (lbutton_down) {
					int tempID(0);
					if (eraseToggle)
						tempID = -1;
					else if (destroyToggle)
						tempID = -2;
					else
						tempID = selectedItemId.first.first;
					for (int yPos = y - brush; yPos <= y + brush; yPos++) {
						for (int xPos = x - brush; xPos <= x + brush; xPos++) {
							thingsToDraw.insert(ToDraw(tempID, xPos, yPos, z, (xPos / 50 + (yPos / 50) * 40)));
						}
					}
				}
				generate_GUI_Preview_Tiles(getObjectByName(objects, "GUI_Preview_Tiles_"));
			}
		}
	}
	else if (!clickPaletteDropDown && !hoveringPaletteDropDown && (xPos >= screenWidth - 26 && xPos <= screenWidth - 10) && (yPos >= 33 && yPos <= 50)) {
		leftPanelState = 1;
		generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
		hoveringPaletteDropDown = true;
	}
	else if (!clickPaletteDropDown && hoveringPaletteDropDown && !((xPos >= screenWidth - 26 && xPos <= screenWidth - 10) && (yPos >= 33 && yPos <= 50))) {
		leftPanelState = 0;
		generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
		hoveringPaletteDropDown = false;
	}else if (clickPaletteDropDown && ((xPos >= screenWidth - 267 && xPos <= screenWidth - 29) && (yPos >= 54 && yPos <= startDropDown + palettes.size() * 36))) {
		size_t item = (yPos - startDropDown) / 36;
		generate_GUI_Left_Panel_DropDown(getObjectByName(objects, "GUI_LeftPanel_DropDown_"), VertecesHandler::findByName(verteces, "GUI_1"), item);
	}
	else if (!clickPaletteDropDown && (xPos >= screenWidth - 267 && xPos <= screenWidth - 10) && (yPos >= startDropDown + 2 && yPos <= screenHeight - 30)) {
		if(!isWithinTileArea) isWithinTileArea = true;
		size_t x = (xPos - (screenWidth - 267)) / 64;
		size_t y = (yPos - (startDropDown)) / 64;
		generate_GUI_Left_Panel_Selector(getObjectByName(objects, "GUI_LeftPanel_select_"), x, y, false);
		if ((xPos >= screenWidth - 188 && xPos <= screenWidth - 170) && (yPos >= startDropDown + 2 + (paletteMaxY * 64) + 7 && yPos <= screenHeight - 35)) {
			paletteLeftHover = true;
			generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
		}
		else if ((xPos >= screenWidth - 103 && xPos <= screenWidth - 85) && (yPos >= startDropDown + 2 + (paletteMaxY * 64) + 7 && yPos <= screenHeight - 35)) {
			paletteRightHover = true;
			generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
		}
	}if (bottomBarShow) {
		bottomBar.checkHover(getObjectByName(objects, "GUI_BottomBar_"), xPos, yPos);
	}

	if (isWithinTileArea && !(xPos >= screenWidth - 267 && xPos <= screenWidth - 10) && (yPos >= startDropDown + 2 && yPos <= screenHeight - 30)) {
		isWithinTileArea = false;
		generate_GUI_Left_Panel_Selector(getObjectByName(objects, "GUI_LeftPanel_select_"), -1, -1, false);
	}
	if ( !( (xPos >= screenWidth - 188 && xPos <= screenWidth - 170) && (yPos >= startDropDown + 2 + (paletteMaxY * 64) + 7 && yPos <= screenHeight - 35) ) ) {
		paletteLeftHover = false;
		paletteLeftPressed = false;
		generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
	}
	if ( !( (xPos >= screenWidth - 103 && xPos <= screenWidth - 85) && (yPos >= startDropDown + 2 + (paletteMaxY * 64) + 7 && yPos <= screenHeight - 35) ) ) {
		paletteRightHover = false;
		paletteRightPressed = false;
		generate_GUI_Left_Panel(getObjectByName(objects, "GUI_LeftPanel_"), VertecesHandler::findByName(verteces, "GUI_1"), leftPanelState);
	}
}