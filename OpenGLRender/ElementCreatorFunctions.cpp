#include "ElementCreatorFunctions.h"

/**
* Reset left palette modifier's RTT.
*
*/
void reset_Left_Palette_Modifier_RTT()
{
	auto preview = dynamic_cast<GUIGroup*>(paletteModifier->getElementByName("rightSidePreview"));
	auto previewHover = dynamic_cast<GUIGroup*>(paletteModifier->getElementByName("rightSidePreviewHover"));
	preview->clear();
	previewHover->clear();
	if (preview->getShow()) {
		preview->toggleShow();
		previewHover->toggleShow();
	}
	if (ToggleButtonGroup* tgb = dynamic_cast<ToggleButtonGroup*>(paletteModifier->getElementByName("paletteToggleGroupLeft"))) {
		tgb->resetAll(); // Reset all the buttons.
		auto tb = tgb->getElementByHover();
		if (tb != nullptr) {
			tb->setV(tb->getHoverVertices());
		}
	}

	if (palette_Modifier_Left_Shift_Start != -1) // Reset integer used for keeping track of when several buttons are clicked
		palette_Modifier_Left_Shift_Start = -1;

	palette_Modifier_Left_Shift_Bool = false;
	palette_Modifier_Left_Clicked.clear();

	generate_Palette_Modifier_Rend_To_Text(); // Redraw the rtt.
}

/**
* Reset right palette modifier's RTT.
*
*/
void reset_Right_Palette_Modifier_RTT()
{
	if (!palette_Modifier_Right_First) {
		palette_Modifier_Right_Selected_Tile.changeAll(-1, false, -1, -1);
		palette_Modifier_Right_First = true;
	}
	if (palette_Modifier_Right_Shift_Start != -1)
		palette_Modifier_Right_Shift_Start = -1;
	palette_Modifier_Right_Shift_Bool = false;
	palette_Modifier_Right_Clicked.clear();
	if (ToggleButtonGroup* tgb = dynamic_cast<ToggleButtonGroup*>(paletteModifier->getElementByName("paletteToggleGroupRight"))) {
		tgb->resetAll();
		auto tb = tgb->getElementByHover();
		if (tb != nullptr) {
			tb->setV(tb->getHoverVertices());
		}
	}
	generate_Palette_Modifier_Rend_To_Text(); // Redraw the rtt.
}

/**
* Make sure that empty buttons aren't considered clicked.
*
*@param p, the palette which is to be used as a reference for valid clicks.
*@param clicked, the map of button clicks which is to be checked for redundant button clicks.
*/
void palette_Modifier_Check_Clicked_Buttons(Palette& p, std::unordered_map<int, int>& clicked)
{
	for (auto& i : p.getPalette()) {
		int pos = (i.getY() * 4) + i.getX() + 4;
		//if (pos > pageCompensation)
			//pos -= 4;
		auto location = clicked.find(pos);
		if (location != clicked.end()) {
			(*location).second = i.getIDRef();
		}
	}
	for (auto it = clicked.begin(); it != clicked.end(); ) {
		if ((*it).second == -1) {
			it = clicked.erase(it);
		}
		else {
			++it;
		}
	}
}

/**
* The lambda that is to be triggered on a mouseclick on the left palette modifier.
*
*/
std::function<void(int& mousebutton, int& mouseState)> get_Palette_Modifier_Left_Mouse_Lambda()
{
	auto lambda = [](int& mousebutton, int& mouseState) { // Lambda for when the ToggleButtonGroup is clicked.
		if (mousebutton == MOUSE_RIGHT_CLICK && mouseState == MOUSE_RELEASE) { // Handle right clicks
			reset_Left_Palette_Modifier_RTT();
			reset_Right_Palette_Modifier_RTT();
		}
		else if (mousebutton == MOUSE_LEFT_CLICK && mouseState == MOUSE_PRESS && palette_Modifier_Left_Selected_Palette.size() != 0) { // Handle left clicks
			auto preview = dynamic_cast<GUIGroup*>(paletteModifier->getElementByName("rightSidePreview")); // Grab the preview element.
			auto previewHover = dynamic_cast<GUIGroup*>(paletteModifier->getElementByName("rightSidePreviewHover")); // Grab the previewHover element.
			if (ToggleButtonGroup* tbg = dynamic_cast<ToggleButtonGroup*>(paletteModifier->getElementByName("paletteToggleGroupLeft"))) {
				int x = (xPos - tbg->getXStart()) / 64, y = (yPos - tbg->getYStart()) / 64, pos = ((int)((yPos - tbg->getYStart() + tbg->getRendToTextOffsetY() + (palette_Modifier_Left_page * palette_Modifier_Left_Offset_Max) - (palette_Modifier_Left_page * 64.0)) / 64) * 4) + x; // Get the x, y and pos values from the mouse position.
				int pageCompensation = ((int)(((palette_Modifier_Left_page * palette_Modifier_Left_Offset_Max) - (palette_Modifier_Left_page * 64.0)) / 64) * 4);

				//printf("pos = %i \n", pos);

				auto tb = tbg->getCurrentlyPressed();
				if (tb) { // Make sure that the clicked tile is the same as the last pressed button. This is necessary due to 4xsize tile items.
					std::string str = tbg->getCurrentlyPressed()->getName(), erase = "toggle_";
					Remove_SubString_From_String(str, erase);
					int pressedPos = std::stoi(str);
					if (pressedPos != pos - pageCompensation)
						pos = pressedPos;
				}

				if (palette_Modifier_Left_Shift_Start == -1 || (palette_Modifier_Left_Shift_Start != -1 && !lShift)) { // Handle first click or second click without shift held down

					palette_Modifier_Left_Clicked.clear();

					if (palette_Modifier_Left_Shift_Bool) {
						if (tb && !tb->getClicked())
							tb->doToggle(MOUSE_LEFT_CLICK, MOUSE_RELEASE);
						else if (tb && !tb->getToggle() && tb->getClicked()) {
							tb->setPressed(true);
						}
						palette_Modifier_Left_Shift_Bool = false;
					}

					if (palette_Modifier_Left_Shift_Start == pos)
						palette_Modifier_Left_Shift_Start = -1;
					else
						palette_Modifier_Left_Shift_Start = pos;

				}
				else if (lShift) { // Handle if shift is held down
					auto rtt = findByName(renderToTextureContainer, "paletteModifier");
					ToggleButton* tb = nullptr;
					if (pos > palette_Modifier_Left_Shift_Start) {
						for (int i = palette_Modifier_Left_Shift_Start; i < pos; i++) {
							if (i < pageCompensation)
								tb = tbg->getElementByName("toggle_" + std::to_string(i));
							else
								tb = tbg->getElementByName("toggle_" + std::to_string(i - pageCompensation));
							if (tb && !tb->getClicked() && tb->getButton()) {
								tb->toggleClicked();
								palette_Modifier_Left_Clicked.insert(std::pair<int, int>(i, findByName(tb->getMiscellaneousIDsList(), "itemID")));
							}
							else
								palette_Modifier_Left_Clicked.insert(std::pair<int, int>(i, -1));
						}
					}
					else {
						for (int i = palette_Modifier_Left_Shift_Start; i > pos; i--) {
							if (i < pageCompensation)
								tb = tbg->getElementByName("toggle_" + std::to_string(i));
							else
								tb = tbg->getElementByName("toggle_" + std::to_string(i - pageCompensation));
							if (tb && !tb->getClicked() && tb->getButton()) {
								tb->toggleClicked();
								palette_Modifier_Left_Clicked.insert(std::pair<int, int>(i, findByName(tb->getMiscellaneousIDsList(), "itemID")));
							}
							else
								palette_Modifier_Left_Clicked.insert(std::pair<int, int>(i, -1));
						}
					}
					palette_Modifier_Left_Shift_Bool = true;
				}

				if (tb)
					palette_Modifier_Left_Clicked.insert(std::pair<int, int>(pos, findByName(tb->getMiscellaneousIDsList(), "itemID")));

				// Make sure that the selected tiles actually contains items and not empty tiles.
				auto tf = paletteModifier->getElementByName("textField");
				if (tf && tf->getText() != "")
					palette_Modifier_Check_Clicked_Buttons(tempPaletteLeft, palette_Modifier_Left_Clicked);
				else if (tf)
					for (Palette p : palettes) {
						if (p.getName().compare(palette_Modifier_Left_Selected_Palette) == 0) {
							palette_Modifier_Check_Clicked_Buttons(p, palette_Modifier_Left_Clicked);
							break;
						}
					}

				// Create the preview items for the right side RTT.
				preview->clear();
				previewHover->clear();
				auto rtt = findByName(renderToTextureContainer, "paletteModifier");
				for (auto& tb : palette_Modifier_Left_Clicked) {
					if (tb.second != -1) {
						auto itemObj = itemAtlas.getItemObject(tb.second);
						if (itemObj) {
							auto v = rtt->getVerticesByName(itemObj->getName());
							std::vector<GUIElement*> previewTemp = { new GUIElement("", 0, 0, 63, 63), new GUIElement("", 0, 0, 63, 63) };
							previewTemp.at(0)->addMiscellaneousID("itemID", tb.second);
							previewTemp.at(0)->setV(v);
							if (v->isDoubleSize()) {
								previewTemp.at(0)->setWidth(127);
								previewTemp.at(0)->setHeight(127);
								previewTemp.at(1)->setV(rtt->getVerticesByName("yellowSquareBig"));
							}
							else
								previewTemp.at(1)->setV(rtt->getVerticesByName("yellowSquareSmall"));
							preview->addElement(previewTemp.at(0));
							previewHover->addElement(previewTemp.at(1));
						}
					}
				}
				generate_Palette_Modifier_Rend_To_Text();
			}
		}
	};
	return lambda;
}

/**
* The lambda that is to be triggered on a mouseclick on the right palette modifier.
*
*@return the generated lambda function.
*/
std::function<void(int& mousebutton, int& mouseState)> get_Palette_Modifier_Right_Mouse_Lambda()
{
	auto lamda = [](int& mousebutton, int& mouseState) {
		if (mousebutton == MOUSE_RIGHT_CLICK && mouseState == MOUSE_RELEASE) {
			reset_Left_Palette_Modifier_RTT();
			reset_Right_Palette_Modifier_RTT();
		}
		else if (mousebutton == MOUSE_LEFT_CLICK && mouseState == MOUSE_PRESS && !lControl) {
			auto preview = dynamic_cast<GUIGroup*>(paletteModifier->getElementByName("rightSidePreview"));
			auto previewHover = dynamic_cast<GUIGroup*>(paletteModifier->getElementByName("rightSidePreviewHover"));
			if (preview->getAmount() > 0) {
				if (ToggleButtonGroup* tgbLeft = dynamic_cast<ToggleButtonGroup*>(paletteModifier->getElementByName("paletteToggleGroupLeft"))) {
					if (ToggleButtonGroup* tgb = dynamic_cast<ToggleButtonGroup*>(paletteModifier->getElementByName("paletteToggleGroupRight"))) {
						auto& p = tempPalette;
						auto previewItems = preview->getElements();
						auto previewHoverItems = previewHover->getElements();
						GUIElement* previewItem{ nullptr };

						for (int i = 0; i < previewItems.size(); i++) {
							previewItem = previewItems.at(i);
							int pageSize = screenHeight / 64, pos = previewItem->getMiscellaneousID(1),
								yPosPalette = int(pos / 4), xPosPalette = pos % 4, itemID = previewItem->getMiscellaneousID(0),
								yPosMouse = yPosPalette, xPosMouse = xPosPalette;
							bool placed = false, doubleSize = itemAtlas.checkIfDouleSize(itemID);
							auto test = p.getItemAtLocation(xPosPalette, yPosPalette);
							if (test.first) {
								auto& paletteItem = test.second;
								if (!doubleSize) {
									if (!paletteItem.isDoubleSize()) {
										paletteItem.setID(itemID);
										placed = true;
									}
									else {
										// Remove the palettes surrounding the doubleSize if there is any
										p.removeItemAtLocation(xPosPalette + 1, yPosPalette);
										p.removeItemAtLocation(xPosPalette, yPosPalette + 1);
										p.removeItemAtLocation(xPosPalette + 1, yPosPalette + 1);

										// Change the doubleSize to the regular size and change its position
										paletteItem.setID(itemID);
										paletteItem.setDoubleSize(false);
										paletteItem.setX(xPosMouse);
										paletteItem.setY(yPosMouse);
									}
								}
								else {
									if (xPosMouse != 3) {

										// Remove the palettes surrounding the doubleSize if there is any
										p.removeItemAtLocation(xPosMouse + 1, yPosMouse);
										p.removeItemAtLocation(xPosMouse, yPosMouse + 1);
										p.removeItemAtLocation(xPosMouse + 1, yPosMouse + 1);

										paletteItem.setID(itemID);
										paletteItem.setDoubleSize(true);
										paletteItem.setX(xPosMouse);
										paletteItem.setY(yPosMouse);
									}
								}
							}
							else {
								if (!doubleSize) {
									p.addItem(itemID, doubleSize, xPosPalette, yPosPalette);
								}
								else {
									if (xPosMouse != 3) {
										p.addItem(itemID, doubleSize, xPosPalette, yPosPalette);

										// Remove items if they overlap
										p.removeItemAtLocation(xPosPalette + 1, yPosPalette); // Remove east
										p.removeItemAtLocation(xPosPalette, yPosPalette + 1); // Remove south
										p.removeItemAtLocation(xPosPalette + 1, yPosPalette + 1); // Remove south east

										p.removeItemAtLocationIFDoubleSize(xPosPalette + 1, yPosPalette - 1); // Remove double size if at north east
										p.removeItemAtLocationIFDoubleSize(xPosPalette - 1, yPosPalette + 1); // Remove double size if at south west
									}
								}
							}
						}

						tgb->resetAll();
						if (preview->getShow()) {
							preview->toggleShow();
							previewHover->toggleShow();
						}
						if (palette_Modifier_Right_Shift_Start != -1)
							palette_Modifier_Right_Shift_Start = -1;
						auto rttTemp = paletteModifier->getElementByName("tilesRight");
						prepare_Palette_Modifier_Rend_To_Text(paletteModifier, rttTemp->getRenderToText(), rttTemp, tgb, p, palette_Modifier_Right_page, palette_Modifier_Right_Offset_Max, palette_Modifier_Right_Clicked);
					}
				}

				generate_Palette_Modifier_Rend_To_Text();

			}
			else {
				if (ToggleButtonGroup* tbg = dynamic_cast<ToggleButtonGroup*>(paletteModifier->getElementByName("paletteToggleGroupRight"))) {
					int x = (xPos - tbg->getXStart()) / 64, y = (yPos - tbg->getYStart()) / 64, pos = ((int)((yPos - tbg->getYStart() + tbg->getRendToTextOffsetY() + (palette_Modifier_Right_page * palette_Modifier_Right_Offset_Max) - (palette_Modifier_Right_page * 64.0)) / 64) * 4) + x; // Get the x, y and pos values from the mouse position.
					int pageCompensation = ((int)(((palette_Modifier_Right_page * palette_Modifier_Right_Offset_Max) - (palette_Modifier_Right_page * 64.0)) / 64) * 4);

					if (palette_Modifier_Right_First)
						palette_Modifier_Right_First = false;

					if (palette_Modifier_Right_Clicked.size() > 0 && !lShift) {
						auto& p = tempPalette;
						int position = pos;
						bool change = false;
						for (auto& e : palette_Modifier_Right_Clicked) {
							int yPosPalette = position / 4 - 1, xPosPalette = position % 4, itemID = -1;
							auto item = p.getItemAtLocation(xPosPalette, yPosPalette);
							if (item.first)
								itemID = item.second.getIDRef();
							bool doubleSize = itemAtlas.checkIfDouleSize(itemID);
							palette_Modifier_Right_Selected_Tile.changeAll(itemID, doubleSize, xPosPalette, yPosPalette);
							{ // Switch the first and second item
								auto& firstTile = palette_Modifier_Right_Selected_Tile;
								auto secondTileGet = p.getItemAtLocation(e.first % 4, e.first / 4 - 1);
								auto& secondTile = secondTileGet.second;
								if (secondTile != firstTile) {
									if (secondTile.isDoubleSize()) {
										if (firstTile.isDoubleSize()) {
											p.getItemAtLocation(firstTile.getX(), firstTile.getY()).second.changePositions(secondTile.getX(), secondTile.getY());
											secondTile.changePositions(firstTile.getX(), firstTile.getY());
											change = true;
										}
										else {
											if (position % 4 == 3)
												position++;
											if (position % 4 < 3 && !p.check_If_Double_Size_Is_Blocked_At_Location(firstTile.getX(), firstTile.getY(), secondTile.getX(), secondTile.getY()) /*&& p.check_If_Double_Size_Is_Possible_At_Location(secondTile.getX(), secondTile.getY(), firstTile.getX(), firstTile.getY()) > 0*/) {
												int check = 0, yDifference = std::abs(secondTile.getY() - (position / 4 - 1)), xDifference = std::abs(secondTile.getX() - (position % 4));
												if (secondTile.getY() < (position / 4 - 1))
													yDifference *= -1;
												if (secondTile.getX() < position % 4)
													xDifference *= -1;
												while ((check = p.check_If_Double_Size_Is_Possible_At_Location(position % 4, position / 4 - 1, secondTile.getX(), secondTile.getY())) != -1) {
													p.changePositionAt(check % 4 + xDifference, check / 4 + yDifference, check % 4, check / 4);
												}
												secondTile.changePositions(position % 4, position / 4 - 1);
												change = true;
											}
										}
									}
									else if (!secondTile.isDoubleSize()) {
										if (!firstTile.isDoubleSize()) {
											if (firstTile.getID() != -1) {
												p.getItemAtLocation(firstTile.getX(), firstTile.getY()).second.changePositions(secondTile.getX(), secondTile.getY());
												secondTile.changePositions(firstTile.getX(), firstTile.getY());
												change = true;
											}
											else {
												int check = 0;
												while ((check = p.check_If_Normal_Size_Is_Possible_At_Location(position % 4, position / 4 - 1)) != 0) {
													p.removeItemAtLocation(position % 4, position / 4 - 1);
												}
												secondTile.changePositions(position % 4, position / 4 - 1);
												change = true;
											}
										}
										else {
											if (p.check_If_Double_Size_Is_Possible_At_Location(secondTile.getX(), secondTile.getY(), position % 4, position / 4 - 1) != -1)
												p.removeItemAtLocation(position % 4, position / 4 - 1);
											else
												p.getItemAtLocation(position % 4, position / 4 - 1).second.changePositions(secondTile.getX(), secondTile.getY());
											secondTile.changePositions(position % 4, position / 4 - 1);
											change = true;
										}
									}
								}
								if (secondTile.isDoubleSize()) {
									position++;
								}
							}
							position++;
							if (position % 4 == 0 && p.is_Double_Size(position % 4, position / 4 - 2))
								position += 2;
							if (position % 4 == 2 && p.is_Double_Size(position % 4, position / 4 - 2))
								position += 2;
						}
						if (change) {
							p.sortPalette();
							auto rttTemp = paletteModifier->getElementByName("tilesRight");
							prepare_Palette_Modifier_Rend_To_Text(paletteModifier, rttTemp->getRenderToText(), rttTemp, tbg, p, palette_Modifier_Right_page, palette_Modifier_Right_Offset_Max, palette_Modifier_Right_Clicked);
							if (GUIElement* rtt = dynamic_cast<GUIElement*>(paletteModifier->getElementByName("tilesRight"))) {
								auto rttTemp = findByName(renderToTextureContainer, "paletteModifier");
								if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(paletteModifier->getElementByName("scrollbarRight"))) {
									sb->reset();
									auto paletteItems = p.getPalette();
									if (paletteItems.size() > 0) {
										auto y = paletteItems.at(paletteItems.size() - 1).getY();
										palette_Modifier_Right_maxPage = y * 64.0 / (palette_Modifier_Right_Offset_Max);
										double maxPixels = 64.0 * y,
											scrollbarLength = sb->getScrollbarMaxLength() * (((rtt->getHeight() / 64.0) * 64) / maxPixels);
										sb->updateScrollbarLengthAndPositionAndIncrementSize(scrollbarLength, 0, y * 64);
										int sbOldPos = (((palette_Modifier_Right_page * palette_Modifier_Right_Offset_Max) + palette_Modifier_Right_Offset - (palette_Modifier_Right_page * 64)) / sb->getIncrementSize());
										sb->updateScrollbarPosition(sbOldPos, false);
										//sb->updateScrollbarLengthAndPosition();
										//sb->updateScrollbarPosition(((palette_Modifier_Right_page * palette_Modifier_Right_Offset_Max) + palette_Modifier_Right_Offset - (palette_Modifier_Right_page * 64)) / sb->getIncrementSize(), false);
									}
									else {
										sb->updateScrollbarLengthAndPosition(10000, 0);
									}

								}
							}
							tbg->resetAll();
							palette_Modifier_Right_First = true;
							palette_Modifier_Right_Shift_Start = -1;
							palette_Modifier_Right_Shift_Bool = false;
							palette_Modifier_Right_Clicked.clear();
						}
					}

					if (!palette_Modifier_Right_First) {

						auto tb = tbg->getCurrentlyPressed();
						if (tb) { // Make sure that the clicked tile is the same as the last pressed button. This is necessary due to 4xsize tile items.
							std::string str = tbg->getCurrentlyPressed()->getName(), erase = "toggle_";
							Remove_SubString_From_String(str, erase);
							int pressedPos = std::stoi(str);
							if (pressedPos != pos - pageCompensation)
								pos = pressedPos;
						}

						if (palette_Modifier_Right_Shift_Start == -1 || (palette_Modifier_Right_Shift_Start != -1 && !lShift)) {

							palette_Modifier_Right_Clicked.clear();

							if (palette_Modifier_Right_Shift_Bool) {
								auto tb = tbg->getCurrentlyPressed();
								if (tb && !tb->getClicked())
									tb->doToggle(MOUSE_LEFT_CLICK, MOUSE_RELEASE);
								else if (tb && !tb->getToggle() && tb->getClicked()) {
									tb->setPressed(true);
								}
								palette_Modifier_Right_Shift_Bool = false;
							}
							if (palette_Modifier_Right_Shift_Start == pos)
								palette_Modifier_Right_Shift_Start = -1;
							else {
								palette_Modifier_Right_Shift_Start = pos;
							}

						}
						else if (lShift) {
							auto rtt = findByName(renderToTextureContainer, "paletteModifier");

							if (pos > palette_Modifier_Right_Shift_Start) {
								for (int i = palette_Modifier_Right_Shift_Start; i < pos; i++) {
									if (i < pageCompensation)
										tb = tbg->getElementByName("toggle_" + std::to_string(i));
									else
										tb = tbg->getElementByName("toggle_" + std::to_string(i - pageCompensation));
									if (tb && !tb->getClicked() && tb->getButton()) {
										tb->toggleClicked();
										palette_Modifier_Right_Clicked.insert(std::pair<int, int>(i, findByName(tb->getMiscellaneousIDsList(), "itemID")));
									}
									else
										palette_Modifier_Right_Clicked.insert(std::pair<int, int>(i, -1));
								}
							}
							else {
								for (int i = palette_Modifier_Right_Shift_Start; i > pos; i--) {
									if (i < pageCompensation)
										tb = tbg->getElementByName("toggle_" + std::to_string(i));
									else
										tb = tbg->getElementByName("toggle_" + std::to_string(i - pageCompensation));
									if (tb && !tb->getClicked() && tb->getButton()) {
										tb->toggleClicked();
										palette_Modifier_Right_Clicked.insert(std::pair<int, int>(i, findByName(tb->getMiscellaneousIDsList(), "itemID")));
									}
									else
										palette_Modifier_Right_Clicked.insert(std::pair<int, int>(i, -1));
								}
							}
							palette_Modifier_Right_Shift_Bool = true;
						}

						if (tb)
							palette_Modifier_Right_Clicked.insert(std::pair<int, int>(pos, findByName(tb->getMiscellaneousIDsList(), "itemID")));

						// Make sure that the selected tiles actually contains items and not empty tiles.
						palette_Modifier_Check_Clicked_Buttons(tempPalette, palette_Modifier_Right_Clicked);
					}
				}
			}
		}
	};
	return lamda;
}

/**
* The lambda function that is to be triggered on a scroll event.
*
*@param panel, the GUI panel which contains the element that contains triggered the scroll event.
*@param tbgName, the name of the toggle button group element which is the element that triggered the scroll event.
*@param sbName, the name of the scrollbar element which needs to be updated.
*@param rttName, the name of the render to texture element which needs to be updated.
*@param offset, the offset value of the rtt.
*@param offset_Max, the max offset before a new page is needed.
*@param page, the current page.
*@param selected_Palette, the selected palette.
*
*@return the generated lambda function.
*/
std::function<void(double& v)> get_Scroll_Lambda_Tbg(GUIPanel* panel, std::string tbgName, std::string sbName, std::string rttName, double& offset, double& offset_Max, int& page, std::string& selected_Palette, std::unordered_map<int, int>& map)
{
	auto lambda = [&, panel, tbgName, sbName, rttName](double& v) {
		if (GUIElement* rttElement = dynamic_cast<GUIElement*>(panel->getElementByName(rttName))) {
			if (rttElement->getHover() && selected_Palette != "") {

				offset -= (v) * 15;

				bool change = false;

				if (offset < 64) {
					if (page > 0) {
						change = true;
						offset = offset_Max - (64 - offset);
						page--;
					}
					else {
						offset = 64;
					}
				}
				else if (offset >= offset_Max) {
					offset = (offset - offset_Max) + 64;
					change = true;
					page++;
				}

				for (Palette p : palettes) {
					if (p.getName().compare(selected_Palette) == 0) {

						if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(panel->getElementByName(sbName))) {
							if (page == 0 && offset == 64.0)
								sb->updateScrollbarPosition(0, false);
							else {
								sb->updateScrollbarPosition(((page * offset_Max) + offset - (page * 64)) / sb->getIncrementSize(), false);
							}
						}

						auto tgb = panel->getElementByName(tbgName);
						rttElement->setOffsetY(offset - rttElement->getV()->getYStartText());
						tgb->setRendToTextOffsetY(offset);

						if (change) {
							auto rtt = rttElement->getRenderToText();
							if(rtt)
								prepare_Palette_Modifier_Rend_To_Text(panel, rtt, rttElement, tgb, p, page, offset_Max, map);
						}

						break;
					}
				}
			}
		}
	};
	return lambda;
}

/**
* The lambda function that is to be triggered on a scroll event.
*
*@param tbg, the toggle button group that triggers the hover event.
*
*@return the generated lambda function.
*/
std::function<void(double& xPos, double& yPos)> get_Hover_Lambda_Palette_Modifier_Right(ToggleButtonGroup* tbg)
{
	return [tbg](double& x, double& y) {
		bool change = false;
		auto preview = dynamic_cast<GUIGroup*>(paletteModifier->getElementByName("rightSidePreview"));
		auto previewHover = dynamic_cast<GUIGroup*>(paletteModifier->getElementByName("rightSidePreviewHover"));
		int xStart = (int)((x / 64)) * 64 - (4 * 64), yStart = (int)(y / 64) * 64;
		double xNormal = xPos + tbg->getXStart() - tbg->getRendToTextOffsetX(), yNormal = yPos + tbg->getYStart() - tbg->getRendToTextOffsetY();
		bool testBounds = (xPos >= tbg->getXStart() && xPos < (tbg->getXStart() + tbg->getWidth())) && (yPos >= tbg->getYStart() && yPos < (tbg->getYStart() + tbg->getHeight()));
		if (testBounds) {
			if ((lControl || lShift) && !tbg->getResetAfterToggle())
				tbg->setResetAfterToggle(true);
			else if (!(lControl || lShift) && tbg->getResetAfterToggle())
				tbg->setResetAfterToggle(false);
		}
		if (preview->getAmount() > 0 && palette_Modifier_Right_Selected_Palette != "") { // Make sure that atleast one item has been selected from the left side and make sure that a palette for the right side has benn selected.
			if (testBounds) { // Make sure that the cursor is within the bounds of the area
				if (preview->getAmount() == 1) { // Logic for if only one item was selected
					auto previewItem = preview->getElements().at(0);
					auto previewHoverItem = previewHover->getElements().at(0);
					if ((!xStart >= 192 && !previewItem->getV()->isDoubleSize()) || (xStart < 192 && previewItem->getV()->isDoubleSize()) || !previewItem->getV()->isDoubleSize()) { // Check to make sure that a doubleSized item won't be drawn outside of area
						int xTile = xStart / 64, yTile = yStart / 64;
						//previewItem->setXStartText(64 * 4 + xStart);
						//previewItem->setYStartText(yStart);
						previewItem->setStartX(64 * 4 + xStart);
						previewItem->setStartY(yStart);
						previewItem->addMiscellaneousID("position", (yTile - 1 + palette_Modifier_Right_page * (palette_Modifier_Right_Offset_Max / 64) - palette_Modifier_Right_page) * 4 + xTile, 1);
						//previewHoverItem->setXStartText(64 * 4 + xStart);
						//previewHoverItem->setYStartText(yStart);
						previewHoverItem->setStartX(64 * 4 + xStart);
						previewHoverItem->setStartY(yStart);
						//tbg->resetAll();
						change = true;
						if (!preview->getShow()) {
							preview->toggleShow();
							previewHover->toggleShow();
						}
					}
					else {
						if (preview->getShow()) {
							preview->toggleShow();
							previewHover->toggleShow();
							change = true;
						}
					}
				}
				else { // Logic for when More than one item were selected
					auto previewItems = preview->getElements();
					auto previewHoverItems = previewHover->getElements();
					int xTile = xStart / 64, yTile = yStart / 64, tempY = -1, tempY2 = -1, tempStartX = -1, tempStopX = -1, tempStartX2 = -1, tempStopX2 = -1;
					bool previousDoubleSize = false;
					GUIElement* previewItem{ nullptr }, * previewHoverItem{ nullptr };
					for (int i = 0; i < previewItems.size(); i++) {
						previewItem = previewItems.at(i);
						previewHoverItem = previewHoverItems.at(i);
						if (!previewItem->getV()->isDoubleSize()) {
							if (tempStartX != -1 && tempStartX2 == -1) {
								if (yTile == tempY && xTile == tempStopX) {
									tempY = -1; tempStartX = -1; tempStopX = -1;
								}
								else if (xTile < tempStartX)
									xTile = tempStartX;
							}
							else if (tempStartX == -1 && tempStartX2 != -1 && tempY2 != -1) {
								if (yTile == tempY2 && xTile == tempStopX2) {
									tempY = -1; tempY2 = -1; tempStartX2 = -1; tempStopX2 = -1;
								}
								else if (yTile != tempY2 && xTile < tempStartX2) {
									xTile = tempStartX2;
								}
								else if (yTile != tempY2) {
									yTile++;
								}
							}
							else if (tempStartX != -1 && tempStartX2 != -1 && tempY2 != -1) {
								if (yTile == tempY2 && xTile == tempStopX2) {
									tempY = -1; tempY2 = -1; tempStartX = -1; tempStopX = -1; tempStartX2 = -1; tempStopX2 = -1;
								}
								else if (xTile < tempStartX && yTile != tempY2)
									xTile = tempStartX;
								else if (xTile != 0 && xTile == 2 && yTile == tempY) {
									yTile++;
									xTile = 0;
								}
							}
							else if (tempStartX != -1 && tempStartX2 != -1) {
								if (yTile == tempY && xTile == tempStopX2) {
									tempY = -1; tempStartX = -1; tempStopX = -1; tempStartX2 = -1; tempStopX2 = -1;
								}
								else if (xTile != 0 && xTile < tempStartX2 && yTile == tempY)
									xTile = tempStartX2;
							}
							//previewItem->setXStartText(64 * 4 + xTile * 64);
							//previewItem->setYStartText(yTile * 64);
							previewItem->setStartX(64 * 4 + xTile * 64);
							previewItem->setStartY(yTile * 64);
							previewItem->addMiscellaneousID("position", (yTile - 1 + palette_Modifier_Right_page * (palette_Modifier_Right_Offset_Max / 64) - palette_Modifier_Right_page) * 4 + xTile, 1);
							//previewHoverItem->setXStartText(64 * 4 + xTile * 64);
							//previewHoverItem->setYStartText(yTile * 64);
							previewHoverItem->setStartX(64 * 4 + xTile * 64);
							previewHoverItem->setStartY(yTile * 64);
							if (++xTile > 3) {
								xTile = 0;
								yTile++;
							}
							previousDoubleSize = false;
						}
						else {
							if (tempStartX2 != -1 || tempStartX != -1) {
								tempStartX2 = -1;
								tempStopX2 = -1;
								tempStartX = -1;
								tempStopX = -1;
							}
							if (xTile == 3) {
								xTile = 0;
								yTile++;
								tempStartX = 2;
								tempStopX = 3;
								if (previousDoubleSize)
									yTile++;
							}
							else if (xTile == 2) {
								if (!previousDoubleSize) {
									tempStartX = 0;
									tempStopX = 1;
									tempStartX2 = 0;
									tempStopX2 = 0;
								}
								else {
									tempStartX2 = 0;
									tempStopX2 = 0;
								}
								tempY2 = yTile + 2;
							}
							else if (xTile == 1) {
								tempStartX = 0;
								tempStopX = 0;
								tempStartX2 = 3;
								tempStopX2 = 3;
							}
							else if (xTile == 0) {
								if (previousDoubleSize) {
									tempStartX2 = 2;
									tempStopX2 = 3;
									tempY2 = yTile + 1;
								}
								else {
									tempStartX = 2;
									tempStopX = 3;
								}
							}
							tempY = yTile + 1;
							//previewItem->setXStartText(64 * 4 + xTile * 64);
							//previewItem->setYStartText(yTile * 64);
							previewItem->setStartX(64 * 4 + xTile * 64);
							previewItem->setStartY(yTile * 64);
							previewItem->addMiscellaneousID("position", (yTile - 1 + palette_Modifier_Right_page * (palette_Modifier_Right_Offset_Max / 64) - palette_Modifier_Right_page) * 4 + xTile, 1);
							//previewHoverItem->setXStartText(64 * 4 + xTile * 64);
							//previewHoverItem->setYStartText(yTile * 64);
							previewHoverItem->setStartX(64 * 4 + xTile * 64);
							previewHoverItem->setStartY(yTile * 64);
							if ((xTile += 2) > 3) {
								xTile = 0;
								yTile += 2;
							}
							previousDoubleSize = true;
						}
					}
					change = true;
					if (!preview->getShow()) {
						preview->toggleShow();
						previewHover->toggleShow();
					}
				}
			}
			else {
				if (preview->getShow()) {
					preview->toggleShow();
					previewHover->toggleShow();
					change = true;
				}
			}
		}
		if (change) generate_Palette_Modifier_Rend_To_Text();

	};
}

/**
* The lambda function that is to be triggered on a scrollbar scroll event.
*
*@param sb, the scrollbar.
*@param rtt, the element containing the texture that is rendered to.
*@param elementWithRTT, the element that is used when drawing to the texture.
*@param offset, the offset value of the rtt.
*@param offset_Max, the max offset before a new page is needed.
*@param page, the current page.
*@param Page_Max, the max amount of pages.
*@param selected_Palette, the selected palette.
*
*@return the generated lambda function.
*/
std::function<void()> get_Palette_Modifier_Scrollbar_Scroll_Lambda(ScrollbarVertical* sb, GUIElement* rttElement, GUIElement* elementforRTT, GUIPanel* panel, RendToText* rtt, double& offset, double& offset_Max, int& page, int& Page_Max, std::string& selected_Palette, std::unordered_map<int, int>& map)
{
	return [&, sb,rttElement, panel, rtt, elementforRTT]()
	{
		bool change = false;
		double texturePositionChange = sb->getChange() * sb->getIncrementSize();
		//std::cout << "Direction: " + std::to_string(scrollbar->getChange()) + "\n";
		for (Palette  p: palettes) {
			if (p.getName().compare(selected_Palette) == 0) {
				if (texturePositionChange != 0) {
					if (Page_Max != 0 && page >= Page_Max && offset > p.getMaxYAsFloat(Page_Max, offset_Max)) {
						if (page > Page_Max) {
							page = Page_Max;
							change = 1;
						}
						offset = p.getMaxYAsFloat(Page_Max, offset_Max);
					}

					offset += texturePositionChange;

					if (offset < 64) {
						if (page > 0) {
							change = true;
							offset = offset_Max - (64 - offset);
							page--;
						}
						else {
							offset = 64;
						}
					}
					else if (offset >= offset_Max) {
						offset = (offset - offset_Max) + 64;
						change = true;
						page++;
					}

				}
				else if (sb->getScrollbarPosition() == 0 && page == 0 && offset != 64) {
					page = 0;
					offset = 64;
					change = 1;
				}
				else if (Page_Max >= 1 && sb->isScrollbarAtMaxPosition() && !(page == Page_Max && int(offset) == int(p.getMaxYAsFloat(Page_Max, offset_Max)))) {
					if (page != Page_Max) {
						page = Page_Max;
						change = 1;
					}
					offset = p.getMaxYAsFloat(Page_Max, offset_Max);
				}

				rttElement->setOffsetY(offset);
				elementforRTT->setRendToTextOffsetY(offset);

				if (change)
					prepare_Palette_Modifier_Rend_To_Text(panel, rtt, rttElement, elementforRTT, p, page, offset_Max, map);
				break;
			}
		}
	};
}

/**
* The lambda function that is to be triggered on a scrollbar scroll event.
*
*@param sb, the scrollbar.
*@param rtt, the element containing the texture that is rendered to.
*@param elementWithRTT, the element that is used when drawing to the texture.
*@param offset, the offset value of the rtt.
*@param offset_Max, the max offset before a new page is needed.
*@param page, the current page.
*@param Page_Max, the max amount of pages.
*@param selected_Palette, the selected palette.
*
*@return the generated lambda function.
*/
void prepare_Palette_Modifier_Rend_To_Text(GUIPanel* panel, RendToText* rtt, GUIElement* tilesRTT, GUIElement* tbgE, Palette& p, int& page, double& offset_Max, std::unordered_map<int, int>& map)
{
	if (ToggleButtonGroup* tbg = dynamic_cast<ToggleButtonGroup*>(tbgE)) {
		tbg->clearAll();

		int maxPerPage = (rtt->getHeight() / 64) - 1;
		int minY = page * (maxPerPage)-(page * (tilesRTT->getHeight() / 64)), maxY = minY + (maxPerPage), count = 0;
		offset_Max = rtt->getHeight() - tilesRTT->getHeight();

		int posPage = ((int)(((page * offset_Max) - (page * 64.0)) / 64) * 4);


		auto yellowSmallSquare = rtt->getVerticesByName("yellowSquareSmall"), yellowBigSquare = rtt->getVerticesByName("yellowSquareBig");
		auto redSmallSquare = rtt->getVerticesByName("redSquareSmall"), redBigSquare = rtt->getVerticesByName("redSquareBig");
		for (int y = 0; y < (rtt->getHeight() / 64); y++) {
			for (int x = 0; x < 4; x++) {
				ToggleButton* tb;
				if (tbgE->getName() == "paletteToggleGroupRight") {
					tb = new ToggleButton("toggle_" + std::to_string(count), nullptr, yellowSmallSquare, redSmallSquare, (x + 4) * 64, (y) * 64, 63, 63, [](int& mousebutton, int& mouseState) {
						generate_Palette_Modifier_Rend_To_Text();
						},
						[](double& x, double& y) {
							generate_Palette_Modifier_Rend_To_Text();
						});
				}
				else {
					tb = new ToggleButton("toggle_" + std::to_string(count), nullptr, yellowSmallSquare, redSmallSquare, x * 64, y * 64, 63, 63, [](int& mousebutton, int& mouseState) {
						generate_Palette_Modifier_Rend_To_Text();
						},
						[](double& x, double& y) {
							generate_Palette_Modifier_Rend_To_Text();
						});
				}
				tb->toggleAlwaysShowBase();
				tbg->addElement(tb);
				count++;
			}
		}

		for (auto& i : p.getPalette()) {
			ToggleButton* toggle = nullptr, * toggleTemp = nullptr;
			int pos = 0;
			if (i.getY() >= minY && i.getY() <= maxY) {
				pos = ((i.getY() - minY) * 4) + i.getX();
				pos += 4;
				toggle = tbg->getElementByName("toggle_" + std::to_string(pos));
			}
			else if (minY > 0 && i.getY() == minY - 1) {
				pos = ((i.getY() + 1 - minY) * 4) + i.getX();
				toggle = tbg->getElementByName("toggle_" + std::to_string(pos));
			}
			if (toggle != nullptr) toggle->addMiscellaneousID("itemID", i.getIDRef());
			if (!i.isDoubleSize()) {
				if (toggle != nullptr) {
					toggle->setButton(rtt->getVerticesByName(itemAtlas.getItemObject(i.getIDRef())->getName()));
				}
			}
			else {
				if (toggle != nullptr) {
					auto obj = itemAtlas.getItemObject(i.getIDRef());
					if (obj) {
						toggle->setButton(rtt->getVerticesByName(obj->getName()));
						toggle->setHoverVertices(rtt->getVerticesByName("yellowSquareBig"));
						toggle->set_Clicked_Vertices(rtt->getVerticesByName("redSquareBig"));
						toggle->setWidth(127);
						toggle->setHeight(127);
						//toggle->addToStartXText(64);
						//toggle->addToStartYText(64);
						toggle->setXDrawOffset(-64);
						toggle->setYDrawOffset(-64);

						toggleTemp = tbg->getElementByName("toggle_" + std::to_string(pos + 1));
						if (toggleTemp != nullptr && toggleTemp->getShow()) toggleTemp->toggleShow();
						toggleTemp = tbg->getElementByName("toggle_" + std::to_string(pos + 4));
						if (toggleTemp != nullptr && toggleTemp->getShow()) toggleTemp->toggleShow();
						toggleTemp = tbg->getElementByName("toggle_" + std::to_string(pos + 5));
						if (toggleTemp != nullptr && toggleTemp->getShow()) toggleTemp->toggleShow();
					}
				}
			}
			auto location = map.find(posPage + pos);
			if (location != map.end()) {
				if (toggle && toggle->getButton()) {
					toggle->toggleClicked();
					(*location).second = i.getIDRef();
				}
			}
		}
		generate_Palette_Modifier_Rend_To_Text();
	}
}

void generate_Palette_Modifier_Rend_To_Text() {
	// first pass
	RendToText* rtt = findByName(renderToTextureContainer, "paletteModifier");
	glBindFramebuffer(GL_FRAMEBUFFER, rtt->getFboID());
	glViewport(0, 0, rtt->getWidth(), rtt->getHeight());
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);


	DrawObjects* temp = new DrawObjects("temp");
	auto preview = paletteModifier->getElementByName("rightSidePreview");
	paletteModifier->getElementByName("emptyTiles_right_group")->get_Draw_Object(temp);
	paletteModifier->getElementByName("emptyTiles_left_group")->get_Draw_Object(temp);
	if (palette_Modifier_Right_Selected_Palette != "") {
		paletteModifier->getElementByName("paletteToggleGroupRight")->get_Draw_Object(temp);
		if (preview->getShow()) {
			preview->get_Draw_Object(temp);
			paletteModifier->getElementByName("rightSidePreviewHover")->get_Draw_Object(temp);
		}
	}
	if (palette_Modifier_Left_Selected_Palette != "") paletteModifier->getElementByName("paletteToggleGroupLeft")->get_Draw_Object(temp);
	double yPos = 0.0;
	for (auto& o : temp->getObjects()) {
		o->renderGUI("");
	}

	delete temp;

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glViewport(0, 0, screenWidth, screenHeight);
	glDisable(GL_DEPTH_TEST);
}
void generate_Vertices_Creation_Rend_To_Text()
{
	
	// first pass
	RendToText* rtt = findByName(renderToTextureContainer, "verticesCreation");
	glBindFramebuffer(GL_FRAMEBUFFER, rtt->getFboID());
	glViewport(0, 0, rtt->getWidth(), rtt->getHeight());
	glClearColor(122 / 255.f, 113 / 255.f, 113 / 255.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);


	DrawObjects* temp = new DrawObjects("temp");
	verticesCreation->getElementByName("selectionAreaGroup")->get_Draw_Object(temp, false);
	verticesCreation->getElementByName("selectionAreaGroup")->get_Draw_Object_Text(temp, false);
	for (auto& o : temp->getObjects()) {
		o->renderGUI("");
	}

	delete temp;

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glViewport(0, 0, screenWidth, screenHeight);
	glDisable(GL_DEPTH_TEST);
	
}
void setup_Vertices_Generation_Selection_Area()
{
	auto groupPtr = verticesCreation->getElementByName("selectionAreaGroup");
	
	if(groupPtr)
		if (GUIGroup* group = dynamic_cast<GUIGroup*>(groupPtr)) {
			group->clear();
			int xStart = 0, yStart = 0, height = 0;
			auto rtt = findByName(renderToTextureContainer, "verticesCreation");
			auto verticesCreatiobSelectionDropDown = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdown"),
				verticesCreatiobSelectionDropDownHover = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdownHover"),
				verticesCreatiobSelectionDropDownClicked = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdownClicked"),
				verticesCreationVerticesSelectionDropdownChildMiddle = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdownChildMiddle"),
				verticesCreationVerticesSelectionDropdownChildMiddleHover = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdownChildMiddleHover"),
				verticesCreationVerticesSelectionDropdownChildMiddleClicked = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdownChildMiddleClicked"),
				verticesCreationVerticesSelectionDropdownChildBottom = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdownChildBottom"),
				verticesCreationVerticesSelectionDropdownChildBottomHover = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdownChildBottomHover"),
				verticesCreationVerticesSelectionDropdownChildBottomClicked = rtt->getVerticesByName("verticesCreationVerticesSelectionDropdownChildBottomClicked");


			for (auto obL : objLibraries) {
				height = 17;
				auto dd = new DropDown(obL->getName(),
					verticesCreatiobSelectionDropDown, verticesCreatiobSelectionDropDownHover, verticesCreatiobSelectionDropDownClicked,
					verticesCreationVerticesSelectionDropdownChildMiddle, verticesCreationVerticesSelectionDropdownChildMiddleHover, verticesCreationVerticesSelectionDropdownChildMiddleClicked,
					verticesCreationVerticesSelectionDropdownChildBottom, verticesCreationVerticesSelectionDropdownChildBottomHover, verticesCreationVerticesSelectionDropdownChildBottomClicked,
					xStart, yStart, 200, height);
				dd->setRenderToText(rtt, true);
				dd->set_Child_X_Offset(10);
				dd->set_Ellipsis(true);
				dd->setCenteredText(false);
				dd->set_Child_Text_Offset_X(25);
				dd->set_Close_After_Select(false);
				dd->set_Size_Based_On_Text(true);
				group->addElement(dd);

				for (auto ob : obL->getObjects()) {
					dd->add(ob->getName(), [](double& x, double& y) {
						verticesCreationUpdateSelectionArea = true;
						},
						[groupPtr, dd, ob](int& mousebutton, int& mouseState) {
							update_Vertices_Creation_Info(ob);

							auto previewRTT = verticesCreation->getElementByName("previewRTT");
							previewRTT->setOffsetX(2048 / 2 - previewRTT->getWidth() / 2);
							previewRTT->setOffsetY(2048 / 2 - previewRTT->getHeight() / 2);
							previewRTT->setUpdate(true);
							verticesCreationPreviewZoom = 1.0;
							generate_Vertices_Creation_Preview_Rend_To_Text();

							for (auto& e : groupPtr->getElements()) {
								if (DropDown* dd = dynamic_cast<DropDown*>(e))
									for (auto& dde : dd->get_Drop_Down_Elements())
										if (dde->getV() == dde->get_Clicked_Vertices() && dde->getName() != ob->getName()) {
											dde->setV(dde->get_Section_Vertices());
											dde->set_Clicked(false);
											dde->get_Draw_Object()->setDraw(false);
										}
							}

							verticesCreationUpdateSelectionArea = true;
						}, true);
				}

				auto tb = dd->getButton();
				tb->setHoverLambda([](double& x, double& y) {
					verticesCreationUpdateSelectionArea = true;
					});
				tb->setClickLambda([dd, groupPtr](int& mousebutton, int& mouseState) {
					if(dd->get_Drop_Down_Elements().size() > 0)
						dd->toggleShowDropDown();
					update_Vertices_Generation_Selection_Area("none");
					});
				tb->setText(obL->getName());
				tb->setRenderToText(rtt, true);
				yStart += height + 1;
			}
		}
		
}

void update_Vertices_Generation_Selection_Area(std::string state = "none")
{
	
	auto groupPtr = verticesCreation->getElementByName("selectionAreaGroup");
	if (groupPtr)
		if (GUIGroup* group = dynamic_cast<GUIGroup*>(groupPtr)) {
			double offsetMax = 0;
			DropDown* previous = nullptr;
			int yStart = 0, rowHeight = 0;
			bool start = true, doUpdate = false, direction = false;
			if (state != "none")
				doUpdate = true;
			if (state == "up")
				direction = true;
			for (auto e : group->getElements()) {
				if (DropDown* dde = dynamic_cast<DropDown*>(e)) {
					if (doUpdate && start) {
						if (direction) {
							if(verticesCreationSelectionAreaPage != 0)
								yStart = dde->getYStart() + (verticesCreationSelectionAreaPageSize - group->getHeight());
						}
						else
							yStart = dde->getYStart() - (verticesCreationSelectionAreaPageSize - group->getHeight());
						start = false;
					}
					if(rowHeight == 0)
						rowHeight = dde->getHeight();
					offsetMax += dde->getHeight();
					if (dde->getShowDropDown())
						offsetMax += dde->get_Drop_Down_Elements().size() * dde->getHeight();
					if (previous && previous->getShowDropDown()) {
						yStart = previous->get_Highest_Child_Y_Position() + 1;
						if(yStart == 0)
							yStart = previous->getYStart() + previous->getHeight();
					}
					else if (previous) {
						yStart = previous->getYStart() + previous->getHeight() + 1;
					}
					dde->change_Position(dde->getXStart(), yStart);
					previous = dde;
				}
			}

			if (GUIElement* rtt = dynamic_cast<GUIElement*>(verticesCreation->getElementByName("selectionAreaRTT"))) {
				auto rttTemp = findByName(renderToTextureContainer, "VerticesCreation");
				if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(verticesCreation->getElementByName("selectionAreaScrollBar"))) {

					double position = verticesCreationSelectionAreaPage * (verticesCreationSelectionAreaPageSize - group->getHeight()) + verticesCreationSelectionAreaOffset;
					//if (verticesCreationSelectionAreaPageChange)
						//position -= group->getHeight();

					if (offsetMax != verticesCreationSelectionAreaOffsetMax) {
						verticesCreationSelectionAreaOffsetMax = offsetMax;
						sb->reset();
						if (offsetMax > rtt->getHeight()) {
							double scrollbarLength = sb->getScrollbarMaxLength() * (((rtt->getHeight() / rowHeight) * rowHeight) / offsetMax);
							//sb->updateScrollbarPosition(((page * offset_Max) + offset - (page * 64)) / sb->getIncrementSize(), false);
							sb->updateScrollbarLengthAndPositionAndIncrementSize(scrollbarLength,
								position,
								offsetMax - group->getHeight() + rowHeight*3);
						}
						else {
							sb->updateScrollbarLengthAndPosition(10000, 0);
						}
					}
				}
			}
			groupPtr->createObjectNoRestriction();
			verticesCreationUpdateSelectionArea = true;
		}
};

/**
* The lambda function that is to be triggered on a scrollbar scroll event.
*
*@param sb, the scrollbar.
*
*@return the generated lambda function.
*/
std::function<void()> get_Vertices_Creation_Selection_Area_Scrollbar_Scroll_Lambda(ScrollbarVertical* sb, GUIElement* rttElement, GUIElement* elementforRTT, GUIPanel* panel, RendToText* rtt, double& offset)
{
	return [&, sb, rttElement, panel, rtt, elementforRTT]()
	{
		
		std::string direction = "none";
		bool change = false;
		double texturePositionChange = sb->getChange() * sb->getIncrementSize();

		if (texturePositionChange != 0) {
			offset += texturePositionChange;

			if (offset <= 0 && verticesCreationSelectionAreaPage > 0) {
				offset += verticesCreationSelectionAreaPageSize - rttElement->getHeight();
				verticesCreationSelectionAreaPage--;
				change = true;
				verticesCreationSelectionAreaPageChange = false;
				direction = "up";
			}
			else if (offset + rttElement->getHeight() >= verticesCreationSelectionAreaPageSize) {
				offset -= (verticesCreationSelectionAreaPageSize - rttElement->getHeight());
				verticesCreationSelectionAreaPage++;
				change = true;
				verticesCreationSelectionAreaPageChange = true;
				direction = "down";
			}

		} else if (sb->getScrollbarPosition() == 0) {
			offset = 0;
			verticesCreationSelectionAreaPage = 0;
		}
		else if (sb->isScrollbarAtMaxPosition()) {
			//offset = verticesCreationSelectionAreaOffsetMax - rttElement->getHeight() - verticesCreationSelectionAreaPage * (verticesCreationSelectionAreaPageSize - rttElement->getHeight()) + 17 * (verticesCreationSelectionAreaPage + 1);
			//verticesCreationSelectionAreaPage = verticesCreationSelectionAreaOffsetMax / (verticesCreationSelectionAreaPageSize - rttElement->getHeight());
		}
		
		//leftPanel->getElementByName("tilesRTT")->setOffsetY(offset);
		rttElement->setOffsetY(offset);
		elementforRTT->setRendToTextOffsetY(offset);
		
		rttElement->setUpdate(true);

		if (change)
			update_Vertices_Generation_Selection_Area(direction);
	};
}

void generate_Vertices_Creation_Preview_Rend_To_Text()
{
	RendToText* rtt = findByName(renderToTextureContainer, "verticesCreationPreview");
	glBindFramebuffer(GL_FRAMEBUFFER, rtt->getFboID());
	glViewport(0, 0, rtt->getWidth(), rtt->getHeight());
	glClearColor(122 / 255.f, 113 / 255.f, 113 / 255.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	if (selectedObject) {
		Object* tempObj = getObjectFromLibrary(rendToTextLibrary.getLibrary(2048, 2048), selectedObject->getLib()->getName(), selectedObject->getID());
		if (tempObj) {
			DrawObjects* temp = new DrawObjects("temp");
			double x = (1024 - (tempObj->getVertices()->getWidth() * verticesCreationPreviewZoom) / 2) / (double(rtt->getWidth()) / 2),
				y = (1024 - (tempObj->getVertices()->getHeight() * verticesCreationPreviewZoom) / 2) / (double(rtt->getHeight()) / 2);
			x = round(x * 1000) / 1000.;
			y = round(y * 1000) / 1000.;
			auto o = new DrawObject(x, y, tempObj->getVertices()->getID(), tempObj->getVertices()->getVAO(), tempObj->getVertices()->getTextPos());

			o->setScale(verticesCreationPreviewZoom);
			temp->addObject(o);

			for (auto& o : temp->getObjects()) {
				o->renderGUI("");
			}

			delete temp;
			delete o;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glViewport(0, 0, screenWidth, screenHeight);
	glDisable(GL_DEPTH_TEST);
}

void update_Vertices_Creation_Info(Object* ob)
{
	std::string name = "", xStart = "", yStart = "", width = "", height = "", textureName = "";
	if (!selectedObject || selectedObject != ob) {
		selectedObject = ob;
		name = selectedObject->getName();
		xStart = std::to_string(selectedObject->getVertices()->getXStartText());
		yStart = std::to_string(selectedObject->getVertices()->getYStartText());
		width = std::to_string(selectedObject->getVertices()->getWidthText());
		height = std::to_string(selectedObject->getVertices()->getHeightText());
		textureName = paths[selectedObject->getVertices()->getTextPos()];
	}
	else if (selectedObject = ob) {
		selectedObject = nullptr;
	}

	TextField* tempElement = dynamic_cast<TextField*>(verticesCreation->getElementByName("verticesPreviewTextFieldName"));
	tempElement->setText(name);
	//tempElement->change_Width(Get_Text_Size(tempElement->getText(), tempElement->get_Text_Type()) + tempElement->getTextStartXOffset() + 3);
	tempElement->setUpdate(true);


	tempElement = dynamic_cast<TextField*>(verticesCreation->getElementByName("verticesPreviewTextFieldXStart"));
	tempElement->setText(xStart);
	tempElement->setUpdate(true);

	tempElement = dynamic_cast<TextField*>(verticesCreation->getElementByName("verticesPreviewTextFieldYStart"));
	tempElement->setText(yStart);
	tempElement->setUpdate(true);

	tempElement = dynamic_cast<TextField*>(verticesCreation->getElementByName("verticesPreviewTextFieldWidth"));
	tempElement->setText(width);
	tempElement->setUpdate(true);

	tempElement = dynamic_cast<TextField*>(verticesCreation->getElementByName("verticesPreviewTextFieldHeight"));
	tempElement->setText(height);
	tempElement->setUpdate(true);

	DropDown* tempElement2 = dynamic_cast<DropDown*>(verticesCreation->getElementByName("verticesPreviewDropDownTextureName"));
	tempElement2->getButton()->setText(textureName);
	tempElement2->getButton()->resetToggle();
	if (tempElement2->getShowDropDown())
		tempElement2->toggleShowDropDown();

}
