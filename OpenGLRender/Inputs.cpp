#include "Inputs.h"
#include <thread>
#include "Serializer.h"
#pragma warning(disable: 4244)



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {

			if ((xPos >= 0.0 && xPos <= screenWidth - 276) && (yPos >= 0.0 && yPos <= screenHeight - 30)) {
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
			// Handle clicks on the bottom bar
			if (bottomBar.getShow()) {
				bottomBar.checkClicked(getObjectByName(objects, "GUI_BottomBar_"), xPos, yPos, 1);
			}
			// Handle clicks on the left panel
			if (leftPanel.getShow()) {
				leftPanel.checkClicked(getObjectByName(objects, "GUI_LeftPanel_"), xPos, yPos, 1);
			}
			// Handle clicks on the platte modifier window
			if (paletteModifier.getShow()) {
				paletteModifier.checkClicked(getObjectByName(objects, "GUI_Palette_Modifier_"), xPos, yPos, 1);
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

			if (leftPanel.getShow()) {
				leftPanel.checkClicked(getObjectByName(objects, "GUI_LeftPanel_"), xPos, yPos, 0);
			}

			// Handle clicks on the platte modifier window
			if (paletteModifier.getShow()) {
				paletteModifier.checkClicked(getObjectByName(objects, "GUI_Palette_Modifier_"), xPos, yPos, 0);
			}
		}

	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (selectedItemId.first.first != -1) {
			thingsToDraw.clear();
			selectedItemId = std::pair<std::pair<int, bool>, std::pair<int, int>>(std::pair<int, bool>(-1, false), std::pair<int, int>(-1, -1));
		}
		if (ToggleButtonGroup* check = dynamic_cast<ToggleButtonGroup*>(bottomBar.getElementByName("toggles"))) {
			check->resetAll();
			thingsToDraw.clear();
			getObjectByName(objects, "GUI_Preview_Tiles_").clearObjects();
		}
		// Reset left panel
		if (leftPanel.getShow()) {
			if (ToggleButtonGroup* tg = dynamic_cast<ToggleButtonGroup*>(leftPanel.getElementByName("tileSelectionToggles"))) {
				tg->resetAll();
			}
			if (DropDown* d = dynamic_cast<DropDown*>(leftPanel.getElementByName("paletteSelection"))) {
				if (d->getShow()) {
					if (ToggleButton* t = dynamic_cast<ToggleButton*>(leftPanel.getElementByName("displayBarToggle"))) {
						t->resetToggle();
					}
				}
			}
		}
		if (paletteModifier.getShow()) {
			paletteModifier.checkClicked(getObjectByName(objects, "GUI_Palette_Modifier_"), xPos, yPos, 2);
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
		}
	}

	if (button == GLFW_MOUSE_BUTTON_4 && action == GLFW_PRESS) {
		if (!itemInfoWindow && z > 0) {
			newZ -= 1;
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
		if (paletteModifier.getShow()) {
			paletteModifier.checkScroll(getObjectByName(objects, "GUI_Palette_Modifier_"), xPos, yPos, yOffset);
		}
		else {
			yCameraPos += yOffset * (imgScale / (screenHeightPixels / 2.0));
			current_state.yGoal = yCameraPos;
			if (yCameraPos > -1.0f) {
				yCameraPos = -1.0f;
				current_state.yGoal = yCameraPos;
			}
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
			paletteModifier.toggleShow();
			paletteModifier.reCreateObjects(getObjectByName(objects, "GUI_Palette_Modifier_"));
			generate_Palette_Modifier_Rend_To_Text();
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
		xText = std::to_string(x), yText = std::to_string(y), zText = std::to_string(z);
		if (itemInfoWindow) {
			itemInfo.checkHover(getObjectByName(objects, "GUI_Item_Info_Panel_"), xPos, yPos);
		}
		else if (paletteModifier.getShow()) {
			paletteModifier.checkHover(getObjectByName(objects, "GUI_Palette_Modifier_"), xPos, yPos);
		}
		else {
			//bottomBar.updateLabels(getObjectByName(objects, "GUI_BottomBar_"));
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
	// Handle hover over left panel
	if (leftPanel.getShow()) {
		leftPanel.checkHover(getObjectByName(objects, "GUI_LeftPanel_"), xPos, yPos);
	}
	// Handle hover over bottom bar
	if (bottomBar.getShow()) {
		bottomBar.checkHover(getObjectByName(objects, "GUI_BottomBar_"), xPos, yPos);
	}
}