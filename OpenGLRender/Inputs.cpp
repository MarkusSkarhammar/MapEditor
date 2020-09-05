#include "Inputs.h"
#include <thread>
#include "Serializer.h"
#include "Palette.h"
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
						generate_GUI_Item_Info_Panel();
					}
				}
				else if (!itemInfoWindow && (selectedItemId.getIDRef() != -1 || eraseToggle || destroyToggle || destroyTileToggle || copyToggle || cutToggle) && thingsToDraw.size() == 0) {
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
							tempID = selectedItemId.getIDRef();
						for (int yPos = y - brush; yPos <= y + brush; yPos++) {
							for (int xPos = x - brush; xPos <= x + brush; xPos++) {
								thingsToDraw.insert(ToDraw(tempID, xPos, yPos, z, (xPos / 50 + (yPos / 50) * 40)));
							}
						}
					}
					outlinedItem = nullptr;
				}
				else if (itemInfoWindow) {
					itemInfo->checkClicked(xPos, yPos, 1);
				}
			}
			// Handle clicks on the bottom bar
			if (bottomBar->getShow()) {
				bottomBar->checkClicked(xPos, yPos, 1);
			}
			// Handle clicks on the left panel
			if (leftPanel->getShow()) {
				leftPanel->checkClicked(xPos, yPos, 1);
			}
			// Handle clicks on the platte modifier window
			if (paletteModifier->getShow()) {
				paletteModifier->checkClicked(xPos, yPos, 1);
			}

			lbutton_down = true;
		}else if (GLFW_RELEASE == action) {
			lbutton_down = false;
			if (itemInfoWindow) {
				itemInfo->checkClicked(xPos, yPos, 0);
				if (updateItemInfo) {
					updateItemInfo = false;
					generate_GUI_Item_Info_Panel();
				}
			}


			if (leftPanel->getShow()) {
				leftPanel->checkClicked(xPos, yPos, 0);
			}

			// Handle clicks on the platte modifier window
			if (paletteModifier->getShow()) {
				paletteModifier->checkClicked(xPos, yPos, 0);
			}
		}

	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (selectedItemId.getIDRef() != -1) {
			thingsToDraw.clear();
			selectedItemId = PaletteItem(-1, false, -1, -1);
		}
		if (ToggleButtonGroup* check = dynamic_cast<ToggleButtonGroup*>(bottomBar->getElementByName("toggles"))) {
			check->resetAll();
			thingsToDraw.clear();
			getObjectByName(objects, "GUI_Preview_Tiles_")->clearObjects();
		}
		// Reset left panel
		if (leftPanel->getShow()) {
			if (ToggleButtonGroup* tg = dynamic_cast<ToggleButtonGroup*>(leftPanel->getElementByName("paletteToggleGroup"))) {
				tg->resetAll();
				generate_Left_Panel_Rend_To_Text();
			}
			if (DropDown * dd = dynamic_cast<DropDown*>(leftPanel->getElementByName("itemsSelection")))
				if (dd->getShowDropDown())
					dd->toggleShowDropDown();
		}
		if (paletteModifier->getShow()) {
			paletteModifier->checkClicked(xPos, yPos, 2);
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
		zoom -= (yOffset * 0.1);
		if (zoom < 0.2) zoom = 0.2;
		yCameraPos = yCameraPos;
		current_state.yGoal = yCameraPos;
		xCameraPos = xCameraPos;
		current_state.xGoal = xCameraPos;
		//setCameraZoom(zoom / 90);
	}
	else if (!lShift && yOffset != 0) {
		if (isWithinTileArea) {
			if (paletteModifier->getShow()) {
				paletteModifier->checkScroll(xPos, yPos, yOffset);
			}
			else {
				yCameraPos += yOffset * (imgScale / (screenHeightPixels / 2.0));
				current_state.yGoal = yCameraPos;
				if (yCameraPos > -1.0f) {
					yCameraPos = -1.0f;
					current_state.yGoal = yCameraPos;
				}
			}

			/*
			else {
				yCameraPos += yOffset * (imgScale / (screenHeightPixels / 2.0));
				current_state.yGoal = yCameraPos;
				if (yCameraPos > -1.0f) {
					yCameraPos = -1.0f;
					current_state.yGoal = yCameraPos;
				}
			}
			*/
		}
		else {
			if (leftPanel->getShow()) {
				leftPanel->checkScroll(xPos, yPos, yOffset);
			}
		}
	}
	else if (isWithinTileArea && yOffset != 0) {
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

	if (key == GLFW_KEY_LEFT_SHIFT) {
		if (action == GLFW_PRESS)
			lShift = true;
		else if (GLFW_RELEASE == action)
			lShift = false;
	}

	if (key == GLFW_KEY_H) {
		if (action == GLFW_PRESS) {
			if (!outlineItemBool) {
				if (outlineTileBool)
					outlineTileBool = false;
				else
					outlineItemBool = true;
			}
			else if (outlineItemBool) {
				outlineItemBool = false;
				outlineTileBool = true;
			}
		}
	}

	if (key == GLFW_KEY_Z) {
		if (lControl && action == GLFW_PRESS) {
			if (paletteModifier->getShow() && palette_Modifier_Undo.size() > 0) {
				auto& changes = palette_Modifier_Undo.at(0);
				if (changes.getAction() == "Deleted") {
					for (auto& e : changes.getItemsToCreate()) {
						tempPalette.addItem(e);
					}
				} 
				/// TODO SORT CHANGED PALETTE!
				palette_Modifier_Redo.push_back(PaletteChange("Created", std::vector<PaletteItem>(), changes.getItemsToCreate()));
				palette_Modifier_Undo.pop_back();

				//generate_Palette_Modifier_Fill_Palette_Right(tempPalette);
			}
		}
	}

	if (key == GLFW_KEY_Y) {
		if (lControl && action == GLFW_PRESS) {
			if (paletteModifier->getShow() && palette_Modifier_Redo.size() > 0) {
				auto& changes = palette_Modifier_Redo.at(0);
				std::vector<PaletteItem> changesItem;
				if (changes.getAction() == "Deleted") {
					for (auto& e : changes.getItemsToCreate()) {
						tempPalette.addItem(e);
					}
				}else if (changes.getAction() == "Created") {
					for (auto& e : changes.getItemsToDelete()) {
						tempPalette.removeItemAtLocation(e.getX(), e.getY(), changesItem);
					}
				}
				/// TODO SORT CHANGED PALETTE!
				palette_Modifier_Undo.push_back(PaletteChange("Deleted", changesItem, std::vector<PaletteItem>()));
				palette_Modifier_Redo.pop_back();

				//generate_Palette_Modifier_Fill_Palette_Right(tempPalette);
			}
		}
	}

	if (key == GLFW_KEY_S) {
		if (lControl && action == GLFW_PRESS) {
			//serializer.saveWorld(world);
		}
		else if (action == GLFW_PRESS) {
			if (!itemInfoWindow && !paletteModifier->getShow()) {
				//updateCameraPosition(0, -(imgScale / (1080 / 2)));
				moveDown = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveDown = false;
		}
	}

	/*
	* Loading map hotkey
	*/
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		if (lControl && !loadWorldLock) {
			loadWorldLock = true;
			//worldLoadTemp.deleteWorld();
			std::thread(&Serialize::loadWorld, serializer, std::ref(worldLoadTemp)).detach();
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
			paletteModifier->toggleShow();
			paletteModifier->reCreateObjects();
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
			generate_GUI_Item_Info_Panel();
		}
	}

	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			if (!itemInfoWindow && !paletteModifier->getShow()) {
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
			if (!itemInfoWindow && !paletteModifier->getShow()) {
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
			else if (!itemInfoWindow && !paletteModifier->getShow()) {
				//updateCameraPosition(0, (imgScale / (1080 / 2)));
				moveUp = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveUp = false;
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

	for (auto panel : GUIPanels) {
		if (panel->getShow())
			panel->checkKeyStroke(key, action, mods);
	}

}

void handelHover() {

	if ((xPos >= 0.0 && xPos <= screenWidth - 276) && (yPos >= 0.0 && yPos <= screenHeight - 30)) {
		isWithinTileArea = 1;
		x = /*(xCoord / imgScale)*/ ((xCameraPos - 1.0f) / (imgScale / (screenWidth / 2))) + (xPos / imgScale);
		y = -((yCameraPos + 1.f) / (imgScale / (screenHeight / 2))) + (yPos / imgScale);
		auto tile = world.getFloor(z).getTile(currentSection, x, y);
		/*
		if (selectedItemId.getIDRef() == -1 && tile != nullptr && tile->getAllItems().size() > 0) {
			Item* currentHoveredItem = tile->getTopItem();
			if (currentHoveredItem) {
				if (outlinedItem != nullptr && outlinedItem->getObject() != nullptr)
					outlinedItem->setOutline(false);
				currentHoveredItem->setOutline(true);
				outlinedItem = currentHoveredItem;
			}
			else if (outlinedItem != nullptr && outlinedItem->getObject() != nullptr) {
				outlinedItem->setOutline(false);
			}
		}
		else {
			if(outlinedItem) outlinedItem->setOutline(false);
			outlinedItem = nullptr;
		}
		*/

		if (selectedItemId.getIDRef() == -1 && tile != nullptr) {
			if(outLinedTile) outLinedTile->getObject()->setOutline(false);
			if (outLinedTile) outLinedTile = tile;
			Item* currentHoveredItem = tile->getTopItem();
			if (currentHoveredItem) {
				if (outlinedItem != nullptr && outlinedItem->getObject() != nullptr)
					outlinedItem->setOutline(false);
				currentHoveredItem->setOutline(true);
				outlinedItem = currentHoveredItem;
			}
			else {
				if (outlinedItem != nullptr && outlinedItem->getObject() != nullptr)
					outlinedItem->setOutline(false);
				if (outLinedTile) outLinedTile->getObject()->setOutline(true);
			}
		}
		else {
			if (outlinedItem) outlinedItem->setOutline(false);
			outlinedItem = nullptr;
		}

		if (itemInfoWindow) {
			itemInfo->checkHover(xPos, yPos);
		}
		else if (paletteModifier->getShow()) {
			paletteModifier->checkHover(xPos, yPos);
		}
		else {
			xText = "x: " + std::to_string(x), yText = "y: " + std::to_string(y), zText = "z: " + std::to_string(z);
			if (eraseToggle || destroyToggle || destroyTileToggle || copyToggle || cutToggle || selectedItemId.getIDRef() != -1) {
				if (lbutton_down) {
					int tempID(0);
					if (eraseToggle)
						tempID = -1;
					else if (destroyToggle)
						tempID = -2;
					else
						tempID = selectedItemId.getIDRef();
					for (int yPos = y - brush; yPos <= y + brush; yPos++) {
						for (int xPos = x - brush; xPos <= x + brush; xPos++) {
							thingsToDraw.insert(ToDraw(tempID, xPos, yPos, z, (xPos / 50 + (yPos / 50) * 40)));
						}
					}
				}
				generate_GUI_Preview_Tiles();
			}
		}
	}
	else if (isWithinTileArea)
		isWithinTileArea = false;
	// Handle hover over left panel
	if (leftPanel->getShow()) {
		leftPanel->checkHover(xPos, yPos);
	}
	// Handle hover over bottom bar
	if (bottomBar->getShow()) {
		bottomBar->checkHover(xPos, yPos);
	}
}