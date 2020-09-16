#include "ElementCreator.h"
#include "Palette.h"
#pragma warning(disable: 4244)

void generate_GUI_Bottom_Bar()
{
	GUIPanels.push_back(bottomBar);
	bottomBar->setDimensions(0, screenHeightPixels - 30, screenWidth, 30);
	bottomBar->setCheckIfOutside(true);
	bottomBar->clear();
	bottomBar->toggleShow();
	
	Vertices* v = getGUIObjectFromLibrary("bottomBarBasic")->getVertices();
	v->changeSize(screenWidth, 30);
	bottomBar->addElement(new GUIElement("leftCorner", v, 0, screenHeightPixels - 30, screenWidth, 30, ""));

	GUIGroup* labelGroup = new GUIGroup("labelGroup", 64, screenHeightPixels - 30, 64 * 3, 30 * 3);
	bottomBar->addElement(labelGroup);
	//X
	auto label = new GUILabel("x", getGUIObjectFromLibrary("bottomBarLabel")->getVertices(), 64, screenHeightPixels - 30, 64, 30, xText);
	label->setTextStartOffset(6);
	labelGroup->addElement(label);
	//Y
	label = new GUILabel("y", getGUIObjectFromLibrary("bottomBarLabel")->getVertices(), 64 * 2, screenHeightPixels - 30, 64, 30, yText);
	label->setTextStartOffset(6);
	labelGroup->addElement(label);
	//Z
	label = new GUILabel("z", getGUIObjectFromLibrary("bottomBarLabel")->getVertices(), 64 * 3, screenHeightPixels - 30, 64, 30, zText);
	label->setTextStartOffset(6);
	labelGroup->addElement(label);

	ToggleButtonGroup* group = new ToggleButtonGroup("toggles", 64 * 5, screenHeightPixels - 30, screenWidthPixels, 30);
	bottomBar->addElement(group);

	ToggleButton* tb = new ToggleButton("eraser", getGUIObjectFromLibrary("bottomBarEraser")->getVertices(), getGUIObjectFromLibrary("bottomBarEraserHover")->getVertices(), getGUIObjectFromLibrary("bottomBarEraserPressed")->getVertices(), 64 * 5, screenHeightPixels - 30, 30, 30, [&](int& mouseState) { eraseToggle = !eraseToggle; if (!eraseToggle) getObjectByName(objects, "GUI_Preview_Tiles_")->clearObjects(); });
	tb->toggleCleanReset();
	group->addElement(tb);
	tb = new ToggleButton("destroyer", getGUIObjectFromLibrary("bottomBarDestroyer")->getVertices(), getGUIObjectFromLibrary("bottomBarDestroyerHover")->getVertices(), getGUIObjectFromLibrary("bottomBarDestroyerPressed")->getVertices(), 64 * 5 + 40, screenHeightPixels - 30, 30, 30, [&](int& mouseState) { destroyToggle = !destroyToggle; if (!destroyToggle) getObjectByName(objects, "GUI_Preview_Tiles_")->clearObjects(); });
	tb->toggleCleanReset();
	group->addElement(tb);
	tb = new ToggleButton("tileDestroyer", getGUIObjectFromLibrary("bottomBarTileDestroyer")->getVertices(), getGUIObjectFromLibrary("bottomBarTileDestroyerHover")->getVertices(), getGUIObjectFromLibrary("bottomBarTileDestroyerPressed")->getVertices(), 64 * 5 + 80, screenHeightPixels - 30, 30, 30, [&](int& mouseState) {destroyTileToggle = !destroyTileToggle; if (!destroyTileToggle) getObjectByName(objects, "GUI_Preview_Tiles_")->clearObjects(); });
	tb->toggleCleanReset();
	group->addElement(tb);
	tb = new ToggleButton("cut", getGUIObjectFromLibrary("bottomBarCut")->getVertices(), getGUIObjectFromLibrary("bottomBarCutHover")->getVertices(), getGUIObjectFromLibrary("bottomBarCutPressed")->getVertices(), 64 * 5 + 120, screenHeightPixels - 30, 30, 30, [&](int& mouseState) {cutToggle = !cutToggle; if (!cutToggle) getObjectByName(objects, "GUI_Preview_Tiles_")->clearObjects(); });
	tb->toggleCleanReset();
	group->addElement(tb);
	tb = new ToggleButton("copy", getGUIObjectFromLibrary("bottomBarCopy")->getVertices(), getGUIObjectFromLibrary("bottomBarCopyHover")->getVertices(), getGUIObjectFromLibrary("bottomBarCopyPressed")->getVertices(), 64 * 5 + 160, screenHeightPixels - 30, 30, 30, [&](int& mouseState) {copyToggle = !copyToggle; if (!copyToggle) getObjectByName(objects, "GUI_Preview_Tiles_")->clearObjects(); });
	tb->toggleCleanReset();
	group->addElement(tb);
	

	bottomBar->createObjects();
}

void generate_GUI_Bottom_Bar_text(DrawObjects *ob, std::string xText, std::string yText, std::string zText)
{
	ob->clearObjects();
	// X
	double x = xCameraPos + ((64 * 2 + 16) / (double(screenWidthPixels) / 2)), y = ((30 + 11) / (double(screenHeightPixels) / 2));
	generate_GUI_Text(ob, x, y, xText);
	// Y
	x = ((64 * 3 + 16) / (double(screenWidthPixels) / 2)), y = 2.0 + ((30 + 11) / (double(screenHeightPixels) / 2));
	generate_GUI_Text(ob, x, y, yText);
	// Z
	x = ((64 * 4 + 16) / (double(screenWidthPixels) / 2)), y = 2.0 + ((30 + 11) / (double(screenHeightPixels) / 2));
	generate_GUI_Text(ob, x, y, zText);
}

void generate_GUI_Text(DrawObjects *ob, double x, double y, std::string text)
{
	/*int character = 0;
	float offset = 0.0f;
	for (auto& letter : text) {
		character = letter;
		auto& letterInformation = letterLibrary.getLetterInformation(std::string(1, letter) + "Black");
		if (letterInformation.size() == 3)
			ob->addObject(new DrawObject(x + offset, y, letterInformation[2], letterInformation[0], letterInformation[1]));
		offset += ((OFFSET_PER_CHARACTER[character]) / (double(screenWidthPixels) / 2));
	}*/
}

void generate_GUI_Text(std::vector<DrawObject*>& vec, double x, double y, std::string text)
{
	int character = 0;
	float offset = 0.0f;
	for (auto& letter : text) {
		character = letter;
		//vec.push_back(new DrawObject(x + offset, y, character, vh->getVAO(), vh->getTextureID()));
		//offset += ((OFFSET_PER_CHARACTER[character]) / (double(screenWidthPixels) / 2));
	}
}

void generate_GUI_Text(DrawObjects* ob, double x, double y, int textStartOffset, int rowLength, std::string text, string textType)
{
	x += (textStartOffset / (double(screenWidthPixels) / 2));
	int character = 0, width = 0;
	float offset = 0.0f, maxOffset = (rowLength / (double(screenWidthPixels) / 2));
	for (auto& letter : text) {
		character = letter;
		auto& letterInformation = letterLibrary.getLetterInformation(std::string(1, letter) + textType);
		if (letterInformation.size() == 6) {
			if (width == 0)
				width = letterInformation[5];
			offset -= ((letterInformation[3]) / (double(screenWidthPixels) / 2));
			ob->addObject(new DrawObject(x + offset, y, letterInformation[2], letterInformation[0], letterInformation[1]));
			offset += ((letterInformation[4] + 1) / (double(screenWidthPixels) / 2));
		}
		if (offset > maxOffset) {
			offset = 0.0f;
			y += ((width + 10) / (double(screenWidthPixels) / 2));
		}
	}
}

void generate_GUI_Left_Panel(size_t displayState)
{
	GUIPanels.push_back(leftPanel);

	int width = 295, height = screenHeightPixels - 30, xStart = screenWidthPixels - width, yStart = 0;
	leftPanel->setDimensions(xStart, yStart, width, height);
	leftPanel->setCheckIfOutside(true);
	leftPanel->clear();
	leftPanel->toggleShow();
	
	Vertices* v = getGUIObjectFromLibrary("leftPanelBackground")->getVertices();
	v->changeSize(width, height);
	leftPanel->addElement(new GUIElement("leftPanelBackground", v, xStart, yStart, width, height, ""));

	
	v = getGUIObjectFromLibrary("leftPanelTileArea")->getVertices();
	v->changeSize(64*4, ((height - 54) / 64) * 64);
	leftPanel->addElement(new GUIElement("leftPanelTileArea", v, xStart + 10, yStart + 54, 64 * 4, ((height - 54) / 64) * 64, ""));
	

	auto rtt = findByName(renderToTextureContainer, "leftPanelTiles");

	GUIGroup* groupLeft = new GUIGroup("emptyTiles_group", 0, 0, 64 * 4, 64 * 10);
	leftPanel->addElement(groupLeft);
	groupLeft->setRenderToText(rtt, true);
	// Empty tiles left
	for (int i = 0; i < (rtt->getHeight() / 64); i++) {
		for (int j = 0; j < 4; j++) {
			groupLeft->addElement(new GUIElement("emptyTile_" + std::to_string(i * 4 + j), rtt->getVerticesByName("emptyTileMarker"), 64 * j, 64 * i, 64, 64, ""));
		}
	}
	
	
	// Left items selection render to texture
	auto text = getGUIObjectFromLibrary("leftPanelRendToText")->getVertices();
	text->setWidthText(64*4);
	text->setHeightText(((height - 54) / 64) * 64);
	float s = (std::fmod(screenHeight / 64.0f, 1) * 64) / 2;
	text->setYStartText(s + 2);
	text->setTextureSizeWidth(rtt->getWidth());
	text->setTextureSizeHeight(rtt->getHeight());
	text->setWidth(rtt->getWidth());
	text->setHeight(rtt->getHeight());
	ScaleableElement* tilesRTT = new ScaleableElement("tilesRTT", xStart + 10, yStart + 54, 64 * 4, ((height - 54) / 64) * 64, text);
	tilesRTT->setRenderToText(findByName(renderToTextureContainer, "leftPanelTiles"));
	tilesRTT->setOffsetY(left_panel_Offset);
	leftPanel->addElement(tilesRTT);
	removeVertices(text);
	

	
	ToggleButtonGroup* tbg = new ToggleButtonGroup("paletteToggleGroup", xStart + 10, yStart + 54, 64 * 4, ((height - 54) / 64) * 64);
	tbg->setScrollLambda([&](double& v) {
		if (GUIElement * rtt = dynamic_cast<GUIElement*>(leftPanel->getElementByName("tilesRTT"))) {
			if (rtt->getHover() && left_panel_Selected_Palette != "") {

				left_panel_Offset -= (v) * 15;

				bool change = false;

				if (left_panel_Offset < 64) {
					if (left_panel_page > 0) {
						change = true;
						left_panel_Offset = left_panel_Offset_Max - (64 - left_panel_Offset);
						left_panel_page--;
					}
					else {
						left_panel_Offset = 64;
					}
				}
				else if (left_panel_Offset >= left_panel_Offset_Max) {
					left_panel_Offset = (left_panel_Offset - left_panel_Offset_Max) + 64;
					change = true;
					left_panel_page++;
				}


				for (Palette p : palettes) {
					if (p.getName().compare(left_panel_Selected_Palette) == 0) {

						if (ScrollbarVertical * sb = dynamic_cast<ScrollbarVertical*>(leftPanel->getElementByName("scrollbar"))) {
							if (left_panel_page == 0 && left_panel_Offset == 64.0)
								sb->updateScrollbarPosition(0, false);
							else {
								sb->updateScrollbarPosition(((left_panel_page * left_panel_Offset_Max) + left_panel_Offset - (left_panel_page * 64)) / sb->getIncrementSize(), false);
							}
						}

						if (change)
							generate_Left_Panel_Tiles(p);

						RendToText* rttTemp = findByName(renderToTextureContainer, "leftPanelTiles");

						rtt->setOffsetY(left_panel_Offset - rtt->getV()->getYStartText());

						if (ToggleButtonGroup * tgb = dynamic_cast<ToggleButtonGroup*>(leftPanel->getElementByName("paletteToggleGroup")))
							tgb->setRendToTextOffsetY(left_panel_Offset);
						break;
					}
				}
			}

			//leftPanel->reCreateObjects(getObjectByName(objects, "GUI_LeftPanel_"));
		}
		});
	tbg->setClickLambda([](int& mouseState) {
		if (mouseState == 2) {
			if (ToggleButtonGroup * tgb = dynamic_cast<ToggleButtonGroup*>(leftPanel->getElementByName("paletteToggleGroup")))
				tgb->resetAll();
			generate_Left_Panel_Rend_To_Text();
		}
		else if (mouseState == 1) {
			std::vector<ToggleButton*> tb = {};
			if (ToggleButtonGroup * tbg = dynamic_cast<ToggleButtonGroup*>(leftPanel->getElementByName("paletteToggleGroup"))) {
				tb = tbg->getElementsByClicked(true);
				if (tb.size() == 0) {
					thingsToDraw.clear();
					selectedItemId = PaletteItem(-1, false, -1, -1);
					getObjectByName(objects, "GUI_Preview_Tiles_")->clearObjects();
				}
				else
					for (auto& tb : tb) {
						if (tb != nullptr && tb->getButton() != nullptr) {
							for (Palette p : palettes)
								if (p.getName().compare(left_panel_Selected_Palette) == 0)
									selectedItemId = p.getItemFromID(findByName(tb->getMiscellaneousIDsList(), "itemID"));
						}
					}
			}
		}
		});
	tbg->setHoverLambda([tbg](double& x, double& y) {
		int xStart = (int)((x / 64)) * 64, yStart = (int)(y / 64) * 64;
		if ((xStart >= 0 && xStart < 256) && (yStart >= 0 && yStart < 1024)) {
			if ((lControl || lShift) && !tbg->getDoNotResetAfterToggle())
				tbg->setDoNotResetAfterToggle(true);
			else if (!(lControl || lShift) && tbg->getDoNotResetAfterToggle())
				tbg->setDoNotResetAfterToggle(false);
		}
		});
	tbg->setCheckOutside();
	tbg->setRenderToText(findByName(renderToTextureContainer, "leftPanelTiles"), true);
	leftPanel->addElement(tbg);

	DropDown* d = new DropDown("itemsSelection", getGUIObjectFromLibrary("paletteModifierDropDownElement")->getVertices(), nullptr, nullptr, getGUIObjectFromLibrary("paletteModifierDropDownElementHover")->getVertices(), xStart + 10, yStart + 30, 102, 24);
	d->setPadding(1);
	for (auto& i : palettes) {
		d->add(i.getName(), [&, d]() {
			left_panel_Selected_Palette = i.getName();
			left_panel_Offset = 64;
			left_panel_page = 0;

			auto tb = d->getButton();
			tb->setText(i.getName());
			tb->resetToggle();

			generate_Left_Panel_Tiles(i);
			

			RendToText* rttTemp = findByName(renderToTextureContainer, "leftPanelTiles");
			if (GUIElement * rtt = dynamic_cast<GUIElement*>(leftPanel->getElementByName("tilesRTT"))) {
				rtt->setOffsetY(left_panel_Offset);

				if (ToggleButtonGroup* tgb = dynamic_cast<ToggleButtonGroup*>(leftPanel->getElementByName("paletteToggleGroup"))) {
					tgb->setRendToTextOffsetY(left_panel_Offset);
				}

				if (ScrollbarVertical * sb = dynamic_cast<ScrollbarVertical*>(leftPanel->getElementByName("scrollbar"))) {
					auto paletteItems = i.getPalette();
					if (paletteItems.size() > 0) {
						auto y = paletteItems.at(paletteItems.size() - 1).getY();
						left_panel_maxPage = y * 64.0 / (left_panel_Offset_Max);
						double maxPixels = 64.0 * y,
							scrollbarLength = sb->getScrollbarMaxLength() * (((rtt->getHeight() / 64.0) * 64) / maxPixels);
						sb->updateScrollbarLengthAndPositionAndIncrementSize(scrollbarLength, 0, y * 64);
					}
					else {
						sb->updateScrollbarLengthAndPosition(10000, 0);
					}

				}
			}
			});
	}
	leftPanel->addElement(d);

	
	{
		//Left scrollbar
		int scrollbarXStart = xStart + 10 + tilesRTT->getWidth(), scrollbarYStart = yStart + 54;
		auto buttonUp = new Button("up",
			getGUIObjectFromLibrary("scrollbar1ButtonUp")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonUpHover")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonUpClicked")->getVertices(),
			scrollbarXStart, scrollbarYStart, 19, 10, "", [](int& mouseState) {});
		auto buttonDown = new Button("down",
			getGUIObjectFromLibrary("scrollbar1ButtonDown")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonDownHover")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonDownClicked")->getVertices(),
			scrollbarXStart, scrollbarYStart, 19, 10, "", [](int& mouseState) {});
		auto scrollbar = new ScrollbarVertical("scrollbar", scrollbarXStart, scrollbarYStart, 19, tilesRTT->getHeight(), buttonUp, buttonDown,
			getGUIObjectFromLibrary("scrollbar1Background")->getVertices(),
			nullptr,
			nullptr,
			getGUIObjectFromLibrary("scrollbar1Bar")->getVertices(), getGUIObjectFromLibrary("scrollbar1BarHover")->getVertices(), getGUIObjectFromLibrary("scrollbar1BarClicked")->getVertices());
		scrollbar->setLambda([scrollbar]()
			{
				bool change = false;
				double texturePositionChange = scrollbar->getChange() * scrollbar->getIncrementSize();
				//std::cout << "Direction: " + std::to_string(scrollbar->getChange()) + "\n";
				for (Palette p : palettes) {
					if (p.getName().compare(left_panel_Selected_Palette) == 0) {
						if (texturePositionChange != 0) {
							if (left_panel_page >= left_panel_maxPage && left_panel_Offset > p.getMaxYAsFloat(left_panel_maxPage, left_panel_Offset_Max)) {
								if (left_panel_page > left_panel_maxPage) {
									left_panel_page = left_panel_maxPage;
									change = 1;
								}
								left_panel_Offset = p.getMaxYAsFloat(left_panel_maxPage, left_panel_Offset_Max);
							}

							left_panel_Offset += texturePositionChange;

							if (left_panel_Offset < 64) {
								if (left_panel_page > 0) {
									change = true;
									left_panel_Offset = left_panel_Offset_Max - (64 - left_panel_Offset);
									left_panel_page--;
								}
								else {
									left_panel_Offset = 64;
								}
							}
							else if (left_panel_Offset >= left_panel_Offset_Max) {
								left_panel_Offset = (left_panel_Offset - left_panel_Offset_Max) + 64;
								change = true;
								left_panel_page++;
							}

						}
						else if (scrollbar->getScrollbarPosition() == 0 && left_panel_page == 0 && left_panel_Offset != 64) {
							left_panel_page = 0;
							left_panel_Offset = 64;
							change = 0;
						}
						else if (left_panel_maxPage >= 1 && scrollbar->isScrollbarAtMaxPosition() && !(left_panel_page == left_panel_maxPage && int(left_panel_Offset) == int(p.getMaxYAsFloat(left_panel_maxPage, left_panel_Offset_Max)))) {
							if (left_panel_page != left_panel_maxPage) {
								left_panel_page = left_panel_maxPage;
								change = 1;
							}
							left_panel_Offset = p.getMaxYAsFloat(left_panel_maxPage, left_panel_Offset_Max);
						}
						if (change)
							generate_Left_Panel_Tiles(p);
						break;
					}
				}

				RendToText* rttTemp = findByName(renderToTextureContainer, "leftPanelTiles");
				if (GUIElement * rtt = dynamic_cast<GUIElement*>(leftPanel->getElementByName("tilesRTT")))
					rtt->setOffsetY(left_panel_Offset - rtt->getV()->getYStartText());
				if (ToggleButtonGroup * tgb = dynamic_cast<ToggleButtonGroup*>(leftPanel->getElementByName("paletteToggleGroup")))
					tgb->setRendToTextOffsetY(left_panel_Offset);
			});
		//scrollbar->setTestOnClick(true);
		leftPanel->addElement(scrollbar);
	}
	

	leftPanel->createObjects();	
}

void generate_GUI_Left_Panel_Text_(DrawObjects *ob) {
	// Create the text for tile palette selection area
	if (paletteID != -1) {
		ob->clearObjects();
		double x = ((screenWidthPixels - 125) / (double(screenWidthPixels) / 2)), y = 0.0 + ((startDropDown + 75 + 36 * -1) / (double(screenHeightPixels) / 2));
		generate_GUI_Text(ob, x, y, palettes.at(paletteID).getName());
	}
}

void generate_GUI_Preview_Tiles()
{
	auto ob = getObjectByName(objects, "GUI_Preview_Tiles_");
	ob->clearObjects();
	int increment = 64;
	auto itemObject = itemAtlas.getItemObject(selectedItemId.getID());
	for (auto& item : thingsToDraw) {
		if (item.getId() < 0) {
			if (eraseToggle) itemObject = getGUIObjectFromLibrary("eraseTile");
			if (destroyToggle) itemObject = getGUIObjectFromLibrary("destroyTile");
		}
		else {
			itemObject = itemAtlas.getItemObject(item.getId());
		}
		if(itemObject)
			ob->addObject(new DrawObject(((increment * item.getX()) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * item.getY()) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), itemObject->getID(), itemObject->getVertices()->getVAO(), itemObject->getVertices()->getTextPos()));

	}
	if (eraseToggle || destroyToggle || destroyTileToggle || copyToggle || cutToggle) {
		if (eraseToggle) itemObject = getGUIObjectFromLibrary("eraseTile");
		if (destroyToggle) itemObject = getGUIObjectFromLibrary("destroyTile");
	}
	if (copyBuffer.size() == 0) {
		for (int yPos = y - brush; yPos <= y + brush; yPos++) {
			for (int xPos = x - brush; xPos <= x + brush; xPos++) {
				if (itemObject)
					ob->addObject(new DrawObject( ((increment * xPos) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * yPos) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), itemObject->getID(), itemObject->getVertices()->getVAO(),itemObject->getVertices()->getTextPos()) );
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
				//ob.addObject(new DrawObject(((increment * ((i.first.getX() - smallestX - size_t(differenceX / 2)) + x)) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * ((i.first.getY() - smallestY - size_t(differenceY / 2)) + y)) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), id, vh->getVAO(), vh->getTextureID()));
				for (auto& item : i.second->getAllItems()) {
					//ob.addObject(new DrawObject(((increment * ((i.first.getX() - smallestX - size_t(differenceX / 2)) + x)) / (double(screenWidthPixels) / 2)) - (xCameraPos - 1.0f), 0.0 + ((increment * ((i.first.getY() - smallestY - size_t(differenceY / 2)) + y)) / (double(screenHeightPixels) / 2)) + (yCameraPos + 1.0f), id % 1024, vh->getVAO(), vh->getTextureID()));
				}
			}
		}
	}

}

void generate_GUI_Item_Info_Panel() {
	itemInfo->clearObjects();
	/*
	if (itemInfoWindow) {
		int startX = ((screenWidth - 1000 - 267) / 2), startY = ((screenHeight - 30 - 700) / 2);
		itemInfo->setDimensions(startX, startY, 1000, 700);
		itemInfo->clear();
		bool doubleSize = false;

		//Main Panel
		itemInfo->addElement(new GUIElement("MainPanel", findByName(verticesContainer, "itemInfoPanelID"), startX, startY, 1000, 700, ""));

		if (itemInfoCurrentPage == 0) {
			bool x124 = false;
			std::string s = getTextFromID((itemInfoTile->getID()), x124);
			// Icon
			if(!x124)
				itemInfo->addElement(new GUIElement("icon", findByName(verticesContainer64xTiles, s), (startX + 468), (startY + 10), 64, 64, ""));
			else
				itemInfo->addElement(new GUIElement("icon", findByName(verticesContainer128xTiles, s), (startX + 468), (startY + 10), 64, 64, ""));
		}
		else {
			VertecesHandler* vhT = nullptr;
			getVertecesHandlerFromID(vhT, itemInfoTile->getAllItems().at(itemInfoCurrentPage - 1)->getID(), doubleSize);
			bool x124 = false;
			std::string s = getTextFromID((itemInfoTile->getAllItems().at(itemInfoCurrentPage - 1)->getID()), x124);
			// Icon
			if (!x124)
				itemInfo->addElement(new GUIElement("icon", findByName(verticesContainer64xTiles, s), (startX + 468), (startY + 10), 64, 64, ""));
			else
				itemInfo->addElement(new GUIElement("icon", findByName(verticesContainer128xTiles, s), (startX + 468), (startY + 10), 64, 64, ""));
		}

		// Right arrow
		if (itemInfoCurrentPage != itemInfoMaxPage) {
			auto lambda = ([&](int& mouseState) {
				if (itemInfoCurrentPage < itemInfoMaxPage) {
					itemInfoSubPage = 0;
					itemInfoCurrentPage++;
					updateItemInfo = true;
				}
			});
			itemInfo->addElement(new Button("RightArrow", findByName(verticesContainer, "itemInfoRightArrowID"), findByName(verticesContainer, "itemInfoRightArrowHoverID"), findByName(verticesContainer, "itemInfoRightArrowPressedID"), (startX + 547 + (40 * doubleSize)), (startY + 19), 30, 43, "", lambda));
		}

		// Left arrow
		if (itemInfoCurrentPage != 0) {
			auto lambda = ([&](int& mouseState) {
				if (itemInfoCurrentPage > 0) {
					itemInfoSubPage = 0;
					itemInfoCurrentPage--;
					updateItemInfo = true;
				}
			});
			itemInfo->addElement(new Button("LeftArrow", findByName(verticesContainer, "itemInfoLeftArrowID"), findByName(verticesContainer, "itemInfoLeftArrowHoverID"), findByName(verticesContainer, "itemInfoLeftArrowPressedID"), (startX + 423 - (40 * doubleSize)), (startY + 19), 30, 43, "", lambda));
		}

		if (itemInfoCurrentPage == 0) {
			VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

			std::string text = "id: " + std::to_string(itemInfoTile->getID());
			// First row text fields
			for (int i = 0; i < 4; i++) {
				if (i == 1) text = "uid: " + std::to_string(itemInfoTile->getUID());
				else if (i == 2) text = "zone: " + std::to_string(itemInfoTile->getZone());
				else if (i == 3) text = "speed: " + std::to_string(itemInfoTile->getSpeed());

				itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + 200), 150, 40, text));
				itemInfo->addElement(new GUIElement("FirstRowTextInput_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + 240), 150, 40, ""));

				if (i == 1) {
					itemInfo->addElement(new Button("FirstRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + 280), 150, 26, "", [](int& mouseState) {}));
				}
				else if (i == 2) {
					itemInfo->addElement(new Button("FirstRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + 280), 150, 26, "", [](int& mouseState) {}));
				}
				else if (i == 3) {
					itemInfo->addElement(new Button("FirstRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + 280), 150, 26, "", [](int& mouseState) {}));
				}

			}

			text = "name: " + itemInfoTile->getName();
			// Second row text fields
			for (int i = 0; i < 4; i++) {
				if (i == 1) {
					text = "article: " + itemInfoTile->getArticle();
				}
				else if (i == 2) { 
					text = "description: " + itemInfoTile->getDescription(); 
				}
				else if (i == 3) {
					text = "blockPathfinding: " + std::to_string(itemInfoTile->getBlockPathfind());
				}

				itemInfo->addElement(new GUIElement("SecondRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextAreaID"), (startX + 50 + 250 * i), (startY + 400), 150, 40, text));

				if (i == 0) {
					itemInfo->addElement(new GUIElement("SecondRowTextInput_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + 510), 150, 40, ""));
					itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + 550), 150, 26, "", [](int& mouseState) {}));
				}
				else if (i == 1) {
					itemInfo->addElement(new GUIElement("SecondRowTextInput_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + 440), 150, 40, ""));
					itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + 480), 150, 26, "", [](int& mouseState) {}));
				}
				else if (i == 2) {
					itemInfo->addElement(new GUIElement("SecondRowTextInput_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + 510), 150, 40, ""));
					itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + 550), 150, 26, "", [](int& mouseState) {}));
				}
				else if (i == 3) {
					itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + 510), 150, 26, "", [&](int& mouseState) { itemInfoTile->setBlockPathfind(!itemInfoTile->getBlockPathfind()); itemInfo->getElementByName("SecondRowText_3")->setText("blockPathfinding: " + std::to_string(itemInfoTile->getBlockPathfind())); }));
				}
				// Text
				//generate_GUI_Text(ob, vhText, ((startX + 125 + 250 * i) / (double(screenWidthPixels) / 2)), 0.0 + ((startY + 475) / (double(screenHeightPixels) / 2)), text, 114);
			}
		}
		else {
			auto& item = itemInfoTile->getAllItems().at(itemInfoCurrentPage - 1);
			VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

			if (itemInfoSubPage == 0) {
				int yFirst = 125, offset = 40;

				std::string text = "id: " + std::to_string(item->getBlockProjectile());
				// First row: booleans
				for (int i = 0; i < 2; i++) {
					if (i == 1) text = "uid: " + std::to_string(item->getBlockPathfind());

					itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

					if (i == 1) {
						itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
						itemInfo->addElement(new Button("FirstRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", [](int& mouseState) {}));
					}

				}

				yFirst += offset * 3;
				text = "name: " + item->getName();
				// Second row: text areas
				for (int i = 0; i < 4; i++) {
					if (i == 1) text = "article: " + item->getArticle();
					else if (i == 2) text = "description: " + item->getDescription();
					else if (i == 3) text = "type: " + item->getType();

					itemInfo->addElement(new GUIElement("SecondRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextAreaID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

					if (i == 0) {
						itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2.75), 150, 40, ">"));
						itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 3.75), 150, 26, "", [&](int& mouseState) {item->setBlockProjectile(!item->getBlockProjectile()); itemInfo->getElementByName("ThirdRowText_0")->setText("blockProjectile: " + std::to_string(item->getBlockProjectile())); }));
					}
					else if (i == 1) {
						itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2.75), 150, 40, ">"));
						itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 3.75), 150, 26, "", [&](int& mouseState) {item->setBlockPathfind(!item->getBlockPathfind()); itemInfo->getElementByName("ThirdRowText_1")->setText("blockPathfind: " + std::to_string(item->getBlockPathfind())); }));
					}
					else if (i == 2) {
						itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2.75), 150, 40, ">"));
						itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 3.75), 150, 26, "", [&](int& mouseState) {item->setBlockObject(!item->getBlockObject()); itemInfo->getElementByName("ThirdRowText_2")->setText("blockObject: " + std::to_string(item->getBlockObject())); }));
					}

				}
				yFirst += offset * 4.75;
				text = "blockProjectile: " + std::to_string(item->getBlockProjectile());
				// Third row: booleans
				for (int i = 0; i < 4; i++) {
					if (i == 1) text = "blockPathfind: " + std::to_string(item->getBlockPathfind());
					else if (i == 2) text = "blockObject: " + std::to_string(item->getBlockObject());
					else if (i == 3) text = "moveable: " + std::to_string(item->getMoveable());

					itemInfo->addElement(new GUIElement("ThirdRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

					if (i == 0) {
						itemInfo->addElement(new Button("ThirdRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&](int& mouseState) {item->setBlockProjectile(!item->getBlockProjectile()); itemInfo->getElementByName("ThirdRowText_0")->setText("blockProjectile: " + std::to_string(item->getBlockProjectile())); }));
					}
					else if (i == 1) {
						itemInfo->addElement(new Button("ThirdRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&](int& mouseState) {item->setBlockPathfind(!item->getBlockPathfind()); itemInfo->getElementByName("ThirdRowText_1")->setText("blockPathfind: " + std::to_string(item->getBlockPathfind())); }));
					}
					else if (i == 2) {
						itemInfo->addElement(new Button("ThirdRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&](int& mouseState) {item->setBlockObject(!item->getBlockObject()); itemInfo->getElementByName("ThirdRowText_2")->setText("blockObject: " + std::to_string(item->getBlockObject())); }));
					}
					else if (i == 3) {
						itemInfo->addElement(new Button("ThirdRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&](int& mouseState) {item->setMoveable(!item->getMoveable()); itemInfo->getElementByName("ThirdRowText_3")->setText("moveable: " + std::to_string(item->getMoveable())); }));
					}

				}
				yFirst += offset * 2;
				text = "pickupable: " + std::to_string(item->getPickupable());
				// Fourth row: booleans
				for (int i = 0; i < 4; i++) {
					if (i == 1) text = "useable: " + std::to_string(item->getUseable());
					else if (i == 2) text = "hangeable: " + std::to_string(item->getHangeable());
					else if (i == 3) text = "alwaysOnTop: " + std::to_string(item->getAlwaysOnTop());

					itemInfo->addElement(new GUIElement("FourthRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

					if (i == 0) {
						itemInfo->addElement(new Button("FourthRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&](int& mouseState) {item->setPickupable(!item->getPickupable()); itemInfo->getElementByName("FourthRowText_0")->setText("pickupable: " + std::to_string(item->getPickupable())); }));
					}
					else if (i == 1) {
						itemInfo->addElement(new Button("FourthRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&](int& mouseState) {item->setUseable(!item->getUseable()); itemInfo->getElementByName("FourthRowText_1")->setText("useable: " + std::to_string(item->getUseable())); }));
					}
					else if (i == 2) {
						itemInfo->addElement(new Button("FourthRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&](int& mouseState) {item->setHangeable(!item->getHangeable()); itemInfo->getElementByName("FourthRowText_2")->setText("hangeable: " + std::to_string(item->getHangeable())); }));
					}
					else if (i == 3) {
						itemInfo->addElement(new Button("FourthRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 26, "", [&](int& mouseState) {item->setAlwaysOnTop(!item->getAlwaysOnTop()); itemInfo->getElementByName("FourthRowText_3")->setText("alwaysOnTop: " + std::to_string(item->getAlwaysOnTop())); }));
					}
				}
				if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(item)) {
					yFirst += offset * 2;
					auto lambda = ([&](int& mouseState) {
					itemInfoSubPage++;
					updateItemInfo = true;
					});
					itemInfo->addElement(new Button("RightArrow", findByName(verticesContainer, "itemInfoRightArrowID"), findByName(verticesContainer, "itemInfoRightArrowHoverID"), findByName(verticesContainer, "itemInfoRightArrowPressedID"), (startX + 547), (startY + 650), 30, 43, "", lambda));
				}
			}
			else {
				auto lambda = ([&](int& mouseState) {
					itemInfoSubPage--;
					updateItemInfo = true;
				});
				itemInfo->addElement(new Button("LeftArrow", findByName(verticesContainer, "itemInfoLeftArrowID"), findByName(verticesContainer, "itemInfoLeftArrowHoverID"), findByName(verticesContainer, "itemInfoLeftArrowPressedID"), (startX + 423 - (40 * doubleSize)), (startY + 650), 30, 43, "", lambda));

				if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(item)) {
					int yFirst = 125, offset = 40;

					std::string text = "weight: " + std::to_string(check->getWeight());
					// First row
					for (int i = 0; i < 3; i++) {
						if (i == 1) text = "stack: " + std::to_string(check->getStack());
						if (i == 2) text = "stackMax: " + std::to_string(check->getStackMax());

						itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));

						if (i == 0) {
							itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
							itemInfo->addElement(new Button("FirstRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", [](int& mouseState) {}));
						}
						else if (i == 1) {
							itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
							itemInfo->addElement(new Button("FirstRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", [](int& mouseState) {}));
						}
						else if (i == 2) {
							itemInfo->addElement(new GUIElement("FirstRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
							itemInfo->addElement(new Button("FirstRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", [](int& mouseState) {}));
						}

					}


					if (Armor* check = dynamic_cast<Armor*>(item)) {
						yFirst += offset * 3;
						text = "armor: " + std::to_string(check->getArmor());
						// Second row: text areas
						for (int i = 0; i < 2; i++) {
							if (i == 1) { text = "armorType: " + check->getArmorType(); 
							itemInfo->addElement(new GUIElement("SecondRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextAreaID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text)); }


							if (i == 0) {
								itemInfo->addElement(new GUIElement("SecondRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));
								itemInfo->addElement(new GUIElement("SecondRowTextSection_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
								itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", [](int& mouseState) {}));
							}

						}
					}
					else if (Weapon* check = dynamic_cast<Weapon*>(item)) {
						yFirst += offset * 3;
						text = "attack: " + std::to_string(check->getAttack());
						// Second row: text areas
						for (int i = 0; i < 3; i++) {
							if (i == 1) text = "defense: " + std::to_string(check->getDefense());
							else if (i == 2) { 
								text = "WeaponType: " + check->getWeaponType();
								itemInfo->addElement(new GUIElement("SecondRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextAreaID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));
							}


							if (i == 0) {
								itemInfo->addElement(new GUIElement("SecondRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));
								itemInfo->addElement(new GUIElement("SecondRowTextSection_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
								itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", [](int& mouseState) {}));
							}
							else if (i == 1) {
								itemInfo->addElement(new GUIElement("SecondRowText_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst), 150, 40, text));
								itemInfo->addElement(new GUIElement("SecondRowTextSection_" + std::to_string(i), findByName(verticesContainer, "itemInfoTextSectionID"), (startX + 50 + 250 * i), (startY + yFirst + offset), 150, 40, ">"));
								itemInfo->addElement(new Button("SecondRowButton_" + std::to_string(i), findByName(verticesContainer, "itemInfoButtonID"), findByName(verticesContainer, "itemInfoButtonHoverID"), findByName(verticesContainer, "itemInfoButtonPressedID"), (startX + 50 + 250 * i), (startY + yFirst + offset * 2), 150, 26, "", [](int& mouseState) {}));
							}

						}

					}
				}
			}
			
		}
		
		itemInfo->createObjects();
	}
	*/
	
}

void generate_Palette_Modifier() {

	GUIPanels.push_back(paletteModifier);
	
	paletteModifier->setCheckIfOutside(true);

	int width = 679, height = 724, startX = ((screenWidth - width) / 2), startY = ((screenHeight - height) / 2);
	paletteModifier->setDimensions(startX, startY, width, height);
	if(paletteModifier->getShow()) paletteModifier->toggleShow();

	// Base
	paletteModifier->addElement(new GUIElement("basePanel", getGUIObjectFromLibrary("paletteModifierPanel")->getVertices(), startX, startY, width, height, ""));

	auto rtt = findByName(renderToTextureContainer, "paletteModifier");

	GUIGroup* groupLeft = new GUIGroup("emptyTiles_left_group", 0, 0, 64 * 4, 64 * 10);
	paletteModifier->addElement(groupLeft);
	groupLeft->setRenderToText(rtt, true);
	// Empty tiles left
	for (int i = 0; i < (rtt->getHeight() / 64); i++) {
		for (int j = 0; j < 4; j++) {
			groupLeft->addElement(new GUIElement("emptyTile_Left_" + std::to_string(i * 4 + j), rtt->getVerticesByName("emptyTileMarker"), 64 * j, 64 * i, 64, 64, ""));
		}
	}

	// Left items selection render to texture
	auto text = getGUIObjectFromLibrary("paletteModifierRendToTextLeft")->getVertices();
	text->setWidthText(64 * 4);
	text->setHeightText(640);
	text->setWidth(64 * 4);
	text->setHeight(640);
	float s = (std::fmod(screenHeight / 64.0f, 1) * 64) / 2;
	text->setYStartText(0);
	text->setTextureSizeWidth(rtt->getWidth());
	text->setTextureSizeHeight(rtt->getHeight());
	//ScaleableElement* leftSide = new ScaleableElement("tilesLeft", 0, 0, 64 * 40, 1000, text);
	ScaleableElement* leftSide = new ScaleableElement("tilesLeft", startX + 32, startY + 39, 64 * 4, 640, text);
	leftSide->setRenderToText(findByName(renderToTextureContainer, "paletteModifier"));
	leftSide->setOffsetY(palette_Modifier_Left_Offset);
	paletteModifier->addElement(leftSide);
	removeVertices(text);

	// Left toggle buttons
	{
		ToggleButtonGroup* tbg = new ToggleButtonGroup("paletteToggleGroupLeft", startX + 32, startY + 39, 64 * 4, 640);
		tbg->setCheckOutsideOfArea();
		tbg->setScrollLambda(get_Scroll_Lambda_Tbg(paletteModifier, "paletteToggleGroupLeft", "scrollbarLeft", "tilesLeft",
			palette_Modifier_Left_Offset, palette_Modifier_Left_Offset_Max, palette_Modifier_Left_page, palette_Modifier_Left_Selected_Palette, palette_Modifier_Left_Clicked));
		tbg->setClickLambda(get_Palette_Modifier_Left_Mouse_Lambda());
		tbg->setHoverLambda([tbg](double& x, double& y) {
			int xStart = (int)((x / 64)) * 64, yStart = (int)(y / 64) * 64;
			if ((xStart >= 0 && xStart < 256) && (yStart >= 0 && yStart < 1024)) {
				if ((lControl || lShift) && !tbg->getDoNotResetAfterToggle())
					tbg->setDoNotResetAfterToggle(true);
				else if (!(lControl || lShift) && tbg->getDoNotResetAfterToggle())
					tbg->setDoNotResetAfterToggle(false);
			}
			});
		tbg->setCheckOutside();
		tbg->setRenderToText(rtt, true);
		paletteModifier->addElement(tbg);
	}

	GUIGroup* groupRight = new GUIGroup("emptyTiles_right_group", 0, 0, 64 * 4, 64 * 10);
	paletteModifier->addElement(groupRight);
	groupRight->setRenderToText(rtt, true);
	// Empty tiles left
	for (int i = 0; i < (rtt->getHeight() / 64); i++) {
		for (int j = 0; j < 4; j++) {
			groupRight->addElement(new GUIElement("emptyTile" + std::to_string((i * 4) + j), rtt->getVerticesByName("emptyTileMarker"), 64 * (j+4), 64 * i, 64, 64, ""));
		}
	}

	text->setWidth(64 * 4);
	text->setHeight(640);

	text->setWidthText(64 * 4);
	text->setHeightText(640);

	text->setXStartText(64 * 4);

	text->setTextureSizeWidth(rtt->getWidth());
	text->setTextureSizeHeight(rtt->getHeight());
	// Right side object to use render to texture
	ScaleableElement* rightSide = new ScaleableElement("tilesRight", startX + 392, startY + 39, 64 * 4, 640, text);
	rightSide->setRenderToText(findByName(renderToTextureContainer, "paletteModifier"));
	rightSide->setOffsetY(palette_Modifier_Right_Offset);
	paletteModifier->addElement(rightSide);

	// Right side toggle buttons
	{
		auto tbg = new ToggleButtonGroup("paletteToggleGroupRight", startX + 392, startY + 39, 64 * 4, 640);
		tbg->setCheckOutsideOfArea();
		tbg->setScrollLambda(get_Scroll_Lambda_Tbg(paletteModifier, "paletteToggleGroupRight", "scrollbarRight", "tilesRight",
			palette_Modifier_Right_Offset, palette_Modifier_Right_Offset_Max, palette_Modifier_Right_page, palette_Modifier_Right_Selected_Palette, palette_Modifier_Right_Clicked));
		tbg->setHoverLambda(get_Hover_Lambda_Palette_Modifier_Right(tbg));
		tbg->setClickLambda(get_Palette_Modifier_Right_Mouse_Lambda());
		tbg->setCheckOutside();
		tbg->setRenderToText(rtt, true);
		paletteModifier->addElement(tbg);
	}

	// The right side preview item
	GUIGroup* rightSidePreview = new GUIGroup("rightSidePreview", 0, 0, 64, 64);
	rightSidePreview->setRenderToText(rtt, true);
	paletteModifier->addElement(rightSidePreview);

	// The right side previewHover item
	GUIGroup* rightSidePreviewHover = new GUIGroup("rightSidePreviewHover", 0, 0, 64, 64);
	rightSidePreviewHover->setRenderToText(rtt, true);
	paletteModifier->addElement(rightSidePreviewHover);

	// Search icon
	paletteModifier->addElement(new ExpandingButton("searchIcon", nullptr, nullptr, nullptr, startX + 180, startY + 12, 22, 22, 1.5, "", [](int& mouseState) {}));

	// Drop down for palettes (right side)
	{
		DropDown* d = new DropDown("paletteSelection", getGUIObjectFromLibrary("paletteModifierDropDownElement")->getVertices(), nullptr, nullptr, getGUIObjectFromLibrary("paletteModifierDropDownElementHover")->getVertices(), startX + 460, startY + 13, 102, 24);
		d->setPadding(1);
		for (auto& i : palettes) {
			if(i.getName().compare("All") != 0)
				d->add(i.getName(), [&, d]() {
					tempPalette.setName(i.getName());
					tempPalette.replacePalette(i.getPalette());
					tempPalette.findMaxY();
					palette_Modifier_Right_Selected_Palette = tempPalette.getName();
					palette_Modifier_Right_Offset = 64;
					palette_Modifier_Right_page = 0;

					auto rtt = paletteModifier->getElementByName("tilesRight");
					rtt->setOffsetY(palette_Modifier_Right_Offset);

					prepare_Palette_Modifier_Rend_To_Text(paletteModifier, rtt->getRenderToText(), rtt, paletteModifier->getElementByName("paletteToggleGroupRight"), tempPalette, palette_Modifier_Right_page, palette_Modifier_Right_Offset_Max, palette_Modifier_Right_Clicked);

					reset_Right_Palette_Modifier_RTT();

					auto tb = d->getButton();
					tb->setText(tempPalette.getName());
					tb->resetToggle();

					if (GUIElement* rtt = dynamic_cast<GUIElement*>(paletteModifier->getElementByName("tilesRight"))) {
						auto rttTemp = findByName(renderToTextureContainer, "paletteModifier");
						if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(paletteModifier->getElementByName("scrollbarRight"))) {
							sb->reset();
							auto paletteItems = i.getPalette();
							if (paletteItems.size() > 0) {
								auto y = paletteItems.at(paletteItems.size() - 1).getY();
								palette_Modifier_Right_maxPage = y * 64.0 / (palette_Modifier_Right_Offset_Max);
								double maxPixels = 64.0 * y,
									scrollbarLength = sb->getScrollbarMaxLength() * (((rtt->getHeight() / 64.0) * 64) / maxPixels);
								sb->updateScrollbarLengthAndPositionAndIncrementSize(scrollbarLength, 0, y * 64);
							}
							else {
								sb->updateScrollbarLengthAndPosition(10000, 0);
							}

						}
					}
				});
		}
		paletteModifier->addElement(d);
	}

	// Drop down for items (left Side)
	{
		DropDown* d = new DropDown("itemsSelection", getGUIObjectFromLibrary("paletteModifierDropDownElement")->getVertices(), nullptr, nullptr, getGUIObjectFromLibrary("paletteModifierDropDownElementHover")->getVertices(), startX + 210, startY + 13, 102, 24);
		d->setPadding(1);
		for (auto& i : palettes) {
			d->add(i.getName(), [&, d]() {
				palette_Modifier_Left_Selected_Palette = i.getName();
				palette_Modifier_Left_Offset = 64;
				palette_Modifier_Left_page = 0;

				auto rtt = paletteModifier->getElementByName("tilesLeft");
				rtt->setOffsetY(palette_Modifier_Left_Offset);

				prepare_Palette_Modifier_Rend_To_Text(paletteModifier, rtt->getRenderToText(), rtt, paletteModifier->getElementByName("paletteToggleGroupLeft"), i, palette_Modifier_Left_page, palette_Modifier_Left_Offset_Max, palette_Modifier_Left_Clicked);


				reset_Left_Palette_Modifier_RTT();

				auto tb = d->getButton();
				tb->setText(i.getName());
				tb->resetToggle();


				if (GUIElement* rtt = dynamic_cast<GUIElement*>(paletteModifier->getElementByName("tilesLeft"))) {
					auto rttTemp = findByName(renderToTextureContainer, "paletteModifier");
					if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(paletteModifier->getElementByName("scrollbarLeft"))) {
						sb->reset();
						auto paletteItems = i.getPalette();
						if (paletteItems.size() > 0) {
							auto y = paletteItems.at(paletteItems.size() - 1).getY();
							palette_Modifier_Left_maxPage = y * 64.0 / (palette_Modifier_Left_Offset_Max);
							double maxPixels = 64.0 * y,
								scrollbarLength = sb->getScrollbarMaxLength() * (((rtt->getHeight() / 64.0) * 64) / maxPixels);
							sb->updateScrollbarLengthAndPositionAndIncrementSize(scrollbarLength, 0, y * 64);
						}
						else {
							sb->updateScrollbarLengthAndPosition(10000, 0);
						}

					}
				}
				});
		}
		paletteModifier->addElement(d);
	}

	// Save icon
	{
		auto saveIcon = new Button("saveIcon",
			getGUIObjectFromLibrary("saveIcon")->getVertices(),
			getGUIObjectFromLibrary("saveIconHover")->getVertices(),
			getGUIObjectFromLibrary("saveIconClicked")->getVertices(),
			startX + 568, startY + 5, 128, 128, "",
			[&](int& mouseState) {
				if (tempPalette.getName().compare("empty") != 0) {
					std::sort(tempPalette.getPalette().begin(), tempPalette.getPalette().end(), [](PaletteItem& lhs, PaletteItem& rhs) {
						if (lhs.getY() < rhs.getY()) return true;
						if (rhs.getY() < lhs.getY()) return false;

						if (lhs.getX() < rhs.getX()) return true;
						if (rhs.getX() < lhs.getX()) return false;

						return false;
						});
					findByName(palettes, tempPalette.getName()) = tempPalette;
				}
			});
		saveIcon->setScale(0.25);
		paletteModifier->addElement(saveIcon);
	}

	// Trash icon
	{
		// Trash icon
		auto trashIcon = new Button("trashIcon",
			getGUIObjectFromLibrary("trashIcon")->getVertices(),
			getGUIObjectFromLibrary("trashIconHover")->getVertices(),
			getGUIObjectFromLibrary("trashIconClicked")->getVertices(),
			startX + 308, startY + 350, 128, 128, "",
			[&](int& mouseState) {
				if (ToggleButtonGroup* tbg = dynamic_cast<ToggleButtonGroup*>(paletteModifier->getElementByName("paletteToggleGroupRight"))) {
					auto tb = tbg->getElementsByClicked();
					if (tb.size() != 0) {
						std::vector<PaletteItem> changes;
						for (auto& tb : tb) {
							int pageSize = screenHeight / 64, pos = std::stoi(tb->getName().substr(7)) - 4, yPosPalette = int(((pos)-palette_Modifier_Right_page * pageSize) / 4), xPosPalette = pos - (yPosPalette * 4);
							tempPalette.removeItemAtLocation(xPosPalette, yPosPalette, changes);
						}
						palette_Modifier_Undo.push_back(PaletteChange("Deleted", changes));
						palette_Modifier_Redo.clear();
						tbg->resetAll();
						auto rtt = paletteModifier->getElementByName("tilesRight");
						prepare_Palette_Modifier_Rend_To_Text(paletteModifier, rtt->getRenderToText(), rtt, paletteModifier->getElementByName("paletteToggleGroupRight"), tempPalette, palette_Modifier_Right_page, palette_Modifier_Right_Offset_Max, palette_Modifier_Right_Clicked);
						if (!palette_Modifier_Right_First) {
							palette_Modifier_Right_Selected_Tile.changeAll(-1, false, -1, -1);
							palette_Modifier_Right_First = true;
						}
					}
				}
				if (palette_Modifier_Right_Shift_Start != -1)
					palette_Modifier_Right_Shift_Start = -1;
			});
		trashIcon->setScale(0.5);
		paletteModifier->addElement(trashIcon);
	}

	//Left scrollbar
	{
		int scrollbarXStart = startX + 10, scrollbarYStart = startY + 37;
		auto buttonUp = new Button("up",
			getGUIObjectFromLibrary("scrollbar1ButtonUp")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonUpHover")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonUpClicked")->getVertices(),
			scrollbarXStart, scrollbarYStart, 19, 10, "", [](int& mouseState) {
				if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(paletteModifier->getElementByName("scrollbarLeft"))) {
					auto change = sb->getScrollbarMaxLength() / ((sb->getIncrementSize() * sb->getScrollbarMaxLength()) / 64.0);
					sb->updateScrollbarPosition(sb->getScrollbarPositionRaw() - (change), true);
				}
			});
		auto buttonDown = new Button("down",
			getGUIObjectFromLibrary("scrollbar1ButtonDown")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonDownHover")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonDownClicked")->getVertices(),
			scrollbarXStart, scrollbarYStart, 19, 10, "", [](int& mouseState) {
				if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(paletteModifier->getElementByName("scrollbarLeft"))) {
					auto change = sb->getScrollbarMaxLength() / ((sb->getIncrementSize() * sb->getScrollbarMaxLength()) / 64.0);
					sb->updateScrollbarPosition(sb->getScrollbarPositionRaw() + (change), true);
				}
			});
		auto scrollbar = new ScrollbarVertical("scrollbarLeft", scrollbarXStart, scrollbarYStart, 19, 644, buttonUp, buttonDown,
			getGUIObjectFromLibrary("scrollbar1Background")->getVertices(),
			nullptr,
			nullptr,
			getGUIObjectFromLibrary("scrollbar1Bar")->getVertices(), getGUIObjectFromLibrary("scrollbar1BarHover")->getVertices(), getGUIObjectFromLibrary("scrollbar1BarClicked")->getVertices());
		scrollbar->setLambda(get_Palette_Modifier_Scrollbar_Scroll_Lambda(scrollbar, paletteModifier->getElementByName("tilesLeft"), paletteModifier->getElementByName("paletteToggleGroupLeft"), paletteModifier, findByName(renderToTextureContainer, "paletteModifier"),
			palette_Modifier_Left_Offset, palette_Modifier_Left_Offset_Max, palette_Modifier_Left_page, palette_Modifier_Left_maxPage, palette_Modifier_Left_Selected_Palette, palette_Modifier_Left_Clicked));
		paletteModifier->addElement(scrollbar);
	}

	//Right scrollbar
	{
		int scrollbarXStart = startX + 649, scrollbarYStart = startY + 37;
		auto buttonUp = new Button("up",
			getGUIObjectFromLibrary("scrollbar1ButtonUp")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonUpHover")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonUpClicked")->getVertices(),
			scrollbarXStart, scrollbarYStart, 19, 10, "", [](int& mouseState) {
				if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(paletteModifier->getElementByName("scrollbarRight"))) {
					auto change = sb->getScrollbarMaxLength() / ((sb->getIncrementSize() * sb->getScrollbarMaxLength()) / 64.0);
					sb->updateScrollbarPosition(sb->getScrollbarPositionRaw() - (change), true);
				}
			});
		auto buttonDown = new Button("down",
			getGUIObjectFromLibrary("scrollbar1ButtonDown")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonDownHover")->getVertices(),
			getGUIObjectFromLibrary("scrollbar1ButtonDownClicked")->getVertices(),
			scrollbarXStart, scrollbarYStart, 19, 10, "", [](int& mouseState) {
				if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(paletteModifier->getElementByName("scrollbarRight"))) {
					auto change = sb->getScrollbarMaxLength() / ((sb->getIncrementSize() * sb->getScrollbarMaxLength()) / 64.0);
					sb->updateScrollbarPosition(sb->getScrollbarPositionRaw() + (change), true);
				}
			});
		auto scrollbar = new ScrollbarVertical("scrollbarRight", scrollbarXStart, scrollbarYStart, 19, 644, buttonUp, buttonDown,
			getGUIObjectFromLibrary("scrollbar1Background")->getVertices(),
			nullptr,
			nullptr,
			getGUIObjectFromLibrary("scrollbar1Bar")->getVertices(), getGUIObjectFromLibrary("scrollbar1BarHover")->getVertices(), getGUIObjectFromLibrary("scrollbar1BarClicked")->getVertices());
		scrollbar->setLambda(get_Palette_Modifier_Scrollbar_Scroll_Lambda(scrollbar, paletteModifier->getElementByName("tilesRight"), paletteModifier->getElementByName("paletteToggleGroupRight"), paletteModifier, findByName(renderToTextureContainer, "paletteModifier"),
			palette_Modifier_Right_Offset, palette_Modifier_Right_Offset_Max, palette_Modifier_Right_page, palette_Modifier_Right_maxPage, palette_Modifier_Right_Selected_Palette, palette_Modifier_Right_Clicked));
		paletteModifier->addElement(scrollbar);
	}

	// TextField used for looking up items
	{
		TextField* textField = new TextField("textField", startX + 30, startY + 13, 148, 22, getGUIObjectFromLibrary("textField1")->getVertices(), getGUIObjectFromLibrary("textField1Focused")->getVertices());
		textField->setTextStartXOffset(3);
		textField->setLambda([textField]() {
			std::string currentPaletteName = palette_Modifier_Left_Selected_Palette, text = textField->getText(), itemName = "";
			if (currentPaletteName != "" && textField->getText().size() > 0) {
				tempPaletteLeft.clear();
				reset_Left_Palette_Modifier_RTT();
				reset_Right_Palette_Modifier_RTT();

				std::vector<PaletteItem> tempItems;
				std::pair<Item*, Object*> item;
				for (auto p : palettes) {
					if (p.getName() == currentPaletteName) {
						for (auto i : p.getPalette()) {
							item = itemAtlas.getItemAndObject(i.getID());
							itemName = "";
							if (item.second) itemName = item.second->getName();
							if (item.first && item.second &&
								((std::to_string(i.getID()) == text) ||
									check_Sub_String_Case_Insensitive(itemName, text)))
								tempItems.push_back(PaletteItem(i.getID(), i.isDoubleSize(), 0, 0));
						}
					}
				}
				tempPaletteLeft.addItems(tempItems);

				// Reset scrollbar
				if (GUIElement* rtt = dynamic_cast<GUIElement*>(paletteModifier->getElementByName("tilesLeft"))) {
					auto rttTemp = findByName(renderToTextureContainer, "paletteModifier");
					if (ScrollbarVertical* sb = dynamic_cast<ScrollbarVertical*>(paletteModifier->getElementByName("scrollbarLeft"))) {
						sb->reset();
						auto paletteItems = tempPaletteLeft.getPalette();
						if (paletteItems.size() > 0) {
							auto y = paletteItems.at(paletteItems.size() - 1).getY(); if (y == 0) y++;
							palette_Modifier_Left_maxPage = y / (rttTemp->getHeight() / 64);
							double maxPixels = 64.0 * y,
								scrollbarLength = sb->getScrollbarMaxLength() * (((rtt->getHeight() / 64.0) * 64) / maxPixels);
							sb->updateScrollbarLengthAndPositionAndIncrementSize(scrollbarLength, 0, y * 64);
						}
						else {
							sb->updateScrollbarLengthAndPosition(10000, 0);
						}

					}
					prepare_Palette_Modifier_Rend_To_Text(paletteModifier, rtt->getRenderToText(), rtt, paletteModifier->getElementByName("paletteToggleGroupLeft"), tempPaletteLeft, palette_Modifier_Left_page, palette_Modifier_Left_Offset_Max, palette_Modifier_Left_Clicked);

				}

			}
			else if (currentPaletteName != "") {
				reset_Left_Palette_Modifier_RTT();
				reset_Right_Palette_Modifier_RTT();
				if (DropDown* dd = dynamic_cast<DropDown*>(paletteModifier->getElementByName("itemsSelection")))
					for (auto e : dd->geDropDownElements()) {
						if (e->getName() == currentPaletteName) {
							e->runLambda();
							textField->setUpdate(true);
						}
					}
			}
			//palettes
			});
		textField->setTriggerOnKeyStroke(true);
		textField->setAnimationLambda([textField]()
			{
				__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				auto then = textField->getTimeStamp();
				auto marker = textField->getMarker();
				if (textField->getFocus() && marker && (then == 0 || (now - then) > 500)) {
					textField->setTimeStamp(now);
					marker->setDraw(!marker->getDraw());
				}
			});
		textField->setAnimationBool(true);
		paletteModifier->addElement(textField);
	}
	
	paletteModifier->createObjects();
}

void generate_Left_Panel_Tiles(Palette& p) {

	if (ToggleButtonGroup * tgb = dynamic_cast<ToggleButtonGroup*>(leftPanel->getElementByName("paletteToggleGroup"))) {
		tgb->clearAll();

		if (p.getPalette().size() > 0) {
			auto rtt = findByName(renderToTextureContainer, "leftPanelTiles");
			int maxPerPage = (rtt->getHeight() / 64) - 1;
			auto tilesRTT = leftPanel->getElementByName("tilesRTT");
			int minY = left_panel_page * (maxPerPage)-(left_panel_page * (tilesRTT->getHeight() / 64)), maxY = minY + (maxPerPage), count = 0;
			left_panel_Offset_Max = (rtt->getHeight() - tilesRTT->getHeight());
			if (left_panel_page > 0) maxY++;
			//if(palette_Modifier_Right_page == 1) min

			for (int y = 0; y < (rtt->getHeight() / 64); y++) {
				for (int x = 0; x < 4; x++) {
					ToggleButton* tb = new ToggleButton("toggle_" + std::to_string(count), nullptr, rtt->getVerticesByName("yellowSquareSmall"), rtt->getVerticesByName("redSquareSmall"), x * 64, (y) * 64, x * 64, y * 64, 63, 63, [](int& mouseState) {
						generate_Left_Panel_Rend_To_Text();
						},
						[](double& x, double& y) {
							generate_Left_Panel_Rend_To_Text();
						});
					tb->toggleAlwaysShowBase();
					tgb->addElement(tb);
					count++;
				}
			}

			tgb->setDoNotResetAfterToggle(false);

			for (auto& i : p.getPalette()) {
				ToggleButton* toggle = nullptr;
				int pos = 0;
				if (i.getY() >= minY && i.getY() <= maxY) {
					pos = ((i.getY() - minY) * 4) + i.getX();
					pos += 4;
					toggle = tgb->getElementByName("toggle_" + std::to_string(pos));
				}
				else if (minY > 0 && i.getY() == minY - 1) {
					pos = ((i.getY() + 1 - minY) * 4) + i.getX();
					toggle = tgb->getElementByName("toggle_" + std::to_string(pos));
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
							toggle->setHover(rtt->getVerticesByName("yellowSquareBig"));
							toggle->setClicked(rtt->getVerticesByName("redSquareBig"));
							toggle->setWidth(127);
							toggle->setHeight(127);
							//toggle->addToStartXText(64);
							//toggle->addToStartYText(64);
							toggle->setXDrawOffset(-64);
							toggle->setYDrawOffset(-64);

							toggle = tgb->getElementByName("toggle_" + std::to_string(pos + 1));
							if (toggle != nullptr && toggle->getShow()) toggle->toggleShow();
							toggle = tgb->getElementByName("toggle_" + std::to_string(pos + 4));
							if (toggle != nullptr && toggle->getShow()) toggle->toggleShow();
							toggle = tgb->getElementByName("toggle_" + std::to_string(pos + 5));
							if (toggle != nullptr && toggle->getShow()) toggle->toggleShow();
						}
					}
				}
			}

		}
	}
	generate_Left_Panel_Rend_To_Text();
}

void generate_Left_Panel_Rend_To_Text() {
	// first pass
	RendToText* rtt = findByName(renderToTextureContainer, "leftPanelTiles");
	glBindFramebuffer(GL_FRAMEBUFFER, rtt->getFboID());
	glViewport(0, 0, rtt->getWidth(), rtt->getHeight());
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);


	DrawObjects *temp = new DrawObjects("temp");
	leftPanel->getElementByName("emptyTiles_group")->createObjectNoRestriction(temp);
	if (left_panel_Selected_Palette != "") {
		leftPanel->getElementByName("paletteToggleGroup")->createObjectNoRestriction(temp);
	}
	for (auto& o : temp->getObjects()) {
		o->renderGUI("");
	}

	temp->clearObjects();

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glViewport(0, 0, screenWidth, screenHeight);
	glDisable(GL_DEPTH_TEST);
};
