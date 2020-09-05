#include "GUI.h"
#include "Global.h"
#include <algorithm>
#include "ElementCreator.h"
#include "ObjectLibrary.h"
#pragma warning(disable: 4244)


GUIElement::GUIElement(std::string name): name(name)
{
}

GUIElement::GUIElement(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string text = "") : name(name), v(v), xStart(xStart), yStart(yStart), width(width), height(height), text(text)
{
	xStartText = xStart; yStartText = yStart;
}

GUIElement::GUIElement(std::string name, Vertices* v, int xStartText, int yStartText, int xStart, int yStart, int width, int height, std::string text = "") : name(name), v(v), xStartText(xStartText), xStart(xStart), yStartText(yStartText), yStart(yStart), width(width), height(height), text(text) {
	rendToText = true;
}

void GUIElement::testHover(double& xPosRef, double& yPosRef)
{
	if (!rendToText) {
		bool test =
			(
				xPosRef >= xStart &&
				xPosRef <= xStart + (width * scale) &&
				yPosRef >= yStart &&
				yPosRef <= yStart + (height * scale)
				);
		if (!isHover && test) {
			isHover = true;
			handleHover();
		}
		else if (isHover && !test) {
			isHover = false;
			handleHover();
		}
	}
	else {
		bool test =
			(
				xPosRef >= xStart &&
				xPosRef <= xStart + (width * scale) &&
				yPosRef >= yStart &&
				yPosRef <= yStart + (height * scale)
				);
		if (!isHover && test) {
			isHover = true;
			handleHover();
		}
		else if (isHover && !test) {
			isHover = false;
			handleHover();
		}
	}
}

bool GUIElement::testClicked(double & xPos, double & yPos, int mouseState, bool& isClickedAbove)
{
	bool test =
		(
			xPos >= xStart &&
			xPos <= xStart + (width * scale) &&
			yPos >= yStart &&
			yPos <= yStart + (height * scale)
			);
	if (show && mouseState == MOUSE_LEFT_CLICK && !isClicked && test) {
		isClickedAbove = true;
		handleClicked(mouseState);
	}
	else if (show && isClicked && test) {
		handleClicked(mouseState);
	}
	return test;
}

void GUIElement::createObject(DrawObjects *ob)
{
	if (!rendToText) {
		if (v && show) {
			
			auto* o = new DrawObject(((xStartText + xDrawOffset ) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
			if (rtt != nullptr) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob->addObject(o);
			
		}
		if (text.size() > 0) {

			if (!isCenteredText) {
				// Text
				generate_GUI_Text(ob, ((xStartText) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), 0 + textOffset, width - textOffset, text, "Black");
			}
			else {
				// Text
				//generate_GUI_Text(ob, vhText, ((xStartText + (width / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
		}
	}
}

void GUIElement::createObjectNoRestriction(DrawObjects *ob) {
	
	if (v && show) {
		DrawObject* o = nullptr;
		if (!rendToText || (rendToText && !rtt)) o = new DrawObject(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
		else {
			o = new DrawObject(((xStartText + xDrawOffset) / (double(rtt->getWidth()) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(rtt->getHeight()) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
			//o = new DrawObject(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
		}
		if (rtt != nullptr && !rendToText) o->setRendToText(rtt);
		o->setOffsetX(xOffset / textureWidth);
		o->setOffsetY(yOffset / textureHeight);
		ob->addObject(o);
	}
	if (text.size() > 0) {

		if (!isCenteredText) {
			// Text
			//generate_GUI_Text(ob, vhText, ((xStartText) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
		}
		else {
			// Text
			//generate_GUI_Text(ob, vhText, ((xStartText + (width / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
		}
	}
	
}

void GUIElement::testScroll(double & scroll)
{
}


std::string & GUIElement::getName()
{
	return name;
}

void GUIElement::setUpdate(bool value)
{
	update = value;
}

bool GUIElement::getUpdate()
{
	return update;
}

bool GUIElement::getHover()
{
	return isHover;
}

bool GUIElement::getClicked()
{
	return isClicked;
}

void GUIElement::toggleClicked()
{ 
	int mouseState = MOUSE_LEFT_CLICK; handleClicked(mouseState);
};


void GUIElement::setText(std::string s)
{
	text = s;
}

void GUIElement::handleHover()
{
}

void GUIElement::handleClicked(int& mouseState)
{
	isClicked = !isClicked;
}

void GUIElement::handleAnimation()
{
	animation();
}

void GUIElement::handleKeyStroke(int& key, int& pressed, int& mods)
{
}

void GUIElement::checkAnimation()
{
	if (doAnimation && animation) {
		update = true;
		handleAnimation();
	}
}

GUIPanel::GUIPanel(std::string name): name(name)
{
}

GUIPanel::GUIPanel(std::string name, DrawObjects *ob, int x, int y, int width, int height): name(name), ob(ob), xStart(x), yStart(y), width(width), height(height)
{
}

GUIPanel::~GUIPanel()
{
	clear();
}

void GUIPanel::clear()
{
	for (auto& e : elements) {
		delete e;
	}

	elements.clear();
}

void GUIPanel::addElement(GUIElement * e)
{
	if (checkIfOutside)
		e->setCheckOutsideOfArea();
	elements.push_back(e);
	testedLabel = false;
}

GUIElement* GUIPanel::getElementByName(std::string name)
{
	auto it = std::find_if(elements.begin(), elements.end(), [name](GUIElement*& e) { return (e->getName().compare(name) == 0); });
	if (it != elements.end())
		return (*it);
	return nullptr;
}

std::vector<GUIElement*> GUIPanel::getEveryElement()
{
	std::vector<GUIElement*> all;
	for (auto e : elements) {
		all.push_back(e);
		for (auto internalElement : e->getElements())
			all.push_back(internalElement);
	}
	return all;
}

void GUIPanel::removeElementByName(std::string name)
{
	auto it = std::find_if(elements.begin(), elements.end(), [name](GUIElement*& e) { return (e->getName().compare(name) == 0); });
	if (it != elements.end()) {
		delete (*it);
		elements.erase(it);
	}
}

void GUIPanel::createObjects()
{
	if (show && ob) {
		for (auto& e : elements) {
			e->createObject(ob);
		}
	}
}

void GUIPanel::reCreateObjects()
{
	if (ob) {
		ob->clearObjects();
		createObjects();
	}
}

void GUIPanel::clearObjects()
{
	if(ob)
		ob->clearObjects();
}

void GUIPanel::setDimensions(int x, int y, int width, int height)
{
	this->xStart = x;
	this->yStart = y;
	this->width = width;
	this->height = height;
}

void GUIPanel::checkHover(double & xPos, double & yPos)
{
	if (!checkIfOutside && xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		for (auto& e : elements) {
			e->testHover(xPos, yPos);
		}
	}
	else if (checkIfOutside) {
		for (auto& e : elements) {
			e->testHover(xPos, yPos);
		}
	}
}

void GUIPanel::checkClicked(double & xPos, double & yPos, int mouseState)
{
	if (checkIfOutside || xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		for (auto e = elements.end()-1; e != elements.begin(); e-- ) {
			if (!isClicked)
				(*e)->testClicked(xPos, yPos, mouseState, isClicked);
			else
				break;
		}
	}
	isClicked = false;
}

void GUIPanel::checkScroll(double& xPos, double& yPos, double& scroll)
{
	if (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		for (auto& e : elements)
			e->testScroll(scroll);
	}
}

void GUIPanel::checkKeyStroke(int& key, int& action, int& mods)
{
	for (auto e : elements) {
		if(e->getFocus())
			e->handleKeyStroke(key, action, mods);
	}
}

void GUIPanel::checkUpdates()
{
	bool update = false;
	for (auto& e : elements) {
		e->checkAnimation();
		if (e->getUpdate()) {
			update = true;
			e->setUpdate(false);
		}
	}
	if (containsLabel())
		update = true;
	if (update) {
		ob->clearObjects();
		createObjects();
	}
}

bool GUIPanel::containsLabel()
{
	if (!testedLabel) {
		for (auto e : getEveryElement())
			if (GUILabel* l = dynamic_cast<GUILabel*>(e)) {
				isLabel = true;
			}
		testedLabel = true;
	}
	return isLabel;
}

Button::Button(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::string text = "", std::function<void(int& mouseState)> lambda = [](int& mouseState) {}) : button(button), hover(hover), clicked(clicked), GUIElement(name, button, xStart, yStart, width, height, text)
{
	clickLambda = lambda;
}

void Button::createObject(DrawObjects *ob)
{
	
	if (show && !rendToText && getV()) {

		if (button || (button && getV() == button)) {
			auto* o = new DrawObject((xStartText / (double(screenWidthPixels) / 2)), 0.0 + (yStartText / (double(screenHeightPixels) / 2)), button->getID(), button->getVAO(), button->getTextPos());
			o->setScale(scale);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob->addObject(o);
		}
		if (getV() && !(getV() == button)) {
			auto* o = new DrawObject(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
			o->setScale(scale);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob->addObject(o);
		}
		if (text.size() > 0) {

			if (!isCenteredText) {
				// Text
				generate_GUI_Text(ob, ((xStart + 70) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 70) / (double(screenHeightPixels) / 2)), 0, width - 36, text, "Black");
			}
			else {
				// Text
				generate_GUI_Text(ob, ((xStart + (width / 2) - 40) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), 0, width - 36, text, "Black");
			}
		}
	}
	
}

void Button::createObjectNoRestriction(DrawObjects *ob)
{
	/*
	if (show && getV()) {
		if (button || (button && getV() == button)) {
			auto* o = new DrawObject(((xStartText) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), button->getID(), button->getVertecesHandler()->getVAO(), button->getVertecesHandler()->getTextureID());
			if (rtt != nullptr) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob.addObject(o);
		}
		if (getV() && !(getV() == button)) {
			auto* o = new DrawObject(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVertecesHandler()->getVAO(), getV()->getVertecesHandler()->getTextureID());
			if (rtt != nullptr) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob.addObject(o);
		}
		if (text.size() > 0) {
			VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

			if (!isCenteredText) {
				// Text
				generate_GUI_Text(ob, vhText, ((xStartText + (width / 2) - (text.size() * 16) / 4 ) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + (height / 2) - 8) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
			else {
				// Text
				generate_GUI_Text(ob, vhText, ((xStartText + (width / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
		}
	}
	*/
}

bool Button::testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove)
{
	bool test =
		(
			xPos >= xStart &&
			xPos <= xStart + (width * scale) &&
			yPos >= yStart &&
			yPos <= yStart + (height * scale)
			);
	if (show && test) {
		if (mouseState == 1) {
			isClickedAbove = true;
			isClicked = true;
		}else if(mouseState == 0)
			isClicked = false;
		handleClicked(mouseState);
	}
	else if (show && isClicked && !test) {
		handleClicked(mouseState);
		isClicked = false;
		setV(button);
		this->setUpdate(true);
	}
	return isClicked;
}

void Button::handleHover()
{
	if (!getClicked() && getHover() && getV() != hover) {
		setV(hover);
		setUpdate(true);
	}
	else if (!getClicked() && !getHover() && getV() != button) {
		setV(button);
		setUpdate(true);
	}
}

void Button::handleClicked(int& mouseState)
{
	if (mouseState == 1 && getClicked() && getV() != clicked) {
		setV(clicked);
		setUpdate(true);
	}
	else if(mouseState == 0 && !getClicked() && getV() == clicked){
		clickLambda(mouseState);
		if (getHover()) {
			setV(hover);
		}else
			setV(button);
		setUpdate(true);
	}
}
ToggleButton::ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda) : button(button), hover(hover), clicked(clicked), GUIElement(name, button, xStart, yStart, width, height)
{
	clickLambda = lambda;
}
ToggleButton::ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStartText, int yStartText, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda) : button(button), hover(hover), clicked(clicked), GUIElement(name, button, xStartText, yStartText, xStart, yStart, width, height)
{
	clickLambda = lambda;
}
ToggleButton::ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda, std::function<void(double& x, double& y)> hoverLambda) : button(button), hover(hover), clicked(clicked), GUIElement(name, button, xStart, yStart, width, height)
{
	clickLambda = lambda;
	setHoverLambda(hoverLambda);
}
ToggleButton::ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStartText, int yStartText, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda, std::function<void(double& x, double& y)> hoverLambda) : button(button), hover(hover), clicked(clicked), GUIElement(name, button, xStartText, yStartText, xStart, yStart, width, height)
{
	clickLambda = lambda;
	setHoverLambda(hoverLambda);
}


void ToggleButton::createObject(DrawObjects *ob)
{
	
	if (show && !rendToText && (alwaysShowBase || getV())) {
		if ((alwaysShowBase && button) || (button && getV() == button)) {
			auto* o = new DrawObject(((xStartText + 64 * offset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + 64 * offset) / (double(screenHeightPixels) / 2)), button->getID(), button->getVAO(), button->getTextPos());
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob->addObject(o);
		}
		if (getV() && !(getV() == button)) {
			auto* o = new DrawObject(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob->addObject(o);
		}
		if (text.size() > 0) {

			if (!isCenteredText) {
				// Text
				//generate_GUI_Text(ob, ((xStart + 70) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 70) / (double(screenHeightPixels) / 2)), 0, width - 36, text, "Black");
				generate_GUI_Text(ob, ((xStart - 0) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), textOffset, width - textOffset, text, "Black");
			}
			else {
				// Text
				//generate_GUI_Text(ob, ((xStart + (width / 2) - 40) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), 0, width - 36, text, "Black");
				auto textSize = Get_Text_Size(text, "Black");
				generate_GUI_Text(ob, ((xStart + (width / 2) - (textSize/2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), 0, width - textOffset, text, "Black");
			}
		}
	}
	
}

void ToggleButton::createObjectNoRestriction(DrawObjects  *ob) {
	
	if (show && (alwaysShowBase || getV())) {
		if ((alwaysShowBase && button) || (button && getV() == button)) {
			DrawObject* o = nullptr;
			if(!rendToText || (rendToText && !rtt)) o = new DrawObject(((xStartText + 64 * offset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + 64 * offset) / (double(screenHeightPixels) / 2)), button->getID(), button->getVAO(), button->getTextPos());
			else {
				o = new DrawObject( ((xStartText + 64 * offset) / (double(rtt->getWidth()) / 2)), 0.0 + ((yStartText + 64 * offset) / (double(rtt->getHeight()) / 2)), button->getID(), button->getVAO(), button->getTextPos());
			}
			if (rtt != nullptr && !rendToText) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob->addObject(o);
		}
		if (getV() && !(getV() == button)) {
			DrawObject* o = nullptr;
			if (!rendToText || (rendToText && !rtt)) o = new DrawObject(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
			else {
				o = new DrawObject(((xStartText + 64 * offset) / (double(rtt->getWidth()) / 2)), 0.0 + ((yStartText + 64 * offset) / (double(rtt->getHeight()) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
			}
			if (rtt != nullptr && !rendToText) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob->addObject(o);
		}
		if (text.size() > 0) {

			if (!isCenteredText) {
				// Text
				//generate_GUI_Text(ob, vhText, ((xStartText + (width / 2) - (text.size() * 16) / 4 - textOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + (height / 2) - 8) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
			else {
				// Text
				//generate_GUI_Text(ob, vhText, ((xStartText + (width / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
		}
	}
	
}

void ToggleButton::resetToggle()
{
	v = button;
	if (!cleanReset && toggle) {
		int mouseState = 1;
		if(clickLambda != NULL) clickLambda(mouseState);
	}
	toggle = false;
	isClicked = false;
	setUpdate(true);
	clickedOrder = 0;
}

/**
** Trigger the ToggleButton as if clicked.
**
** params: int mouseState; the state of the mouse event
**
**/
void ToggleButton::doToggle(int mouseState)
{
	if (button != nullptr) {
		isClicked = !isClicked;
		toggle = !toggle;
		if (toggle) setV(clicked);
		else if (getHover() && doHover) setV(hover);
		else setV(button);
		setUpdate(true);
		clickLambda(mouseState);
	}
}

/**
** Change the toggle button to either display as pressed or not. No lambda will be executed.
**
** params: bool b; the bool value
**
**/
void ToggleButton::setPressed(bool b)
{
	toggle = b;
	if (toggle) setV(clicked);
	else if (getHover() && doHover) setV(hover);
	else setV(button);
	setUpdate(true);
}


void ToggleButton::handleHover()
{
	if (!toggle && getHover() && getV() != hover) {
		if (doHover) {
			setV(hover);
			setUpdate(true);
			if (hoverLambda != NULL) {
				double x = 0.0, y = 0.0;
				hoverLambda(x, y);
			}
		}
	}
	else if (!toggle && !getHover() && getV() != button) {
		if (doHover) {
			setV(button);
			setUpdate(true);
			if (hoverLambda != NULL) {
				double x = 0.0, y = 0.0;
				hoverLambda(x, y);
			}
		}
	}
}

void ToggleButton::handleClicked(int& mouseState)
{
	if (mouseState == MOUSE_LEFT_CLICK) {
		doToggle(mouseState);
	}

}

ToggleButtonGroup::~ToggleButtonGroup()
{
	for (auto& e: toggleButtons) {
		delete e;
	}
}

void ToggleButtonGroup::createObject(DrawObjects *ob)
{
	if(!rendToText)
		for (auto& e : toggleButtons) {
			e->createObject(ob);
		}
}

void ToggleButtonGroup::createObjectNoRestriction(DrawObjects *ob) {
	for (auto& e : toggleButtons) {
		if (rendToText && rtt)
			e->setRenderToText(rtt, true);
		e->createObjectNoRestriction(ob);
	}
}

void ToggleButtonGroup::testScroll(double & scroll)
{
	if (scrollLambda != NULL)
		scrollLambda(scroll);
}

ToggleButton * ToggleButtonGroup::getElementByName(std::string name)
{
	auto it = std::find_if(toggleButtons.begin(), toggleButtons.end(), [name](ToggleButton*& e) { return (e->getName().compare(name) == 0); });
	if (it != toggleButtons.end())
		return (*it);
	return nullptr;
}

/**
* The first encountered ToggleButton that is clicked.
*@return clicked ToggleButton
*/
ToggleButton * ToggleButtonGroup::getElementByClicked()
{
	ToggleButton* tb = nullptr;
	for (auto& b : toggleButtons) {
		if (b->getClicked()) {
			tb = b;
			break;
		}
	}
	return tb;
}

/**
* The first encountered ToggleButton that is hovered.
*@return the hovered ToggleButton
*/
ToggleButton * ToggleButtonGroup::getElementByHover()
{
	ToggleButton* tb = nullptr;
	for (auto& b : toggleButtons) {
		if (b->getHover()) {
			tb = b;
			break;
		}
	}
	return tb;
}

std::vector<ToggleButton*> ToggleButtonGroup::getElementsByClicked()
{
	std::vector<ToggleButton*> items{};
	for (auto& e : toggleButtons) {
		if (e->getClicked())
			items.push_back(e);
	}
	if (doNotResetAfterToggle) {
		std::sort(items.begin(), items.end(), [](ToggleButton*& lhs, ToggleButton*& rhs) {
			return lhs->getClickedOrder() < rhs->getClickedOrder();
			});
	}
	return items;
}

std::vector<ToggleButton*> ToggleButtonGroup::getElementsByClicked(bool checkClickedOrder)
{
	std::vector<ToggleButton*> items{};
	for (auto& e : toggleButtons) {
		if (e->getClicked()) {
			if(e->getClickedOrder() == 0)
				setClickedOrder(e);
			items.push_back(e);
		}
	}
	if (doNotResetAfterToggle) {
		std::sort(items.begin(), items.end(), [](ToggleButton*& lhs, ToggleButton*& rhs) {
			return lhs->getClickedOrder() < rhs->getClickedOrder();
			});
	}
	return items;
}

void ToggleButtonGroup::addElement(GUIElement * e)
{
	if (ToggleButton* check = dynamic_cast<ToggleButton*>(e)) {
		toggleButtons.push_back(check);
	}
}

void ToggleButtonGroup::addElementOrReplace(GUIElement* e)
{
	if (ToggleButton * check = dynamic_cast<ToggleButton*>(e)) {
		for (auto& e : toggleButtons) {
			if (e->getName().compare(check->getName()) == 0) {
				e = check;
				return;
			}
		}
		toggleButtons.push_back(check);
	}
}

bool ToggleButtonGroup::getUpdate()
{
	for (auto& e : toggleButtons) {
		if (e->getUpdate()) {
			return true;
		}
	}
	return false;
}

void ToggleButtonGroup::testHover(double & xPos, double & yPos)
{
	bool test = (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height);
	if (!rendToText) {
		if (width == 0) {
			for (auto& e : toggleButtons) {
				e->testHover(xPos, yPos);
			}
		}
		else if (test) {
			for (auto& e : toggleButtons) {
				e->testHover(xPos, yPos);
			}
		}
		else if (checkOutsideOfArea) {
			for (auto& e : toggleButtons) {
				if (e->getHover())
					e->testHover(xPos, yPos);
			}
		}
		if (hoverLambda != NULL)
			hoverLambda(xPos, yPos);
	}
	else {
		double x = xPos-xStart+xRendToTextOffset, y = yPos-yStart+yRendToTextOffset;
		if (test) {
			for (auto& e : toggleButtons) {
				e->testHover(x, y);
			}
		}
		else if(checkOutsideOfArea){
			for (auto& e : toggleButtons) {
				if(e->getHover())
					e->testHover(x, y);
			}
		}
		if ((test || checkOutsideOfArea) && hoverLambda != NULL)
			hoverLambda(x, y);
	}
}

/**
** Test if the ToggleButtonGroup was clicked and whether a ToggleButton within the group was clicked.
**
** params: b; the x postion of the mouse, y; the y postion of the mouse, mouseState; the state of the mouse click; isClickedAbove; whether another GUI element has been clicked before this one.
**
** return: If an element within the group was clicked or not.
**/
bool ToggleButtonGroup::testClicked(double & xPos, double & yPos, int mouseState, bool& isClickedAbove)
{
	ToggleButton* before = currentlyPressed;
	double x = xPos, y = yPos;
	if (rendToText) {
		x = xPos - xStart + xRendToTextOffset;
		y = yPos - yStart + yRendToTextOffset;
	}
	bool test = (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height);
	bool wasClicked = false;
	if (test) {
		if (mouseState == MOUSE_LEFT_CLICK) {
			for (auto& e : toggleButtons) {
				/*if (doNotResetAfterToggle) {
					testClickedAt(x, y, mouseState, isClickedAbove);
					break;
				}else*/
				if ((wasClicked = e->testClicked(x, y, mouseState, isClickedAbove))) {
					if (before && before->getName().compare(e->getName()) == 0)
						currentlyPressed = nullptr;
					else if ((!before || before->getName().compare(e->getName()) != 0))
						currentlyPressed = e;
					break;
				}
			}

			if (!doNotResetAfterToggle) {
				for (auto& e : toggleButtons) {
					if ((!currentlyPressed || e != currentlyPressed) && (e->getClicked() || e->getToggle())) {
						e->resetToggle();
					}
				}
			}
		}
		if (clickLambda != NULL) {
			clickLambda(mouseState);
		}
	}
	return wasClicked;
}

void ToggleButtonGroup::resetAll()
{
	for (auto& e: toggleButtons) {
		e->resetToggle();
	}
	currentlyPressed = nullptr;
	clickedOrder = 0;
}

void ToggleButtonGroup::resetHover()
{
	for (auto& tb : toggleButtons) {
		if (tb->getHover()) {
			tb->setHover(false);
		}
	}
}

void ToggleButtonGroup::clearAll()
{
	for (auto& e : toggleButtons) {
		delete e;
	}
	currentlyPressed = nullptr;
	toggleButtons.clear();
	toggleButtons.shrink_to_fit();
}

void ToggleButtonGroup::setUpdate(bool b)
{
	for (auto& e : toggleButtons) {
		e->setUpdate(b);
	}
}

void ToggleButtonGroup::testClickedAt(double& xPos, double& yPos, int mouseState, bool& isClickedAbove)
{
	for (auto& e : toggleButtons) {
		if (xPos >= e->getXStart() && xPos <= e->getXStart() + e->getWidth() && yPos >= e->getYStart() && yPos <= e->getYStart() + e->getHeight()) {
			if (e->testClicked(xPos, yPos, mouseState, isClickedAbove))
				currentlyPressed = e;
			if (isClickedAbove) {
				break;
			}
		}
	}
}

void ToggleButtonGroup::setClickedOrder(ToggleButton* tb)
{
	tb->setClickedOrder(++clickedOrder);
	if (tb->getClickedOrder() != clickedOrder) clickedOrder--;
}

void ToggleButtonGroup::resetClickedOrder()
{
	clickedOrder = 0;
	for (auto& e : toggleButtons) {
		e->setClickedOrder(0);
	}
}

void ToggleButtonGroup::handleHover()
{
}

void ToggleButtonGroup::handleClicked(int & mouseState)
{
}

DropDown::DropDown(std::string name, Vertices* topSection, Vertices* middleSection, Vertices* bottomSection, Vertices* hover, int xStart, int yStart, int width, int height) : topSection(topSection), middleSection(middleSection),
				   bottomSection(bottomSection), hover(hover), xStart(xStart), yStart(yStart), width(width), height(height), GUIElement(name)
{
	tb = new ToggleButton("tb", topSection, hover, topSection, xStart, yStart, width, topSection->getHeight(), [this](int& mouseState) {
		this->toggleShowDropDown();
		this->setUpdate(true);
		});
	tb->setTextOffset(10);
	tb->setCenteredText(true);
	tb->toggleCleanReset();
}

DropDown::~DropDown()
{
	delete tb;
	for (auto& i : items) {
		delete i;
	}
}

void DropDown::add(std::string text, std::function<void()> const& lambda)
{
	int xPos = xStart, yPos = yStart;
	if (items.size() == 0) {
		items.push_back(new DropDownElement(text, topSection, hover, xPos, yPos + height + padding, width, height, lambda));
	}
	else {
		if (!(!middleSection && !bottomSection)) {
			(*items.begin())->setV(topSection);
			for (auto i = items.begin() + 1; i < items.end(); i++) {
				(*i)->setV(middleSection);
			}
			items.push_back(new DropDownElement(text, bottomSection, hover, xPos, yPos + (height * (items.size() + 1)) + (padding * items.size()), width, height, lambda));
		}else
			items.push_back(new DropDownElement(text, topSection, hover, xPos, yPos + (height * (items.size() + 1)) + (padding + padding * items.size()), width, height, lambda));
	}
}

void DropDown::createObject(DrawObjects *ob)
{
	tb->createObject(ob);
	if (showOthers) {
		for (auto& i : items) {
			i->createObject(ob);
		}
	}
}

void DropDown::testHover(double & xPos, double & yPos)
{
	if (showOthers) {
		for (auto& i : items) {
			i->testHover(xPos, yPos);
		}
	}
	tb->testHover(xPos, yPos);

}

bool DropDown::testClicked(double & xPos, double & yPos, int mouseState, bool& isClickedAbove)
{
	if (showOthers) {
		for (auto& i : items) {
			i->testClicked(xPos, yPos, mouseState, isClickedAbove);
		}
	}
	tb->testClicked(xPos, yPos, mouseState, isClickedAbove);
	return false;
}

bool DropDown::getUpdate()
{
	if (update || tb->getUpdate())
		return true;
	for (auto& i : items) {
		if (i->getUpdate())
			return true;
	}
	return false;
}

void DropDown::setUpdate(bool value)
{
	if (!value) {
		update = false;
		tb->setUpdate(false);
		for (auto& i : items) {
			i->setUpdate(false);
		}
	}
}

void DropDown::handleHover()
{
	int pos = (yPos - yStart) / (height + padding);
	if (pos < items.size() && pos != currentSelected) {
		currentSelected = pos;
		for (auto& i : items) {
			i->setHover(false);
		}
		items.at(pos)->setHover(true);
		setUpdate(true);
	}
}

void DropDown::handleClicked(int& mouseState)
{
}

DropDownElement::DropDownElement(std::string name, Vertices* section, Vertices* hover, int xStart, int yStart, int width, int height, std::function<void()> const& lambda) : section(section), hover(hover), lambda(lambda), GUIElement(name, section, xStart, yStart, width, height, name)

{
	setCenteredText(true);
}

void DropDownElement::createObject(DrawObjects *ob)
{
	
	DrawObject* o = new DrawObject((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), section->getID(), section->getVAO(), section->getTextPos());
	ob->addObject(o);
	if (getHover()) {
		DrawObject* o = new DrawObject((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), hover->getID(), hover->getVAO(), hover->getTextPos());
		ob->addObject(o);
	}
	if (name.size() > 0) {

		if (!isCenteredText) {
			// Text
			//generate_GUI_Text(ob, ((xStart + 70) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 70) / (double(screenHeightPixels) / 2)), 0, width - 36, text, "Black");
			generate_GUI_Text(ob, ((xStart - 0) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), textOffset, width - textOffset, text, "Black");
		}
		else {
			// Text
			auto textSize = Get_Text_Size(text, "Black");
			generate_GUI_Text(ob, ((xStart + (width / 2) - (textSize / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), 0, width - textOffset, text, "Black");
		}
	}
	
}

void DropDownElement::handleHover()
{
	if (getHover() && getV() != hover) {
		setV(hover);
		setUpdate(true);
	}
	else if (!getHover() && getV() != section) {
		setV(section);
		setUpdate(true);
	}
}

void DropDownElement::handleClicked(int& mouseState)
{
	if (mouseState == 1) {
		lambda();
		setV(section);
		isHover = false;
		isClicked = false;
		setUpdate(true);
	}
}

void GUIText::createObject(DrawObjects & ob)
{
	if (text.size() > 0) {

		if (!isCenteredText) {
			// Text
			//generate_GUI_Text(ob, vhText, ((xStart + 5) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), text, width - 36);
		}
		else {
			// Text
			//generate_GUI_Text(ob, vhText, ((xStart + (width / 2) - 40) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), text, width - 36);
		}
	}
}

ExpandingButton::ExpandingButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, float scaling, std::string text, std::function<void(int& mouseState)> lambda) : scaling(scaling), Button(name, button, hover, clicked, xStart, yStart, width, height, text, lambda)
{
}

void ExpandingButton::createObject(DrawObjects *ob)
{
	/*
	if (getV()) {
		if (getV() == button) {
			DrawObject* o = new DrawObject((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), button->getID(), button->getVertecesHandler()->getVAO(), button->getVertecesHandler()->getTextureID());
			ob.addObject(o);
		}
		else {
			int differenceWidth = 0, differenceHeight = 0;
			if (scaling > 1.0) {
				differenceWidth = -( (scaling * width) - width) / 2;
				differenceHeight = -((scaling * height) - height) / 2;
			}
			else if(scaling < 1.0){
				differenceWidth = -(scaling * width);
				differenceHeight = -(scaling * height);
			}
			DrawObject* o = new DrawObject(( (xStart + differenceWidth ) / (double(screenWidthPixels) / 2)), 0.0 + ( (yStart + differenceHeight) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVertecesHandler()->getVAO(), getV()->getVertecesHandler()->getTextureID());
			o->setScale(scaling);
			ob.addObject(o);
		}
	}
	*/
}

void GUILabel::createObject(DrawObjects *ob)
{
	if (getV()) {
		o = new DrawObject((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
		ob->addObject(o);
		generate_GUI_Text(ob, ((xStart - 0) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), textStartOffset, width - 16, textRef, textType);
		
	}
}

GUIGroup::~GUIGroup()
{
	clear();
}

void GUIGroup::clear()
{
	for (auto e : elements) {
		delete e;
	}
	elements.clear();
	elements.shrink_to_fit();
}

void GUIGroup::createObject(DrawObjects* ob)
{
	if (!rendToText) {
		for (auto e : elements) {
			e->createObject(ob);
		}
	}
}

void GUIGroup::createObjectNoRestriction(DrawObjects* ob)
{
	for (auto e : elements) {
		if (rendToText && rtt)
			e->setRenderToText(rtt, rendToText);
		e->createObjectNoRestriction(ob);
	}
}

void GUIGroup::addElement(GUIElement * e)
{
	elements.push_back(e);
}

GUIElement * GUIGroup::getElementByName(std::string name)
{
	auto it = std::find_if(elements.begin(), elements.end(), [name](GUIElement*& e) { return (e->getName().compare(name) == 0); });
	if (it != elements.end())
		return (*it);
	return nullptr;
}

void GUIGroup::removeElementByName(std::string name)
{
	auto it = std::find_if(elements.begin(), elements.end(), [name](GUIElement*& e) { return (e->getName().compare(name) == 0); });
	if (it != elements.end()) {
		delete (*it);
		elements.erase(it);
	}
}

void GUIGroup::testHover(double & xPos, double & yPos)
{
	if (hoverLambda != nullptr)
		hoverLambda(xPos, yPos);
}

bool GUIGroup::testClicked(double & xPos, double & yPos, int mouseState, bool & isClickedAbove)
{
	return false;
}

void GUIGroup::updateStartX(int x)
{
	this->xStart = x;
	for (auto e : elements) {
		e->setStartX(x);
	}
}

void GUIGroup::setStartY(int y)
{
	this->yStart = y;
	for (auto e : elements) {
		e->setStartX(y);
	}

}

int GUIGroup::getAmount()
{
	return elements.size();
}

void GUIGroup::handleHover()
{
}

void GUIGroup::handleClicked(int & mouseState)
{
}

ScrollbarVertical::ScrollbarVertical(std::string name, int xStart, int yStart, int width, int height, Button* topButton, Button* bottomButton, Vertices* backgroundPart, Vertices* barTopPart, Vertices* barBottomPart, Vertices* barMiddlePart, Vertices* barMiddlePartHover, Vertices* barMiddlePartClicked) :
	name(name), topButton(topButton), bottomButton(bottomButton),
	backgroundPart(backgroundPart),
	barTopPart(barTopPart), barBottomPart(barBottomPart),
	barMiddlePart(barMiddlePart), barMiddlePartHover(barMiddlePartHover), barMiddlePartClicked(barMiddlePartClicked),
	GUIElement(name, xStart, yStart, width, height) 
{
	
	scrollbarPosition = topButton->getHeight();
	if (!barTopPart && !barBottomPart)
		ignoreEndPartsBar = true;
	scrollbarLength = height - (topButton->getHeight() + bottomButton->getHeight());
	tempBackground = new Vertices("paletteModifierLeftScrollbarBackground", backgroundPart->getWidth(), scrollbarLength, backgroundPart);
	if(ignoreEndPartsBar)
		tempBarBody = new Vertices("paletteModifierLeftScrollbarBarBody", barMiddlePart->getWidth(), scrollbarLength, barMiddlePart);
	else
		tempBarBody = new Vertices("paletteModifierLeftScrollbarBarBody", barMiddlePart->getWidth(), scrollbarLength - (barTopPart->getHeight() + barBottomPart->getHeight()), barMiddlePart);
};

ScrollbarVertical::~ScrollbarVertical()
{
	delete topButton;
	delete bottomButton;
	if (tempBackground != nullptr) {
		removeVertices(tempBackground);
		tempBackground = nullptr;
	}
	if (tempBarBody != nullptr) {
		removeVertices(tempBarBody);
		tempBarBody = nullptr;
	}
}

void ScrollbarVertical::createObject(DrawObjects *ob)
{
	
	if (!rendToText) {
		int heightReached = 0, heightLimit = height - bottomButton->getHeight();
		{ // Create background and buttons
			// Top button
			topButton->createObject(ob);
			heightReached += topButton->getHeight();

			// Background
			ob->addObject(new DrawObject(xStart / (double(screenWidthPixels) / 2), (yStart + heightReached) / (double(screenHeightPixels) / 2), tempBackground->getID(), tempBackground->getVAO(), tempBackground->getTextPos()));
			heightReached += tempBackground->getHeight();

			// Bottom button
			bottomButton->setStartY(this->yStartText + heightReached);
			bottomButton->setYStartText(this->yStartText + heightReached);
			bottomButton->createObject(ob);
		}

		
		{ // Create the scrollbar bar
			heightReached = scrollbarPosition, heightLimit = height - bottomButton->getHeight();
			if (!ignoreEndPartsBar) {
				//ob.addObject(new Object(xStart / (double(screenWidthPixels) / 2), (yStart + heightReached) / (double(screenHeightPixels) / 2), barTopPart->getID(), barTopPart->getVertecesHandler()->getVAO(), barTopPart->getVertecesHandler()->getTextureID()));
				heightReached += barTopPart->getHeight();
			}
			
			ob->addObject(new DrawObject(xStart / (double(screenWidthPixels) / 2), (yStart + heightReached) / (double(screenHeightPixels) / 2), tempBarBody->getID(), tempBarBody->getVAO(), tempBarBody->getTextPos()));
			heightReached += tempBarBody->getHeight();

			if (!ignoreEndPartsBar) {
				//ob.addObject(new Object(xStart / (double(screenWidthPixels) / 2), (yStart + heightReached) / (double(screenHeightPixels) / 2), barTopPart->getID(), barTopPart->getVertecesHandler()->getVAO(), barTopPart->getVertecesHandler()->getTextureID()));
				heightReached += barBottomPart->getHeight();
			}
		}
		
	}
}

void ScrollbarVertical::createObjectNoRestriction(DrawObjects *ob)
{
}

void ScrollbarVertical::testHover(double& xPosRef, double& yPosRef)
{
	topButton->testHover(xPosRef, yPosRef);
	bottomButton->testHover(xPosRef, yPosRef);
	bool test = (xPosRef >= xStart && xPosRef <= xStart + width && yPosRef >= yStart + scrollbarPosition && yPosRef <= yStart + scrollbarPosition + scrollbarLength);
	if (!isClicked) {
		if (!isHover && test) {
			isHover = true;
			update = true;
			tempBarBody->copyAnotherVerticesText(barMiddlePartHover);

		}
		else if (isHover && !test) {
			isHover = false;
			update = true;
			tempBarBody->copyAnotherVerticesText(barMiddlePart);
		}
	}
	else {
		if(yPosRef - mouseYPos != 0 || testOnClick)
			updateScrollbarPosition(scrollbarPosition + (yPosRef - mouseYPos), true);
		mouseYPos = yPosRef;
	}
	
}

bool ScrollbarVertical::testClicked(double& xPosRef, double& yPosRef, int mouseState, bool& isClickedAbove)
{
	topButton->testClicked(xPosRef, yPosRef, mouseState, isClickedAbove);
	bottomButton->testClicked(xPosRef, yPosRef, mouseState, isClickedAbove);
	bool test = (xPosRef >= xStart && xPosRef <= xStart + width && yPosRef >= yStart + scrollbarPosition && yPosRef <= yStart + scrollbarPosition + scrollbarLength);
	if (mouseState == 1 && !isClicked && test) {
		isClicked = true;
		update = true;
		tempBarBody->copyAnotherVerticesText(barMiddlePartClicked);
		mouseYPos = yPosRef;
	}
	else if (isClicked) {
		isClicked = false;
		update = true;
		if(isHover)
			tempBarBody->copyAnotherVerticesText(barMiddlePartHover);
		else
			tempBarBody->copyAnotherVerticesText(barMiddlePart);
	}
	return isClicked;
}

bool ScrollbarVertical::getUpdate()
{
	if (topButton->getUpdate()) return true;
	if (bottomButton->getUpdate()) return true;
	if (update) return true;
	return false;
}

void ScrollbarVertical::setUpdate(bool b)
{
	topButton->setUpdate(b);
	bottomButton->setUpdate(b);
	update = b;
}

void ScrollbarVertical::updateScrollbarLengthAndPosition(int length, int pos)
{
	scrollbarLength = length;
	if (scrollbarLength < scrollbarLengthMin)
		scrollbarLength = scrollbarLengthMin;
	scrollbarPosition = pos;
	if (scrollbarPosition < topButton->getHeight())
		scrollbarPosition = topButton->getHeight();
	if (scrollbarLength + scrollbarPosition > height - bottomButton->getHeight()) {
		if (scrollbarLength > height - bottomButton->getHeight() - topButton->getHeight())
			scrollbarLength = height - bottomButton->getHeight() - topButton->getHeight();
		scrollbarPosition = height - bottomButton->getHeight() - scrollbarLength;
	}
	tempBarBody->changeSize(barMiddlePart->getWidth(), scrollbarLength);
	update = true;
	if (incrementLambda)
		incrementLambda();
}

void ScrollbarVertical::updateScrollbarLengthAndPositionAndIncrementSize(int length, int pos, int end)
{
	scrollbarLength = length;
	if (scrollbarLength < scrollbarLengthMin)
		scrollbarLength = scrollbarLengthMin;
	scrollbarPosition = pos;
	setIncrementSize( double(end) / getScrollbarIncrementsPoints());
	if (incrementSize > 10000) incrementSize = 0;
	updateScrollbarLengthAndPosition(length, pos);
}

void ScrollbarVertical::updateScrollbarPosition(double pos, bool useLambda = true)
{
	if (scrollbarPosition != pos) {
		changeInPosition = pos - scrollbarPosition;
		scrollbarPosition = pos;
		if (scrollbarPosition < topButton->getHeight()) {
			scrollbarPosition = topButton->getHeight();
			changeInPosition = 0;
		}
		if (scrollbarLength + scrollbarPosition > height - bottomButton->getHeight()) {
			scrollbarPosition = height - bottomButton->getHeight() - scrollbarLength;
			changeInPosition = 0;
		}
		update = true;
		if (useLambda && incrementLambda)
			incrementLambda();
	}
}

ScaleableElement::ScaleableElement(std::string name, int xStart, int yStart, int width, int height, Vertices* vertices) : GUIElement(name, xStart, yStart, width, height)
{
	v = new Vertices(name, width, height, vertices);
}

ScaleableElement::~ScaleableElement()
{
	if (v != nullptr) {
		removeVertices(v);
		v = nullptr;
	}
}

void ScaleableElement::createObject(DrawObjects *ob)
{
	if (!rendToText) {
		
		auto o = new DrawObject(xStart / (double(screenWidthPixels) / 2), (yStart) / (double(screenHeightPixels) / 2), v->getID(), v->getVAO(), v->getTextPos());
		if (rtt != nullptr) o->setRendToText(rtt);
		o->setOffsetX(xOffset / textureWidth);
		o->setOffsetY(yOffset / textureHeight);
		ob->addObject(o);
		
	}
}

void ScaleableElement::createObjectNoRestriction(DrawObjects *ob)
{
}

TextField::TextField(std::string name, int xStart, int yStart, int width, int height, Vertices* field, Vertices* fieldFocus) : field(field), fieldFocus(fieldFocus), GUIElement(name, xStart, yStart, width, height)
{
	setV(field);
	textStartYOffset = (height - textSize) / 2;
	rowLength = width - (textStartXOffset + textSize);
}

TextField::~TextField()
{
	delete field;
	delete fieldFocus;
}

/**
*
* Draw the contents of the TextField.
*
*@param ob The buffer which stores the objects to be drawn onto the screen.
*/
void TextField::createObject(DrawObjects* ob)
{
	if (!rendToText) {
		if (v && show) {

			auto* o = new DrawObject(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
			if (rtt != nullptr) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob->addObject(o);

		}
		{ // Do text stuff
			float xStart = ((xStartText + xDrawOffset + textStartXOffset) / (double(screenWidthPixels) / 2)), yStart = 0.0 + ((yStartText + yDrawOffset + textStartYOffset) / (double(screenHeightPixels) / 2)), x = xStart, y = yStart;
			std::string textType = "Black";

			if (!marker) {
				auto& letterInformation = letterLibrary.getLetterInformation(std::string(1, '|') + textType);
				if(letterInformation.size() > 0)
					marker = new DrawObject(0, 0, letterInformation[2], letterInformation[0], letterInformation[1]);
			}

			std::vector<DrawObject*> objects;
			int character = 0, pos = 0, textLength = letterLibrary.getWidthInPixels(text, textType);
			float offset = 0.0f, maxOffset = x + (rowLength / (double(screenWidthPixels) / 2));

			if (textLength > rowLength)
				offset -= ((textLength - rowLength) / (double(screenWidthPixels) / 2));

			for (auto& letter : text) {
				character = letter;
				auto& letterInformation = letterLibrary.getLetterInformation(std::string(1, letter) + textType);
				if (letterInformation.size() > 0) {
					offset -= ((letterInformation[3]) / (double(screenWidthPixels) / 2));
					if (pos == 0)
						xStart -= ((letterInformation[3]) / (double(screenWidthPixels) / 2));
					objects.push_back(new DrawObject(x + offset, y, letterInformation[2], letterInformation[0], letterInformation[1]));
					offset += ((letterInformation[4] + 1) / (double(screenWidthPixels) / 2));
				}
				if (offset > maxOffset) {
					
				}
				pos++;
				if (pos == markerPos && focus && marker->getDraw()) {
					marker->setXPosition(x + offset - (3 / (double(screenWidthPixels) / 2)));
					marker->setYPosition(y);
					objects.push_back(new DrawObject(marker));
				}
			}

			pos = 0;
			for (auto o : objects) {
				if (o->getXPosition() >= xStart && o->getXPosition() <= maxOffset) {
					ob->addObject(objects[pos]);
					objects[pos] = nullptr;
				}
				else
					delete o;
				pos++;
			}

			if ((text.size() == 0 || markerPos == 0) && focus && marker->getDraw()) {
				marker->setXPosition(x - (3 / (double(screenWidthPixels) / 2)));
				marker->setYPosition(yStart);
				ob->addObject(new DrawObject(marker));
			}
		}
	}
}

/**
* Handle incoming clicks.
*
*@param mouseState The state of the mouse (left click == 1, right click = 0, middle click = 2).
*/
void TextField::handleClicked(int& mouseState)
{
	if (mouseState == 1 && !focus) {
		focus = true;
		setV(fieldFocus);
		setUpdate(true);
	}
	else if(mouseState == 1 && focus){
		focus = false;
		setV(field);
		setUpdate(true);
	}
}

/**
* Handle incoming key strokes if currently focused.
*
*@param key The keycode. 
*@param action The action of the key stroke (pressed, released or held). 
*@param mods Determines if a modifier is currently toggled (alt, shift, control...).
*/
void TextField::handleKeyStroke(int& key, int& action, int& mods)
{
	if (key >= 321 && key <= 329) key -= 272;
	if (focus && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if (action == GLFW_REPEAT) repeat = true;
		int temp = key; bool change = false;
		if (key == 259 && markerPos != 0 && text.size() > 0) {
			if (text.size() == 1 || mods != GLFW_MOD_CONTROL) {
				text.erase((markerPos--) - 1, 1);
				change = true;
			}
			else{
				for (auto it = text.begin() + markerPos - 1; it != text.begin(); it--) {
					if ((*it) == ' ') {
						text.erase(it, text.end());
						markerPos--;
						change = true;
						break;
					}else if(it == (text.begin() + 1)){
						text.erase(text.begin(), text.end());
						markerPos = 0;
						change = true;
						break;
					}
					markerPos--;
				}
			}
		}
		else if(key >= 32 && key <= 127){
			if (mods != GLFW_MOD_SHIFT && temp >= 65 && temp <= 90)
				temp += 32;
			text.insert(markerPos++, 1, (char)temp);
			change = true;
		}
		else if (key == 262 && markerPos < text.size()) {
			markerPos++;
		}
		else if (key == 263 && markerPos > 0) {
			markerPos--;
		}
		setUpdate(true);
		if (change && triggerOnKeyStroke && lambda && action != GLFW_REPEAT)
			lambda();
	}
	else if (focus && action == GLFW_RELEASE && repeat == true) {
		repeat = false;
		lambda();
	}
}

/**
* Test if the TextField id being clicked.
*
*@param xPos The mouse's x position.
*@param yPos The mouse's y position.
*@param mouseState The state of the mouse (left click == 1, right click = 0, middle click = 2).
*@param isClickedAbove Determines if this element is the top most element and thus preventing checks on other elements.
*/
bool TextField::testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove)
{
	bool test = (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height);
	if ((mouseState == 1 || mouseState == 0) && test) {
		handleClicked(mouseState);
		setV(fieldFocus);
	}
	else if(focus){
		focus = false;
		setV(field);
		setUpdate(true);
	}
	return false;
}
