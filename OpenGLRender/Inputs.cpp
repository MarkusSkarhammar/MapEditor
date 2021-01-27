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
					//itemInfoTile = world->getFloor(z)->getTile((x / world->get_Width_Section() + (y / world->get_Width_Section()) * world->get_Height_Section()), x, y);
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
						int section = 0;
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
								section = (xPos / world->get_Width_Section_Tiles() + (yPos / world->get_Width_Section()) * world->get_Height_Section());
								thingsToDraw.insert(ToDraw(tempID, xPos, yPos, z, section));
							}
						}
					}
					outlinedItem = nullptr;
				}
			}

			lbutton_down = true;
		}else if (GLFW_RELEASE == action) {
			lbutton_down = false;
		}

	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			if (selectedItemId.getIDRef() != -1) {
				thingsToDraw.clear();
				selectedItemId = PaletteItem(-1, false, -1, -1);
			}

			if (ToggleButtonGroup* check = dynamic_cast<ToggleButtonGroup*>(bottomBar->getElementByName("toggles"))) {
				check->resetAll();
				thingsToDraw.clear();
				//getObjectByName(objects, "GUI_Preview_Tiles_")->clearObjects();
			}

			// Reset left panel
			if (leftPanel->getShow()) {
				if (ToggleButtonGroup* tg = dynamic_cast<ToggleButtonGroup*>(leftPanel->getElementByName("paletteToggleGroup"))) {
					tg->resetAll();
					tg->set_Skip_Hover(false);
					leftPanelUpdate = true;
				}
				if (DropDown* dd = dynamic_cast<DropDown*>(leftPanel->getElementByName("itemsSelection")))
					if (dd->getShowDropDown())
						dd->toggleShowDropDown();
			}
		}
	}


	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		zoomWorld = 1.0;
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

	// Check if any GUI panels were clicked
	for (auto panel : GUIPanels) {
		if (panel->getShow())
			panel->checkClicked(xPos, yPos, button, action);
	}
}

void mouse_scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	
	
	if (lControl && yOffset != 0 && !verticesCreation->getShow()) {
		zoomWorld -= (yOffset * 0.1);
		if (zoomWorld < 0.2) zoomWorld = 0.2;
	}
	else if (!lShift && yOffset != 0) {
		if (isWithinTileArea && !verticesCreation->getShow()) {
			yCameraPos += yOffset * (imgScale / (screenHeightPixels / 2.0));
			//yCameraPos = round(yCameraPos * 1000) / 1000.;
			//current_state.yGoal = int(yCameraPos * 540) / 540.;
			current_state.yGoal = yCameraPos;
			if (yCameraPos > -1.0f) {
				yCameraPos = -1.0f;
				current_state.yGoal = yCameraPos;
			}
		}
	}
	else if (isWithinTileArea && yOffset != 0 && !verticesCreation->getShow()) {
		xCameraPos += yOffset * (imgScale / (screenWidthPixels / 2.0));
		xCameraPos = round(xCameraPos * 1000.0) / 1000.;
		current_state.xGoal = xCameraPos;
		if (xCameraPos < 1.0f) {
			xCameraPos = 1.0f;
			current_state.xGoal = xCameraPos;

		}
	}

	// Check if left clicked any GUI panels
	for (auto panel : GUIPanels) {
		if (panel->getShow() && panel)
			panel->checkScroll(xPos, yPos, yOffset);
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

	if (key == GLFW_KEY_ESCAPE) {
		if (action == GLFW_PRESS) {
			quit_game = true;
		}
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
			serializer.saveWorld(world);
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
		if (lControl && !changeWorldLock) {
			changeWorldLock = true;
			std::thread(&Serialize::loadWorld, serializer, std::ref(mapFileName)).detach();
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

	if (key == GLFW_KEY_5) {
		if (action == GLFW_PRESS) {
			brush = 4;
		}
	}

	if (key == GLFW_KEY_V) {
		if (action == GLFW_PRESS) {
			if (lShift) {
				verticesCreation->toggleShow();

				if (verticesCreationFirstShow) {
					setup_Vertices_Generation_Selection_Area();
					generate_Vertices_Creation_Rend_To_Text();
					generate_Vertices_Creation_Preview_Rend_To_Text();
					verticesCreationFirstShow = false;
				}
			}
		}
	}

	if(!quit_game)
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
		if (!updateWorld) {

			auto tile = world->getFloor(z)->getTile(currentSection, x, y);

			if (selectedItemId.getIDRef() == -1 && tile != nullptr) {
				if (outLinedTile) outLinedTile->getObject()->setOutline(false);
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
		}

		if (!paletteModifier->getShow() && !itemInfo->getShow() && !verticesCreation->getShow()) {
			xText = "x: " + std::to_string(x), yText = "y: " + std::to_string(y), zText = "z: " + std::to_string(z);
			if (eraseToggle || destroyToggle || destroyTileToggle || copyToggle || cutToggle || selectedItemId.getIDRef() != -1) {
				if (lbutton_down) {
					int section = 0;
					int tempID(0);
					if (eraseToggle)
						tempID = -1;
					else if (destroyToggle)
						tempID = -2;
					else
						tempID = selectedItemId.getIDRef();
					for (int yPos = y - brush; yPos <= y + brush; yPos++) {
						for (int xPos = x - brush; xPos <= x + brush; xPos++) {
							section = (xPos / world->get_Width_Section_Tiles() + (yPos / world->get_Width_Section()) * world->get_Height_Section());
							thingsToDraw.insert(ToDraw(tempID, xPos, yPos, z, section));
						}
					}
				}
				generate_GUI_Preview_Tiles();
			}
		}
	}
	else if (isWithinTileArea)
		isWithinTileArea = false;

	// Check if hovering over any GUI panels
	for (auto panel : GUIPanels) {
		if (panel->getShow())
			panel->checkHover(xPos, yPos);
	}
}