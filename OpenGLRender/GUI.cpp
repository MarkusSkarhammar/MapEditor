#include "GUI.h"
#include "Global.h"
#include <algorithm>
#include "ElementCreator.h"


GUIElement::GUIElement(std::string name): name(name)
{
}

GUIElement::GUIElement(std::string name, int vao, int textPos, int id, int xStart, int yStart, int width, int height, std::string text = "") : name(name), VAO(vao), textPos(textPos), id(id), xStart(xStart), yStart(yStart), width(width), height(height), text(text)
{
}

void GUIElement::testHover(double& xPos, double& yPos)
{
	if (!isHover && xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		isHover = true;
		handleHover();
	}
	else if (isHover && !(xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) ){
		isHover = false;
		handleHover();
	}
}

void GUIElement::testClicked(double & xPos, double & yPos, int mouseState)
{
	if (mouseState == 1 && !isClicked && xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		isClicked = true;
		handleClicked(mouseState);
	}
	else if (isClicked) {
		isClicked = false;
		handleClicked(mouseState);
	}
}

void GUIElement::createObject(Objects & ob)
{
	ob.addObject(new Object( (xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), id, this->VAO, textPos));
	if (text.size() > 0) {
		VertecesHandler vhText = VertecesHandler::findByName(verteces, "Letters_");
		// Text
		generate_GUI_Text(ob, vhText, ((xStart + 70) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 70) / (double(screenHeightPixels) / 2)), text , width-36);
	}
}

std::string & GUIElement::getName()
{
	return name;
}

void GUIElement::setID(int & id)
{
	this->id = id;
}

int & GUIElement::getID()
{
	return id;
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

void GUIElement::setText(std::string s)
{
	text = s;
	setUpdate(true);
}

void GUIElement::handleHover()
{
}

void GUIElement::handleClicked(int& mouseState)
{
}

GUIPanel::GUIPanel(std::string name): name(name)
{
}

GUIPanel::GUIPanel(std::string name, int x, int y, int width, int height): name(name), xStart(x), yStart(y), width(width), height(height)
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
	elements.push_back(e);
}

GUIElement* GUIPanel::getElementByName(std::string name)
{
	auto it = std::find_if(elements.begin(), elements.end(), [name](GUIElement*& e) { return (e->getName().compare(name) == 0); });
	if (it != elements.end())
		return (*it);
	return nullptr;
}

void GUIPanel::removeElementByName(std::string name)
{
	auto it = std::find_if(elements.begin(), elements.end(), [name](GUIElement*& e) { return (e->getName().compare(name) == 0); });
	if (it != elements.end()) {
		delete (*it);
		elements.erase(it);
	}
}

void GUIPanel::createObjects(Objects & ob)
{
	for (auto& e : elements) {
		e->createObject(ob);
	}
}

void GUIPanel::setDimensions(int x, int y, int width, int height)
{
	this->xStart = x;
	this->yStart = y;
	this->width = width;
	this->height = height;
}

void GUIPanel::checkHover(Objects& ob, double & xPos, double & yPos)
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
	checkUpdates(ob);
}

void GUIPanel::checkClicked(Objects& ob, double & xPos, double & yPos, int mouseState)
{
	if (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		for (auto& e : elements) {
			e->testClicked(xPos, yPos, mouseState);
		}
	}
	checkUpdates(ob);
}

void GUIPanel::checkUpdates(Objects & ob)
{
	bool update = false;
	for (auto& e : elements) {
		if (e->getUpdate()) {
			update = true;
			e->setUpdate(false);
		}
	}
	if (update) {
		ob.clearObjects();
		createObjects(ob);
	}
}

Button::Button(std::string name, int vao, int textPos, int id1, int id2, int id3, int xStart, int yStart, int width, int height, std::string text = "", std::function<void()> const & lambda = []() {}) : buttonID(id1), hoverID(id2), clickedID(id3), lambda(lambda), GUIElement(name, vao, textPos, id1, xStart, yStart, width, height, text)
{
}

void Button::handleHover()
{
	if (!getClicked() && getHover() && getID() != hoverID) {
		setID(hoverID);
		setUpdate(true);
	}
	else if (!getClicked() && !getHover() && getID() != buttonID) {
		setID(buttonID);
		setUpdate(true);
	}
}

void Button::handleClicked(int& mouseState)
{
	if (mouseState == 1 && getClicked() && getID() != clickedID) {
		setID(clickedID);
		setUpdate(true);
	}
	else if(mouseState == 0 && !getClicked() && getID() == clickedID){
		lambda();
		if (getHover()) {
			setID(hoverID);
		}else
			setID(buttonID);
		setUpdate(true);
	}
}



ToggleButton::ToggleButton(std::string name, int vao, int textPos, int id1, int id2, int id3, int xStart, int yStart, int width, int height, std::string text = "", std::function<void()> const & lambda = []() {}) : buttonID(id1), hoverID(id2), clickedID(id3), lambda(lambda), GUIElement(name, vao, textPos, id1, xStart, yStart, width, height, text)
{
}

void ToggleButton::resetToggle()
{
	setID(buttonID);
	if (toggle) {
		toggle = false;
		lambda();
	}
	isClicked = false;
	setUpdate(true);
}


void ToggleButton::handleHover()
{
	if (!toggle && getHover() && getID() != hoverID) {
		setID(hoverID);
		setUpdate(true);
	}
	else if (!toggle && !getHover() && getID() != buttonID) {
		setID(buttonID);
		setUpdate(true);
	}
}

void ToggleButton::handleClicked(int& mouseState)
{
	if (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		toggle = !toggle;
		lambda();
		if(toggle) setID(clickedID);
		else if(getHover()) setID(hoverID);
		else setID(buttonID);
		setUpdate(true);
	}

}

ToggleButtonGroup::~ToggleButtonGroup()
{
	for (auto& e: toggleButtons) {
		delete e;
	}
}

void ToggleButtonGroup::createObject(Objects & ob)
{
	for (auto& e : toggleButtons) {
		e->createObject(ob);
	}
}

ToggleButton * ToggleButtonGroup::getElementByName(std::string name)
{
	auto it = std::find_if(toggleButtons.begin(), toggleButtons.end(), [name](ToggleButton*& e) { return (e->getName().compare(name) == 0); });
	if (it != toggleButtons.end())
		return (*it);
	return nullptr;
}

void ToggleButtonGroup::addElement(GUIElement * e)
{
	if (ToggleButton* check = dynamic_cast<ToggleButton*>(e)) {
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
	for (auto& e : toggleButtons) {
		e->testHover(xPos, yPos);
	}
}

void ToggleButtonGroup::testClicked(double & xPos, double & yPos, int mouseState)
{
	ToggleButton* before = currentlyPressed;
	for (auto& e : toggleButtons) {
		e->testClicked(xPos, yPos, mouseState);
		if ((before == nullptr || before->getName().compare(e->getName()) != 0) && e->getToggle()) {
			currentlyPressed = e;
		}
	}
	if (before != nullptr && before->getName().compare(currentlyPressed->getName()) != 0) {
		for (auto& e : toggleButtons) {
			if (e != currentlyPressed) {
				e->resetToggle();
			}
		}
	}
}

void ToggleButtonGroup::resetAll()
{
	for (auto& e: toggleButtons) {
		e->resetToggle();
	}
}

void ToggleButtonGroup::handleHover()
{
}

void ToggleButtonGroup::handleClicked(int & mouseState)
{
}
