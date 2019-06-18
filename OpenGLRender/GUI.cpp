#include "GUI.h"
#include "Global.h"
#include <algorithm>
#include "ElementCreator.h"
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
		bool test = (xPosRef >= xStart && xPosRef <= xStart + width && yPosRef >= yStart && yPosRef <= yStart + height);
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
		bool test = (xPosRef >= xStart && xPosRef <= xStart + width && yPosRef >= yStart && yPosRef <= yStart + height);
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

void GUIElement::testClicked(double & xPos, double & yPos, int mouseState, bool& isClickedAbove)
{
	if (show && mouseState == 1 && !isClicked && xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		isClickedAbove = true;
		isClicked = true;
		handleClicked(mouseState);
	}
	else if (show && isClicked) {
		isClicked = false;
		handleClicked(mouseState);
	}
}

void GUIElement::createObject(Objects & ob)
{
	if (!rendToText) {
		if (v && show) {
			auto* o = new Object((xStartText / (double(screenWidthPixels) / 2)), 0.0 + (yStartText / (double(screenHeightPixels) / 2)), v->getID(), v->getVertecesHandler()->getVAO(), v->getVertecesHandler()->getTextureID());
			if (rtt != nullptr) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob.addObject(o);
		}
		if (text.size() > 0) {
			VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

			if (!isCenteredText) {
				// Text
				generate_GUI_Text(ob, vhText, ((xStartText) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
			else {
				// Text
				generate_GUI_Text(ob, vhText, ((xStartText + (width / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
		}
	}
}

void GUIElement::createObjectNoRestriction(Objects & ob) {
	if (v && show) {
		auto* o = new Object((xStartText / (double(screenWidthPixels) / 2)), 0.0 + (yStartText / (double(screenHeightPixels) / 2)), v->getID(), v->getVertecesHandler()->getVAO(), v->getVertecesHandler()->getTextureID());
		if(rtt != nullptr) o->setRendToText(rtt);
		o->setOffsetX(xOffset / textureWidth);
		o->setOffsetY(yOffset / textureHeight);
		ob.addObject(o);
	}
	if (text.size() > 0) {
		VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

		if (!isCenteredText) {
			// Text
			generate_GUI_Text(ob, vhText, ((xStartText) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
		}
		else {
			// Text
			generate_GUI_Text(ob, vhText, ((xStartText + (width / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
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
	if (show) {
		for (auto& e : elements) {
			e->createObject(ob);
		}
	}
}

void GUIPanel::reCreateObjects(Objects & ob)
{
	ob.clearObjects();
	createObjects(ob);
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
		for (auto e = elements.end()-1; e != elements.begin(); e-- ) {
			if (!isClicked)
				(*e)->testClicked(xPos, yPos, mouseState, isClicked);
			else
				break;
		}
	}
	isClicked = false;
	checkUpdates(ob);
}

void GUIPanel::checkScroll(Objects& ob, double& xPos, double& yPos, double& scroll)
{
	if (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		for (auto& e : elements)
			e->testScroll(scroll);
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

void GUIPanel::updateLabels(Objects & ob)
{
	for (auto& e : elements) {
		if (GUILabel* l = dynamic_cast<GUILabel*>(e)) {
			l->update(ob);
		}
	}
}

Button::Button(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::string text = "", std::function<void(int& mouseState)> lambda = [](int& mouseState) {}) : GUIElement(name, button, xStart, yStart, width, height, text)
{
	clickLambda = lambda;
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


void ToggleButton::createObject(Objects & ob)
{
	if (show && !rendToText && (alwaysShowBase || getV())) {
		if ((alwaysShowBase && button) || (button && getV() == button)) {
			auto* o = new Object(((xStartText + 64 * offset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + 64 * offset) / (double(screenHeightPixels) / 2)), button->getID(), button->getVertecesHandler()->getVAO(), button->getVertecesHandler()->getTextureID());
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob.addObject(o);
		}
		if (getV() && !(getV() == button)) {
			auto* o = new Object(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVertecesHandler()->getVAO(), getV()->getVertecesHandler()->getTextureID());
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob.addObject(o);
		}
		if (text.size() > 0) {
			VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

			if (!isCenteredText) {
				// Text
				generate_GUI_Text(ob, vhText, ((xStartText + (width / 2) - (text.size() * 16) / 4 - textOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + (height / 2) - 8) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
			else {
				// Text
				generate_GUI_Text(ob, vhText, ((xStartText + (width / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
		}
	}
}

void ToggleButton::createObjectNoRestriction(Objects & ob) {
	if (show && (alwaysShowBase || getV())) {
		if ((alwaysShowBase && button) || (button && getV() == button)) {
			auto* o = new Object(((xStartText + 64 * offset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + 64 * offset) / (double(screenHeightPixels) / 2)), button->getID(), button->getVertecesHandler()->getVAO(), button->getVertecesHandler()->getTextureID());
			if (rtt != nullptr) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob.addObject(o);
		}
		if (getV() && !(getV() == button)) {
			auto* o = new Object(((xStartText + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVertecesHandler()->getVAO(), getV()->getVertecesHandler()->getTextureID());
			if (rtt != nullptr) o->setRendToText(rtt);
			o->setOffsetX(xOffset / textureWidth);
			o->setOffsetY(yOffset / textureHeight);
			ob.addObject(o);
		}
		if (text.size() > 0) {
			VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

			if (!isCenteredText) {
				// Text
				generate_GUI_Text(ob, vhText, ((xStartText + (width / 2) - (text.size() * 16) / 4 - textOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText + (height / 2) - 8) / (double(screenHeightPixels) / 2)), text, width - 36);
			}
			else {
				// Text
				generate_GUI_Text(ob, vhText, ((xStartText + (width / 2)) / (double(screenWidthPixels) / 2)), 0.0 + ((yStartText) / (double(screenHeightPixels) / 2)), text, width - 36);
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
	if (mouseState == 1) {
		if (button != nullptr) {
			toggle = !toggle;
			if (toggle) setV(clicked);
			else if (getHover() && doHover) setV(hover);
			else setV(button);
			setUpdate(true);
		}
		clickLambda(mouseState);
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

void ToggleButtonGroup::createObjectNoRestriction(Objects & ob) {
	for (auto& e : toggleButtons) {
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
	if (!rendToText) {
		bool test = (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height);
		if (width == 0) {
			for (auto& e : toggleButtons) {
				e->testHover(xPos, yPos);
			}
		}
		else if (test) {
			for (auto& e : toggleButtons) {
				e->testHover(xPos, yPos);
			}
			if (hoverLambda != NULL)
				hoverLambda(xPos, yPos);
		}
	}
	else {
		bool test = (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height);
		double x = xPos-xStart+xRendToTextOffset, y = yPos-yStart+yRendToTextOffset;
		if (test || checkOutside) {
			for (auto& e : toggleButtons) {
				e->testHover(x, y);
			}
		}
		if ((test || checkOutsideOfArea) && hoverLambda != NULL)
			hoverLambda(x, y);
	}
}

void ToggleButtonGroup::testClicked(double & xPos, double & yPos, int mouseState, bool& isClickedAbove)
{
	ToggleButton* before = currentlyPressed;
	double x = xPos, y = yPos;
	if (rendToText) {
		x = xPos - xStart + xRendToTextOffset;
		y = yPos - yStart + yRendToTextOffset;
	}

	if (mouseState == 1) {
		for (auto& e : toggleButtons) {
			e->testClicked(x, y, mouseState, isClickedAbove);
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
	if (clickLambda != NULL) {
		bool test = (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height);
		if(test) clickLambda(mouseState);
	}
}

void ToggleButtonGroup::resetAll()
{
	for (auto& e: toggleButtons) {
		e->resetToggle();
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

void ToggleButtonGroup::handleHover()
{
}

void ToggleButtonGroup::handleClicked(int & mouseState)
{
}

DropDown::DropDown(std::string name, Vertices* topSection, Vertices* middleSection, Vertices* bottomSection, Vertices* hover, int xStart, int yStart, int width, int height) : topSection(topSection), middleSection(middleSection),
				   bottomSection(bottomSection), hover(hover), xStart(xStart), yStart(yStart), width(width), height(height), GUIElement(name)
{
	show = false;
}

DropDown::~DropDown()
{
	for (auto& i : items) {
		delete i;
	}
}

void DropDown::add(std::string text, std::function<void()> const& lambda)
{
	int xPos = xStart, yPos = yStart;
	if (items.size() == 0) {
		items.push_back(new DropDownElement(text, topSection, hover, xPos, yPos + padding, width, height, lambda));
	}
	else {
		if (!(!middleSection && !bottomSection)) {
			(*items.begin())->setV(topSection);
			for (auto i = items.begin() + 1; i < items.end(); i++) {
				(*i)->setV(middleSection);
			}
			items.push_back(new DropDownElement(text, bottomSection, hover, xPos, yPos + (height * items.size()) + (padding * items.size()), width, height, lambda));
		}else
			items.push_back(new DropDownElement(text, topSection, hover, xPos, yPos + (height * items.size()) + (padding + padding * items.size()), width, height, lambda));
	}
}

void DropDown::createObject(Objects & ob)
{
	if (show) {
		for (auto& i : items) {
			i->createObject(ob);
		}
	}
}

void DropDown::testHover(double & xPos, double & yPos)
{
	if (show) {
		for (auto& i : items) {
			i->testHover(xPos, yPos);
		}
	}


}

void DropDown::testClicked(double & xPos, double & yPos, int mouseState, bool& isClickedAbove)
{
	if (show) {
		for (auto& i : items) {
			i->testClicked(xPos, yPos, mouseState, isClickedAbove);
		}
	}
}

bool DropDown::getUpdate()
{
	for (auto& i : items) {
		if (i->getUpdate())
			return true;
	}
	return false;
}

void DropDown::setUpdate(bool value)
{
	if (!value) {
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
	setCentered(true);
}

void DropDownElement::createObject(Objects & ob)
{
	Object* o = new Object((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), section->getID(), section->getVertecesHandler()->getVAO(), section->getVertecesHandler()->getTextureID());
	ob.addObject(o);
	if (getHover()) {
		Object* o = new Object((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), hover->getID(), hover->getVertecesHandler()->getVAO(), hover->getVertecesHandler()->getTextureID());
		ob.addObject(o);
	}
	if (name.size() > 0) {
		VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

		if (!isCenteredText) {
			// Text
			generate_GUI_Text(ob, vhText, ((xStart + 70) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 70) / (double(screenHeightPixels) / 2)), text, width - 36);
		}
		else {
			// Text
			generate_GUI_Text(ob, vhText, ((xStart + (width / 2) - 40) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), text, width - 36);
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
		setUpdate(true);
	}
}

void GUIText::createObject(Objects & ob)
{
	if (text.size() > 0) {
		VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

		if (!isCenteredText) {
			// Text
			generate_GUI_Text(ob, vhText, ((xStart + 5) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), text, width - 36);
		}
		else {
			// Text
			generate_GUI_Text(ob, vhText, ((xStart + (width / 2) - 40) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), text, width - 36);
		}
	}
}

ExpandingButton::ExpandingButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, float scaling, std::string text, std::function<void(int& mouseState)> lambda) : scaling(scaling), Button(name, button, hover, clicked, xStart, yStart, width, height, text, lambda)
{
}

void ExpandingButton::createObject(Objects & ob)
{
	if (getV()) {
		if (getV() == button) {
			Object* o = new Object((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), button->getID(), button->getVertecesHandler()->getVAO(), button->getVertecesHandler()->getTextureID());
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
			Object* o = new Object(( (xStart + differenceWidth ) / (double(screenWidthPixels) / 2)), 0.0 + ( (yStart + differenceHeight) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVertecesHandler()->getVAO(), getV()->getVertecesHandler()->getTextureID());
			o->setScale(scaling);
			ob.addObject(o);
		}
	}
}

void GUILabel::createObject(Objects & ob)
{
	if (getV()) {
		o = new Object((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVertecesHandler()->getVAO(), getV()->getVertecesHandler()->getTextureID());
		ob.addObject(o);

		VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");
		
		size = text.size() + textRef.size();

		// Text
		generate_GUI_Text(ob, vhText, ((xStart + 5) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), text + textRef);
	}
}

void GUILabel::update(Objects & ob)
{
	auto& list = ob.getObjects();
	auto& object = o;
	auto it = std::find_if(list.begin(), list.end(), [object](Object* os) { return os == object; });
	if (it != list.end()) {
		it++;
		for (auto i = it; i != it + size-2; i++ ) {
			delete *i;
		}
		list.erase(it, it + size-2);

		VertecesHandler*& vhText = VertecesHandler::findByName(verteces, "Letters_");

		size = text.size() + textRef.size();

		std::vector<Object*> temp;
		// Text
		generate_GUI_Text(temp, vhText, ((xStart + 5) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 5) / (double(screenHeightPixels) / 2)), text + textRef);

		it = std::find_if(list.begin(), list.end(), [object](Object* os) { return os == object; });
		list.insert(it, temp.begin(), temp.end());
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
}

void GUIGroup::createObject(Objects & ob)
{
	if (!rendToText) {
		for (auto e : elements) {
			e->createObject(ob);
		}
	}
}

void GUIGroup::createObjectNoRestriction(Objects & ob)
{
	for (auto e : elements) {
		e->createObject(ob);
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
}

void GUIGroup::testClicked(double & xPos, double & yPos, int mouseState, bool & isClickedAbove)
{
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

void GUIGroup::handleHover()
{
}

void GUIGroup::handleClicked(int & mouseState)
{
}
