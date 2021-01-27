#include "GUI.h"
#include "Global.h"
#include <algorithm>
#include "ElementCreator.h"
#include "ObjectLibrary.h"
#include <cmath>
#pragma warning(disable: 4244)

GUIElement::GUIElement(std::string name): name(name)
{
}

GUIElement::GUIElement(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string text = "") : name(name), v(v), xStart(xStart), yStart(yStart), width(width), height(height), text(text)
{
}

bool GUIElement::testHover(double& xPosRef, double& yPosRef)
{
	bool test =
		(
			xPosRef >= xStart &&
			xPosRef <= xStart + (width * scale) &&
			yPosRef >= yStart &&
			yPosRef <= yStart + (height * scale)
			);
	if (test && !skipHover) {

		if (!withinArea && mouseEnterExitLambda != NULL) {
			mouseEnterExitLambda(withinArea = true);
		}

		if (!rendToText) {
			isHover = true;
			handleHover(xPosRef, yPosRef);
		}
		else {
			double x = xPosRef - xStart + xRendToTextOffset, y = yPosRef - yStart + yRendToTextOffset;
			isHover = true;
			handleHover(x, y);
		}
	}
	else if (!test) {

		if (withinArea && mouseEnterExitLambda != NULL) {
			mouseEnterExitLambda(withinArea = false);
		}

		if(isHover)
			if (!rendToText) {
				isHover = false;
				handleHover(xPosRef, yPosRef);
			}
			else {
				double x = xPosRef - xStart + xRendToTextOffset, y = yPosRef - yStart + yRendToTextOffset;
				isHover = false;
				handleHover(x, y);
			}
	}
	return test;
	/*
	if (!rendToText) {
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
	*/
}

bool GUIElement::testClicked(double & xPos, double & yPos, int mouseButton, int mouseState, bool& isClickedAbove, GUIElement*& focusElement)
{
	bool test =
		(
			xPos >= xStart &&
			xPos <= xStart + (width * scale) &&
			yPos >= yStart &&
			yPos <= yStart + (height * scale)
			);
	if (show && test) {
		isClickedAbove = true;
		if (mouseState == MOUSE_PRESS) {
			isClicked = !isClicked;
		}
		else {
			if (get_FocusAble()) {
				if (focusElement && focusElement != this) {
					if (getFocus())
						setFocus(false);
					focusElement->setFocus(false);
					focusElement = nullptr;
				}
			}
		}
		handleClicked(mouseButton, mouseState);
	}
	return test;
}

void GUIElement::createObject()
{
	if (!rendToText) {
		bool update = false;
		if (v) {
			if (!dObj) {
				dObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
				update = true;
			}
			else {
				dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
				dObj->setID(v->getID());
				dObj->set_VAO(v->getVAO());
				dObj->set_Text_Pos(v->getTextPos());

			}
			if (rtt != nullptr) dObj->setRendToText(rtt);
			dObj->setOffsetX(xOffset / textureWidth);
			dObj->setOffsetY(yOffset / textureHeight);
			dObj->set_Scale_X(scaleX);
			dObj->set_Scale_Y(scaleY);
			
		}
		if (focusVertices) {
			if (!focusDObj) {
				focusDObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), focusVertices->getID(), focusVertices->getVAO(), focusVertices->getTextPos());
				focusDObj->setDraw(false);
				update = true;
			}
			else
			{
				focusDObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				focusDObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
				focusDObj->setID(focusVertices->getID());
				focusDObj->set_VAO(focusVertices->getVAO());
				focusDObj->set_Text_Pos(focusVertices->getTextPos());
			}
			focusDObj->setOffsetX(xOffset / textureWidth);
			focusDObj->setOffsetY(yOffset / textureHeight);
			if (scaleX != 1.0)
				focusDObj->set_Scale_X(scaleX);
			if (scaleY != 1.0)
				focusDObj->set_Scale_Y(scaleY);
		}
		if (text.size() > 0) {
			if (!dObjText)
				dObjText = new TextDrawObject(text);
			// Text
			generate_GUI_Text(dObjText, xStart, yStart, 0 + textOffset, width - textOffset, height, text, textType, ellipsis, isCenteredText);
		}
		if(update)
			if (DrawObjectGroup* group = dynamic_cast<DrawObjectGroup*>(dObjs)) {
				group->clear();
				if(dObj)
					group->add(dObj);
				if (focusDObj)
					group->add(focusDObj);
				if (dObjText)
					group->add(dObjText);
			}
	}
}

void GUIElement::createObjectNoRestriction() {

	if (v) {
		if(!dObj)
			if (!rendToText) 
				dObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
			else {
				dObj = new DrawObject(((xStart + xDrawOffset) / (double(rtt->getWidth()) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(rtt->getHeight()) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
			}
		else {
			if (!rendToText) {
				dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
			}
			else {
				dObj->setXPosition(((xStart + xDrawOffset) / (double(rtt->getWidth()) / 2)));
				dObj->setYPosition(((yStart + yDrawOffset) / (double(rtt->getHeight()) / 2)));
			}
			dObj->setID(v->getID());
			dObj->set_VAO(v->getVAO());
			dObj->set_Text_Pos(v->getTextPos());
		}
		if (rtt != nullptr && !rendToText) 
			dObj->setRendToText(rtt);
		dObj->setOffsetX(xOffset / textureWidth);
		dObj->setOffsetY(yOffset / textureHeight);
		dObj->set_Scale_X(scaleX);
		dObj->set_Scale_Y(scaleY);
	}
	if (text.size() > 0) {

		if (!dObjText)
			dObjText = new TextDrawObject(text);
		generate_GUI_Text(dObjText, xStart, yStart, 0 + textOffset, width - textOffset, height, text, textType, ellipsis, isCenteredText, rtt);
	}
	
}

void GUIElement::get_Draw_Object(DrawObjects*& objs, bool skipIfRTT)
{
	createObject();

	if (skipIfRTT && !rendToText)
		objs->addObject(dObjs);
	else if (!skipIfRTT)
		objs->addObject(dObjs);

}

DrawObject*& GUIElement::get_Draw_Object()
{
	return dObj;
}

void GUIElement::testScroll(double & scroll)
{
	if (scrollLambda != NULL)
		scrollLambda(scroll);
}


std::string & GUIElement::getName()
{
	return name;
}

void GUIElement::setUpdate(bool value)
{
	update = value;
	if (!value) {
		if (rendToText && rtt) {
			createObjectNoRestriction();
		}
		else {
			createObject();
		}
	}
}

bool GUIElement::getUpdate()
{
	return update;
}

void GUIElement::handle_Update()
{
	this->createObject();
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
	int mouseButton = MOUSE_LEFT_CLICK, mouseState = MOUSE_RELEASE; handleClicked(mouseButton, mouseState);
}

void GUIElement::click(int& mouseButton, int& mouseState)
{
	handleClicked(mouseButton, mouseState);
}

void GUIElement::setText(std::string s)
{
	text = s;
}

void GUIElement::handleHover(double& xPosRef, double& yPosRef)
{
	if (hoverLambda != NULL) {
		hoverLambda(xPosRef, yPosRef);
	}
}

void GUIElement::handleClicked(int& mouseButton, int& mouseState)
{
	isClicked = !isClicked;
}

void GUIElement::handleAnimation(bool& update)
{
	animation(update);
}

void GUIElement::handleKeyStroke(int& key, int& pressed, int& mods)
{
	if (focus && keyStrokeLambda != NULL)
		keyStrokeLambda(key, pressed, mods);
}

bool GUIElement::getFocus()
{
	return focus;
}

void GUIElement::setFocus(bool b)
{
	focus = b;
	if (focusDObj)
		if (focus)
			focusDObj->setDraw(true);
		else
			focusDObj->setDraw(false);
}

void GUIElement::change_Position(int xStart, int yStart)
{
	int xStartDifference = xStart - this->xStart, yStartDifference = yStart - this->yStart;
	this->xStart = xStart; this->yStart = yStart;
}

TextDrawObject*& GUIElement::get_Draw_Object_Text()
{
	return dObjText;
}

void GUIElement::checkAnimation()
{
	if (doAnimation && animation) {
		handleAnimation(update);
	}
}

GUIPanel::GUIPanel(std::string name): name(name)
{
	ob = new DrawObjects(name);
}

GUIPanel::GUIPanel(std::string name, DrawObjects *ob, int x, int y, int width, int height): name(name), ob(ob), xStart(x), yStart(y), width(width), height(height)
{
	ob = new DrawObjects(name);
}

GUIPanel::~GUIPanel()
{
	clear();
	if (ob)
		delete ob;
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
	e->get_Draw_Object(ob);
	//e->get_Draw_Object_Text(ob);
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


void GUIPanel::setDimensions(int x, int y, int width, int height)
{
	this->xStart = x;
	this->yStart = y;
	this->width = width;
	this->height = height;
}

void GUIPanel::checkHover(double & xPos, double & yPos)
{
	for (auto& e : elements) {
		e->testHover(xPos, yPos);
	}
}

void GUIPanel::checkClicked(double & xPos, double & yPos, int mouseButton, int mouseState)
{
	if (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		focus = true;
		for (auto e = elements.end()-1; e != elements.begin(); e-- ) {
			if (!isClicked)
				(*e)->testClicked(xPos, yPos, mouseButton, mouseState, isClicked, focusElement);
			else
				break;
		}
		if (!isClicked && focusElement && mouseState == MOUSE_PRESS) {
			focusElement->setFocus(false);
			focusElement = nullptr;
		}
	}
	else {
		focus = false;
		if (focusElement) {
			focusElement->setFocus(false);
			focusElement = nullptr;
		}
	}
	isClicked = false;
}

void GUIPanel::checkScroll(double& xPos, double& yPos, double& scroll)
{
	if (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height) {
		for (auto& e : elements)
			if(xPos >= e->getXStart() && xPos <= e->getXStart() + e->getWidth() && yPos >= e->getYStart() && yPos <= e->getYStart() + e->getHeight())
				e->testScroll(scroll);
	}
}

void GUIPanel::checkKeyStroke(int& key, int& action, int& mods)
{
	if (focus) {
		if (key != 258)
			for (auto e : elements) {
				if (e->getFocus())
					e->handleKeyStroke(key, action, mods);
			}
		else if(action == GLFW_PRESS){
			GUIElement* first = nullptr;
			GUIElement* current = nullptr;
			for (auto& e : elements) {
				if (e->getElements().size() > 0) {
					for (auto& e2 : e->getElements()) {
						if (e2->get_FocusAble())
							check_Focus(e2);
					}
				}
				else if (e->get_FocusAble())
					check_Focus(e);
			}
		}
	}
}

void GUIPanel::check_Focus(GUIElement* e) 
{

	if (!e->getFocus()) {
		if (!focusElement) {
			focusElement = e;
			e->setFocus(true);
		}
	}
	else if(e == focusElement) {
		focusElement = nullptr;
		e->setFocus(false);
	}
}

void GUIPanel::checkUpdates()
{
	for (auto& e : elements) {
		if (e->getUpdate()) {
			e->setUpdate(false);
		}
		e->checkAnimation();
	}
	/*
	if (containsLabel())
		update = true;
	if (update) {
		ob->clearObjects();
		createObjects();
	}
	*/
}

void GUIPanel::toggleShow()
{
	show = !show;
	set_Focus(show);
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

void GUIPanel::create_Draw_Objects()
{
	ob->clearObjects();
	for (auto e : elements) {
		e->get_Draw_Object(ob);
		//e->get_Draw_Object_Text(ob);
	}
}

void GUIPanel::set_Focus(bool b)
{
	focus = b;
	if (!b && focusElement) {
		focusElement->setFocus(false);
		focusElement = nullptr;
	}
}

Button::Button(std::string name, Vertices* button, Vertices* buttonFocus, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::string text = "", std::function<void(int& mouseButton, int& mouseState)> lambda = [](int& mouseButton, int& mouseState) {}) : button(button), hover(hover), clicked(clicked), GUIElement(name, button, xStart, yStart, width, height, text)
{
	setV(button);

	if (buttonFocus) {
		focusAble = true;
		set_Focus_Vertices(buttonFocus);
	}

	clickLambda = lambda;

	if (v && (v->getWidth() != width || v->getHeight() != height)) {
		scaleX = width / v->getWidth();
		scaleY = height / v->getHeight();
	}

	isCenteredText = true;
	ellipsis = true;
}

void Button::handleKeyStroke(int& key, int& action, int& mods)
{
	if (focus && (key == 335 || key == 257)) {
		if (action == GLFW_PRESS) 
			handleClicked(MOUSE_LEFT_CLICK, MOUSE_PRESS);
		else if(action == GLFW_RELEASE)
			handleClicked(MOUSE_LEFT_CLICK, MOUSE_RELEASE);
	}
}

/*
bool Button::testClicked(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClickedAbove, GUIElement* focusElement)
{
	bool test =
		(
			xPos >= xStart &&
			xPos <= xStart + (width * scale) &&
			yPos >= yStart &&
			yPos <= yStart + (height * scale)
			);
	if (show && test) {
		if (mouseButton == MOUSE_LEFT_CLICK) {
			if (mouseState == MOUSE_PRESS) {
				isClickedAbove = true;
				isClicked = true;
				setFocus(true);
				if (focusElement) {
					focusElement->setFocus(false);
					focusElement = nullptr;
				}
				focusElement = this;
			}
			else if(MOUSE_RELEASE){
				isClicked = false;
				setFocus(false);
				if (focusElement && focusElement == this)
					focusElement = nullptr;
			}
		}
		handleClicked(mouseButton, mouseState);
	}
	return isClicked;
}
*/
void Button::handleHover(double& xPosRef, double& yPosRef)
{
	if ((!getV() || getV() == button) && isHover) {
		setV(hover);
		setUpdate(true);
		if (hoverLambda != NULL) {
			hoverLambda(xPosRef, yPosRef);
		}
	}
	else if ((getV() == clicked || getV() == hover) && !isHover) {
		setV(button);
		setUpdate(true);
		if (hoverLambda != NULL) {
			hoverLambda(xPosRef, yPosRef);
		}
	}
}

void Button::handleClicked(int& mouseButton, int& mouseState)
{
	if (mouseButton == MOUSE_LEFT_CLICK && mouseState == MOUSE_PRESS) {
		setV(clicked);
		setUpdate(true);
	}
	else if (mouseButton == MOUSE_LEFT_CLICK && mouseState == MOUSE_RELEASE && getV() == clicked) {
		if (isHover)
			setV(hover);
		else {
			setV(button);
		}
		setUpdate(true);
		if (clickLambda != NULL)
			clickLambda(mouseButton, mouseState);
	}
}
ToggleButton::ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, Vertices* buttonFocus, int xStart, int yStart, int width, int height, std::function<void(int& mouseButton, int& mouseState)> lambda) : button(button), hover(hover), clicked(clicked), GUIElement(name, button, xStart, yStart, width, height)
{
	if (buttonFocus) {
		set_FocusAble(true);
		set_Focus_Vertices(buttonFocus);
	}

	clickLambda = lambda;
	if (v->getWidth() != width || v->getHeight() != height) {
		scaleX = width / v->getWidth();
		scaleY = height / v->getHeight();
	}
}
ToggleButton::ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, Vertices* buttonFocus, int xStart, int yStart, int width, int height, std::function<void(int& mouseButton, int& mouseState)> lambda, std::function<void(double& x, double& y)> hoverLambda) : button(button), hover(hover), clicked(clicked), GUIElement(name, button, xStart, yStart, width, height)
{
	if (buttonFocus) {
		set_FocusAble(true);
		set_Focus_Vertices(buttonFocus);
	}

	clickLambda = lambda;
	setHoverLambda(hoverLambda);
}

ToggleButton::~ToggleButton()
{
	if (base)
		delete base;
}


void ToggleButton::createObject()
{
	
	if (show && !rendToText && (alwaysShowBase || getV())) {
		if (alwaysShowBase && button) {
			if(!base)
				base = new DrawObject(((xStart + 64 * offset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 64 * offset) / (double(screenHeightPixels) / 2)), button->getID(), button->getVAO(), button->getTextPos());
			else
			{
				base->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				base->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
				base->setID(v->getID());
				base->set_VAO(v->getVAO());
				base->set_Text_Pos(v->getTextPos());
			}
			base->setOffsetX(xOffset / textureWidth);
			base->setOffsetY(yOffset / textureHeight);
		}
		if (getV()) {
			if(!dObj)
				dObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
			else
			{
				dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
				dObj->setID(v->getID());
				dObj->set_VAO(v->getVAO());
				dObj->set_Text_Pos(v->getTextPos());
			}
			dObj->setOffsetX(xOffset / textureWidth);
			dObj->setOffsetY(yOffset / textureHeight);
			if (scaleX != 1.0)
				dObj->set_Scale_X(scaleX);
			if (scaleY != 1.0)
				dObj->set_Scale_Y(scaleY);
		}
		if (focusVertices) {
			if (!focusDObj) {
				focusDObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), focusVertices->getID(), focusVertices->getVAO(), focusVertices->getTextPos());
				focusDObj->setDraw(false);
			}
			else
			{
				focusDObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				focusDObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
				focusDObj->setID(focusVertices->getID());
				focusDObj->set_VAO(focusVertices->getVAO());
				focusDObj->set_Text_Pos(focusVertices->getTextPos());
			}
			focusDObj->setOffsetX(xOffset / textureWidth);
			focusDObj->setOffsetY(yOffset / textureHeight);
			if (scaleX != 1.0)
				focusDObj->set_Scale_X(scaleX);
			if (scaleY != 1.0)
				focusDObj->set_Scale_Y(scaleY);
		}

		// Text
		auto textSize = Get_Text_Size(text, "Black");
		if (!dObjText)
			dObjText = new TextDrawObject(text);
		if (!rendToText || (rendToText && !rtt))
			generate_GUI_Text(dObjText, xStart, yStart, 0, width - textOffset, height, text, textType, ellipsis, isCenteredText);
		else
			generate_GUI_Text(dObjText, xStart, yStart, 0, width - textOffset, height, text, textType, ellipsis, isCenteredText, rtt);
	}
	
}

void ToggleButton::createObjectNoRestriction() {
	
	if (show && (alwaysShowBase || getV())) {
		bool change = false; double x = .0, y = .0;
		if (alwaysShowBase && button) {
			if (!rendToText) {
				if (!base)
					base = new DrawObject(((xStart + 64 * offset + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + 64 * offset + yDrawOffset) / (double(screenHeightPixels) / 2)), button->getID(), button->getVAO(), button->getTextPos());
				else {
					base->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
					base->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
					base->setID(button->getID());
					base->set_VAO(button->getVAO());
					base->set_Text_Pos(button->getTextPos());
				}
			}
			else {
				if(!base)
					base = new DrawObject( ((xStart + 64 * offset + xDrawOffset) / (double(rtt->getWidth()) / 2)), 0.0 + ((yStart + 64 * offset + yDrawOffset) / (double(rtt->getHeight()) / 2)), button->getID(), button->getVAO(), button->getTextPos());
				else {
					base->setXPosition(((xStart + xDrawOffset) / (double(rtt->getWidth()) / 2)));
					base->setYPosition(((yStart + yDrawOffset) / (double(rtt->getHeight()) / 2)));
					base->setID(button->getID());
					base->set_VAO(button->getVAO());
					base->set_Text_Pos(button->getTextPos());
				}
			}
			if (rtt != nullptr && !rendToText) 
				base->setRendToText(rtt);
			base->setOffsetX(xOffset / textureWidth);
			base->setOffsetY(yOffset / textureHeight);
		}
		if (getV()) {
			if (!rendToText) {
				if (!dObj) {
					dObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
				}
				else {
					dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
					dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
					dObj->setID(v->getID());
					dObj->set_VAO(v->getVAO());
					dObj->set_Text_Pos(v->getTextPos());
				}
			}
			else {
				if (!dObj) {
					dObj = new DrawObject(((xStart + 64 * offset + xDrawOffset) / (double(rtt->getWidth()) / 2)), 0.0 + ((yStart + 64 * offset + yDrawOffset) / (double(rtt->getHeight()) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
				}
				else {
					x = (xStart + xDrawOffset) / (double(rtt->getWidth()) / 2);
					if (x != dObj->getXPosition())
						change = true;
					dObj->setXPosition(x);
					y = (yStart + yDrawOffset) / (double(rtt->getHeight()) / 2);
					if (y != dObj->getYPosition())
						change = true;
					dObj->setYPosition(y);
					dObj->setID(v->getID());
					dObj->set_VAO(v->getVAO());
					dObj->set_Text_Pos(v->getTextPos());
				}
			}
			if (rtt != nullptr && !rendToText)
				dObj->setRendToText(rtt);
			dObj->setOffsetX(xOffset / textureWidth);
			dObj->setOffsetY(yOffset / textureHeight);
		}
		if (text.size() > 0) {
			// Text
			if (!dObjText) {
				dObjText = new TextDrawObject(text);
			}
			else if(change){
				dObjText->clear();
				dObjText = new TextDrawObject(text);
			}

			auto textSize = Get_Text_Size(text, "Black");
			if (!rendToText || (rendToText && !rtt))
				generate_GUI_Text(dObjText, xStart, yStart, textOffset, width - textOffset, height, text, textType, ellipsis, isCenteredText);
			else
				generate_GUI_Text(dObjText, xStart, yStart, textOffset, width - textOffset, height, text, textType, ellipsis, isCenteredText, rtt);
		}
	}
	
}

void ToggleButton::get_Draw_Object(DrawObjects*& objs, bool skipIfRTT) {
	if (!dObj)
		if (!rendToText)
			createObject();
		else
			createObjectNoRestriction();
	if (base)
		if (skipIfRTT && !rendToText)
			objs->addObject(base);
		else if (!skipIfRTT)
			objs->addObject(base);
	if (dObj)
		if (skipIfRTT && !rendToText)
			objs->addObject(dObj);
		else if (!skipIfRTT)
			objs->addObject(dObj);
	if (focusDObj)
		if (skipIfRTT && !rendToText)
			objs->addObject(focusDObj);
		else if (!skipIfRTT)
			objs->addObject(focusDObj);
};

void ToggleButton::resetToggle()
{
	v = button;
	if (!cleanReset && toggle) {
		int mouseButton = MOUSE_LEFT_CLICK, mouseState = MOUSE_RELEASE;
		if(clickLambda != NULL) clickLambda(mouseButton, mouseState);
	}
	toggle = false;
	isClicked = false;
	setUpdate(true);
	clickedOrder = 0;
}

/**
** Trigger the ToggleButton as if clicked.
**
** @param: int mouseState; the state of the mouse event
**
**/
void ToggleButton::doToggle(int mouseButton, int mouseState)
{
	if (button != nullptr) {
		isClicked = !isClicked;
		toggle = !toggle;
		if (toggle) setV(clicked);
		else if (getHover() && doHover) setV(hover);
		else setV(button);
		setUpdate(true);
		clickLambda(mouseButton, mouseState);
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

void ToggleButton::handleKeyStroke(int& key, int& action, int& mods)
{
	if (focus && (key == 335 || key == 257)) {
		if (action == GLFW_PRESS) {
			isClicked = !isClicked;
			handleClicked(MOUSE_LEFT_CLICK, MOUSE_PRESS);
		}
		else if (action == GLFW_RELEASE)
			handleClicked(MOUSE_LEFT_CLICK, MOUSE_RELEASE);
	}
}

void ToggleButton::handleHover(double& xPosRef, double& yPosRef)
{
	if (!toggle && (!getV() || getV() == button) && isHover) {
		if (doHover) {
			setV(hover);
			setUpdate(true);
			if (hoverLambda != NULL) {
				hoverLambda(xPosRef, yPosRef);
			}
		}
	}
	else if (!toggle && (getV() == clicked || getV() == hover) && !isHover) {
		if (doHover) {
			if (getV() != button) {
				setV(button);
				setUpdate(true);
			}
			if (hoverLambda != NULL) {
				hoverLambda(xPosRef, yPosRef);
			}
		}
	}
}

void ToggleButton::handleClicked(int& mouseButton, int& mouseState)
{
	if (mouseButton == MOUSE_LEFT_CLICK && mouseState == MOUSE_PRESS) {
		setV(clicked);
		setUpdate(true);
	}
	else if (mouseButton == MOUSE_LEFT_CLICK && mouseState == MOUSE_RELEASE && (getV() == clicked || button == clicked)) {
		if (isClicked) {
			toggle = true;
			setV(clicked);
			setUpdate(true);
		}
		else {
			toggle = false;
			setV(hover);
			setUpdate(true);
		}
		if(clickLambda != NULL)
			clickLambda(mouseButton, mouseState);
	}

}

ToggleButtonGroup::~ToggleButtonGroup()
{
	for (auto& e: toggleButtons) {
		delete e;
	}
}

void ToggleButtonGroup::createObject()
{
	for (auto& e : toggleButtons) {
		if (rendToText && rtt) {
			e->setRenderToText(rtt, true);
			e->createObjectNoRestriction();
		}
		else {
			e->createObject();
		}
	}
}

void ToggleButtonGroup::createObjectNoRestriction() {
	for (auto& e : toggleButtons) {
		if (rendToText && rtt)
			e->setRenderToText(rtt, true);
		e->createObjectNoRestriction();
	}
}

void ToggleButtonGroup::get_Draw_Object(DrawObjects*& objs, bool skipIfRTT)
{
	for (auto e : toggleButtons) {
		if (e->getButton() || getHover())
			e->get_Draw_Object(objs, skipIfRTT);
	}
}

void ToggleButtonGroup::get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT)
{
	for (auto e : toggleButtons) {
		if (e->getText().size() > 0)
			e->get_Draw_Object_Text(objs, skipIfRTT);
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
	if (resetAfterToggle) {
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
	if (resetAfterToggle) {
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
		if (rendToText && rtt) {
			e->setRenderToText(rtt, rendToText);
			e->createObjectNoRestriction();
		}
		else
			e->createObject();
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

bool ToggleButtonGroup::testHover(double & xPos, double & yPos)
{
	if (!skipHover) {
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
			double x = xPos - xStart + xRendToTextOffset, y = yPos - yStart + yRendToTextOffset;
			if (test) {
				for (auto& e : toggleButtons) {
					e->testHover(x, y);
				}
			}
			else if (checkOutsideOfArea) {
				for (auto& e : toggleButtons) {
					if (e->getHover())
						e->testHover(x, y);
				}
			}
			if ((test || checkOutsideOfArea) && hoverLambda != NULL)
				hoverLambda(x, y);
		}
		return test;
	}
	return false;
}

/**
** Test if the ToggleButtonGroup was clicked and whether a ToggleButton within the group was clicked.
**
** params: b; the x postion of the mouse, y; the y postion of the mouse, mouseState; the state of the mouse click; isClickedAbove; whether another GUI element has been clicked before this one.
**
** return: If an element within the group was clicked or not.
**/
bool ToggleButtonGroup::testClicked(double & xPos, double & yPos, int mouseButton, int mouseState, bool& isClickedAbove, GUIElement*& focusElement)
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
		if (mouseButton == MOUSE_LEFT_CLICK) {
			for (auto& e : toggleButtons) {
				/*if (doNotResetAfterToggle) {
					testClickedAt(x, y, mouseState, isClickedAbove);
					break;
				}else*/
				if ((wasClicked = e->testClicked(x, y, mouseButton, mouseState, isClickedAbove, focusElement))) {
					if (before && before->getName().compare(e->getName()) == 0 && !e->getToggle())
						currentlyPressed = nullptr;
					else if ((!before || before->getName().compare(e->getName()) != 0))
						currentlyPressed = e;
					break;
				}
			}

			if (resetAfterToggle) {
				for (auto& e : toggleButtons) {
					if ((!currentlyPressed || e != currentlyPressed) && (e->getClicked() || e->getToggle())) {
						e->resetToggle();
					}
				}
			}
		}
		if (clickLambda != NULL) {
			clickLambda(mouseButton, mouseState);
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
	if (!b) {
		for (auto& e : toggleButtons) {
			if (e->getUpdate()) {
				e->setUpdate(false);
			}
		}
	}
}

void ToggleButtonGroup::testClickedAt(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClickedAbove, GUIElement*& focusElement)
{
	for (auto& e : toggleButtons) {
		if (xPos >= e->getXStart() && xPos <= e->getXStart() + e->getWidth() && yPos >= e->getYStart() && yPos <= e->getYStart() + e->getHeight()) {
			if (e->testClicked(xPos, yPos, mouseButton, mouseState, isClickedAbove, focusElement))
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

std::vector<GUIElement*> ToggleButtonGroup::getElements()
{
	std::vector<GUIElement*> list;
	for (auto& tb : toggleButtons) {
		list.push_back(dynamic_cast<GUIElement*>(tb));
	}
	return list;
}

void ToggleButtonGroup::handleKeyStroke(int& key, int& action, int& mods)
{
	if ((key == 335 || key == 257)) {

		for (auto& tb : toggleButtons) {
			if (tb->getFocus()) {
				if (action == GLFW_PRESS) {
					tb->set_Clicked(!tb->getClicked());
					tb->click(MOUSE_LEFT_CLICK, MOUSE_PRESS);
				}
				else if (action == GLFW_RELEASE)
					tb->click(MOUSE_LEFT_CLICK, MOUSE_RELEASE);
			}
			else
				tb->resetToggle();
		}
	}
}

bool ToggleButtonGroup::getFocus()
{
	for (auto& tb : toggleButtons) {
		if (tb->getFocus())
			return true;
	}
	return false;
}

void ToggleButtonGroup::handleHover()
{
}

void ToggleButtonGroup::handleClicked(int& mouseButton, int & mouseState)
{
}

DropDown::DropDown(std::string name, 
	Vertices* topSection, Vertices* topSectionHover, Vertices* topSectionClicked, Vertices* topSectionFocus,
	Vertices* middleSection, Vertices* middleSectionHover, Vertices* middleSectionClicked,
	Vertices* bottomSection, Vertices* bottomSectionHover, Vertices* bottomSectionClicked,
	int xStart, int yStart, int width, int height) :
	topSection(topSection), topSectionHover(topSectionHover), topSectionClicked(topSectionClicked),
	middleSection(middleSection), middleSectionHover(middleSectionHover), middleSectionClicked(middleSectionClicked),
	bottomSection(bottomSection), bottomSectionHover(bottomSectionHover), bottomSectionClicked(bottomSectionClicked),
	GUIElement(name, xStart, yStart, width, height)
{
	if (topSectionFocus)
		set_FocusAble(true);

	tb = new ToggleButton("tb", topSection, topSectionHover, topSectionClicked, topSectionFocus, xStart, yStart, width, height, [this](int& mousebutton, int& mouseState) {
		this->toggleShowDropDown();
		this->setUpdate(true);
		});
	//tb->setTextOffset(10);
	tb->setCenteredText(true);
	tb->toggleCleanReset();
	isCenteredText = true;

	childWidth = width;
	/*
	if (v->getWidth() != width || v->getHeight() != height) {
		scaleX = width / v->getWidth();
		scaleY = height / v->getHeight();
	}
	*/
}

DropDown::~DropDown()
{
	/*
	delete topSection; delete topSectionHover; delete topSectionClicked;
	delete middleSection; delete middleSectionHover; delete middleSectionClicked;
	delete bottomSection; delete bottomSectionHover; delete bottomSectionClicked;
	*/
	delete tb;
	for (auto& i : items) {
		delete i;
	}
}

void DropDown::add(std::string text, std::function<void(double& xPos, double& yPos)> const& hoverLambda, std::function<void(int& mouseButton, int& mouseState)> const& clickLambda, bool staticSection)
{
	int xPos = xStart, yPos = yStart + 1, width = this->width;
	DropDownElement* dde = nullptr, *previous = nullptr;

	if (items.size() == 0) {
		if (bottomSection)
			items.push_back(dde = new DropDownElement(text, bottomSection, bottomSectionHover, bottomSectionClicked, xPos + xChildOffset, yPos + height + padding, width, height, hoverLambda, clickLambda));
		else
			items.push_back(dde = new DropDownElement(text, middleSection, middleSectionHover, middleSectionClicked, xPos + xChildOffset, yPos + height + padding, width, height, hoverLambda, clickLambda));
	}
	else {
		if (bottomSection) {
			if (fromTopDown) {
				previous = (*(items.end() - 1));
				previous->set_Section_Vertices(middleSection); previous->set_Hover_Vertices(middleSectionHover); previous->set_Clicked_Vertices(middleSectionClicked); previous->setV(middleSection);
				items.push_back(dde = new DropDownElement(text, bottomSection, bottomSectionHover, bottomSectionClicked, xPos + xChildOffset, yPos + (height * (items.size() + 1)) + (padding * items.size()), width, height, hoverLambda, clickLambda));
			}
			else {
				items.insert(items.begin() + 1, dde = new DropDownElement(text, middleSection, middleSectionHover, middleSectionClicked, xPos + xChildOffset, yPos + height + padding, width, height, hoverLambda, clickLambda));
				int pos = 0;
				for (auto& i : items) {
					i->setHeight(yPos + (height * (pos + 1)) + (padding * pos)); 
					pos++;
				}
			}
		}
		else {
			if (fromTopDown) {
				items.push_back(dde = new DropDownElement(text, middleSection, middleSectionHover, middleSectionClicked, xPos + xChildOffset, yPos + (height * (items.size() + 1)) + (padding + padding * items.size()), width, height, hoverLambda, clickLambda));
			}
			else {
				items.insert(items.begin() + 1, dde = new DropDownElement(text, middleSection, middleSectionHover, middleSectionClicked, xPos + xChildOffset, yPos + height + padding, width, height, hoverLambda, clickLambda));
				int pos = 0;
				for (auto& i : items) {
					i->setHeight(yPos + (height * (pos + 1)) + (padding * pos));
					pos++;
				}
			}
		}
	}
	if (get_Size_Based_On_Text())
		dde->set_Size_Based_On_Text();
	if (get_Ellipsis())
		dde->set_Ellipsis(true);
	if (isCenteredText)
		dde->setCenteredText(true);
	if (childTextOffsetX != 0)
		dde->setTextOffset(childTextOffsetX);
	if (dde->getWidth() > childWidth)
		childWidth = dde->getWidth();
	if (staticSection)
		dde->set_Static_Section(true);

	if (dde && rtt) {
		dde->setRenderToText(rtt, true);
		dde->createObjectNoRestriction();
	}
	else
		dde->createObject();

	if (dde->get_Draw_Object())
		dde->get_Draw_Object()->setDraw(false);
	if(dde->get_Draw_Object_Static_Base())
		dde->get_Draw_Object_Static_Base()->setDraw(false);
	if (dde->get_Draw_Object_Text())
		dde->get_Draw_Object_Text()->setDraw(false);
}

void DropDown::createObject()
{
	tb->createObject();
	if (showOthers) {
		for (auto& i : items) {
			i->createObject();
		}
	}
}

void DropDown::createObjectNoRestriction()
{
	tb->createObjectNoRestriction();
	for (auto& i : items) {
		i->createObjectNoRestriction();
		handleDraw(i);
	}
}

void DropDown::get_Draw_Object(DrawObjects*& objs, bool skipIfRTT)
{
	tb->get_Draw_Object(objs, skipIfRTT);
	for (auto& i : items) {
		i->get_Draw_Object(objs, skipIfRTT);
	}
}

void DropDown::get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT)
{
	tb->get_Draw_Object_Text(objs, skipIfRTT);
	for (auto& i : items) {
		i->get_Draw_Object_Text(objs, skipIfRTT);
	}
}


bool DropDown::testHover(double & xPosRef, double & yPosRef)
{
	bool test =
		(
			xPosRef >= xStart &&
			xPosRef <= xStart + (width * scale) &&
			yPosRef >= yStart &&
			yPosRef <= yStart + (height * scale)
			);
	bool test2 =
		(
			xPosRef >= xStart + xChildOffset &&
			xPosRef <= xStart + xChildOffset + (width * scale) &&
			yPosRef >= yStart + height + 1 &&
			yPosRef <= highestY
			);

	if(highestY == 0 and items.size() > 0)
		get_Highest_Child_Y_Position();

	if (test || (showOthers && test2) && !skipHover) {

		if (!withinArea && mouseEnterExitLambda != NULL) {
			mouseEnterExitLambda(withinArea = true);
		}

		isHover = true;
		handleHover(xPosRef, yPosRef);
	}
	else if (!test || (showOthers && !test2)) {

		if (withinArea && mouseEnterExitLambda != NULL) {
			mouseEnterExitLambda(withinArea = false);
		}
		if (isHover) {
			isHover = false;
			handleHover(xPosRef, yPosRef);
		}
	}
	return false;

}


bool DropDown::testClicked(double & xPos, double & yPos, int mouseButton, int mouseState, bool& isClickedAbove, GUIElement*& focusElement)
{
	if (showOthers) {
		for (auto i : items) {
			if (i->testClicked(xPos, yPos, mouseButton, mouseState, isClickedAbove, focusElement) && mouseState == MOUSE_RELEASE) {
				if (onlyOneClickable && lastClicked && lastClicked != i) {
					lastClicked->setV(lastClicked->get_Section_Vertices());
					lastClicked->set_Clicked(false);
					lastClicked->setHover(false);
				}
				if(!lastClicked || lastClicked != i)
					lastClicked = i;
				else
					lastClicked = nullptr;
				if(closeAfterSelect)
					showOthers = false;
				break;
			}
		}
	}
	tb->testClicked(xPos, yPos, mouseButton, mouseState, isClickedAbove, focusElement);
	return false;
}

bool DropDown::getUpdate()
{
	if (update || tb->getUpdate())
		return true;
	if (lastClicked && lastClicked->getUpdate())
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
		if (tb->getUpdate()) {
			tb->setUpdate(false);
		}
		for (auto& i : items) {
			if(i->getUpdate())
				i->setUpdate(false);
		}
	}
}

void DropDown::toggleShowDropDown()
{
	showOthers = !showOthers; 
	for (auto& i : items) {
		handleDraw(i);
	}
}

void DropDown::change_Position(int xStart, int yStart)
{
	int xStartDifference = xStart - this->xStart, yStartDifference = yStart - this->yStart;
	this->xStart = xStart; this->yStart = yStart;
	tb->setStartX(xStart); tb->setStartY(yStart);
	for (auto i : items) {
		i->setStartX(i->getXStart() + xStartDifference);
		i->setStartY(i->getYStart() + yStartDifference);
	}
	if (yStartDifference)
		get_Highest_Child_Y_Position();
}

int DropDown::get_Highest_Child_Y_Position()
{
	int y = yStart, iY = 0;
	for (auto i : items) {
		iY = i->getYStart() + i->getHeight();
		if (iY > y)
			y = iY;
	}
	highestY = y;
	return y;
}

void DropDown::reset_Drop_Down()
{
	if (lastClicked) {
		lastClicked->setHover(false);
		lastClicked->set_Clicked(false);
		lastClicked->setV(lastClicked->get_Section_Vertices());
		lastClicked = nullptr;
	}
	if (lastHover) {
		lastHover->setHover(false);
		lastHover->set_Clicked(false);
		lastHover->setV(lastHover->get_Section_Vertices());
		lastHover = nullptr;
	}
}

std::vector<GUIElement*> DropDown::getElements()
{
	std::vector<GUIElement*> list;
	list.push_back(tb);
	list.insert(list.end(), items.begin(), items.end());
	return list;
}


void DropDown::handleHover(double& xPosRef, double& yPosRef)
{
	/*
	int pos = (yPos - yStart) / (height + padding);
	if (pos < items.size() && pos != currentSelected) {
		currentSelected = pos;
		for (auto& i : items) {
			i->setHover(false);
		}
		items.at(pos)->setHover(true);
		if(!rendToText)
			setUpdate(true);
	}
	*/
	if (showOthers) {
		bool test =
			(
				xPosRef >= xStart + xChildOffset &&
				xPosRef <= xStart + xChildOffset + (width * scale) &&
				yPosRef >= yStart + height + 1 + padding &&
				yPosRef <= highestY
				);
		if (items.size() > 0) {
			if (test && !(lastHover && lastHover->testHover(xPosRef, yPosRef))) {
				bool found = false;
				int current = std::round(items.size() / 2.), top = items.size(), bottom = 0;
				DropDownElement* item = nullptr;
				do {
					item = items[current - 1];
					if (found = item->testHover(xPosRef, yPosRef)) {
						if (lastHover) {
							lastHover->testHover(xPosRef, yPosRef);
						}
						lastHover = item;
					}
					if (!found) {
						if (yPosRef > item->getYStart()) {
							bottom = current;
							if (top != current)
								current += std::round((top - bottom) / 2.);
							else if (current < items.size() - 1)
								current++;
						}
						else {
							top = current;
							if (bottom != current)
								current -= std::round((top - bottom) / 2.);
							else if (current > 1)
								current--;
						}
					}
				} while (!found);
			}
			else if (!test && lastHover)
				lastHover->testHover(xPosRef, yPosRef);
		}
		/*
		for (auto i : items) {
			if (i->testHover(xPosRef, yPosRef)) {
				/*
				if (lastHover && lastHover != i && lastHover->getHover() && !lastHover->getClicked()) {
					lastHover->setHover(false);
					lastHover->setV(lastHover->get_Section_Vertices());
					if (lastHover->is_Hover_Lambda())
						lastHover->run_Hover_Lambda(xPosRef, yPosRef);
				}
				lastHover = i;
				break;
				
			}
		}
		/*
		if (highestY == 0 and items.size() > 0)
			get_Highest_Child_Y_Position();
		bool test =
			(
				xPos >= xStart + xChildOffset &&
				xPos <= xStart + xChildOffset + (width * scale) &&
				yPos >= yStart &&
				yPos <= highestY
				);
		if (test)
			for (auto i : items) {
				i->testHover(xPos, yPos);
				if (i->getHover()) {
					if (latest && i != latest && latest->getHover() && !latest->getClicked()) {
						latest->setHover(false);
						latest->setV(latest->get_Section_Vertices());
						if (latest->is_Hover_Lambda())
							latest->run_Hover_Lambda(xPos, yPos);
					}
					latest = i;
					break;
				}
			}
		if (!test && latest && latest->getHover()) {
			if (!latest->getClicked()) {
				latest->setHover(false);
				latest->setV(latest->get_Section_Vertices());
				if (latest->is_Hover_Lambda()) {
					double x = 0.0, y = 0.0;
					latest->run_Hover_Lambda(x, y);
				}
				latest = nullptr;
			}
		}
		*/
	}
	tb->testHover(xPosRef, yPosRef);
}

void DropDown::handleClicked(int& mouseButton, int& mouseState)
{
}

void DropDown::handleDraw(DropDownElement* dde)
{
	DrawObject*& dObj = dde->get_Draw_Object();
	DrawObject*& dObjStatic = dde->get_Draw_Object_Static_Base();
	TextDrawObject*& dObjText = dde->get_Draw_Object_Text();
	if (dObj && !(dObjStatic && dObj->getID() == dde->get_Section_Vertices()->getID()))
		dObj->setDraw(showOthers);
	if (dObjStatic)
		dObjStatic->setDraw(showOthers);
	if (dObjText)
		dObjText->setDraw(showOthers);
}

DropDownElement::DropDownElement(std::string name, Vertices* section, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(double& xPos, double& yPos)> const& hoverLambda, std::function<void(int& mouseButton, int& mouseState)> const& clickLambda) : GUIElement(name, nullptr, xStart, yStart, width, height, name)

{
	this->hoverLambda = hoverLambda;
	this->clickLambda = clickLambda;
	this->section = section;
	this->v = section;
	this->hover = hover;
	this->clicked = clicked;


	if (v->getWidth() != width || v->getHeight() != height) {
		scaleX = width / v->getWidth();
		scaleY = height / v->getHeight();
	}
}

DropDownElement::~DropDownElement()
{
	if (staticBaseSection)
		delete staticBaseSection;
}

void DropDownElement::createObject()
{
	if (getV()) {
		if(!dObj)
			dObj = new DrawObject((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), section->getID(), section->getVAO(), section->getTextPos());
		else {
			dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
			dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
			dObj->setID(v->getID());
			dObj->set_VAO(v->getVAO());
			dObj->set_Text_Pos(v->getTextPos());
		}
		if (scaleX != 1.0)
			dObj->set_Scale_X(scaleX);
		if (scaleY != 1.0)
			dObj->set_Scale_Y(scaleY);
	}
	if (text.size() > 0) {

		if (!dObjText)
			dObjText = new TextDrawObject(text);
		auto textSize = Get_Text_Size(text, "Black");
		generate_GUI_Text(dObjText, xStart, yStart, 0, width, height, text, textType, ellipsis, isCenteredText);
	}
	
}

void DropDownElement::createObjectNoRestriction()
{
	if (show && getV()) {
		bool change = false; double x = .0, y = .0;
		if (getV()) {
			if (!rendToText || (rendToText && !rtt)) {
				if (!dObj)
					dObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
				else {
					dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
					dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
					dObj->setID(v->getID());
					dObj->set_VAO(v->getVAO());
					dObj->set_Text_Pos(v->getTextPos());
				}
			}
			else {
				if (!dObj)
					dObj = new DrawObject(((xStart + xDrawOffset) / (double(rtt->getWidth()) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(rtt->getHeight()) / 2)), getV()->getID(), getV()->getVAO(), getV()->getTextPos());
				else {
					x = (xStart + xDrawOffset) / (double(rtt->getWidth()) / 2);
					if (x != dObj->getXPosition())
						change = true;
					dObj->setXPosition(x);
					y = (yStart + yDrawOffset) / (double(rtt->getHeight()) / 2);
					if (y != dObj->getYPosition())
						change = true;
					dObj->setYPosition(y);
					dObj->setID(v->getID());
					dObj->set_VAO(v->getVAO());
					dObj->set_Text_Pos(v->getTextPos());
				}
			}
			if (rtt != nullptr && !rendToText) 
				dObj->setRendToText(rtt);
			dObj->setOffsetX(xOffset / textureWidth);
			dObj->setOffsetY(yOffset / textureHeight);
			if (scaleX != 1.0)
				dObj->set_Scale_X(scaleX);
			if (scaleY != 1.0)
				dObj->set_Scale_Y(scaleY);
			if (staticSection && section && dObj->getID() == section->getID())
				dObj->setDraw(false);
		}
		if (staticSection && section) {
			if (!rendToText || (rendToText && !rtt)) {
				if(!staticBaseSection)
					staticBaseSection = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), section->getID(), section->getVAO(), section->getTextPos());
				else {
					staticBaseSection->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
					staticBaseSection->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
					staticBaseSection->setID(section->getID());
					staticBaseSection->set_VAO(section->getVAO());
					staticBaseSection->set_Text_Pos(section->getTextPos());
				}
			}
			else {
				if(!staticBaseSection)
					staticBaseSection = new DrawObject(((xStart) / (double(rtt->getWidth()) / 2)), 0.0 + ((yStart) / (double(rtt->getHeight()) / 2)), section->getID(), section->getVAO(), section->getTextPos());
				else {
					x = (xStart + xDrawOffset) / (double(rtt->getWidth()) / 2);
					if (x != staticBaseSection->getXPosition())
						change = true;
					staticBaseSection->setXPosition(x);
					y = (yStart + yDrawOffset) / (double(rtt->getHeight()) / 2);
					if (y != staticBaseSection->getYPosition())
						change = true;
					staticBaseSection->setYPosition(y);
					staticBaseSection->setID(section->getID());
					staticBaseSection->set_VAO(section->getVAO());
					staticBaseSection->set_Text_Pos(section->getTextPos());
				}
			}
			if (rtt != nullptr && !rendToText) 
				staticBaseSection->setRendToText(rtt);
			staticBaseSection->setOffsetX(xOffset / textureWidth);
			staticBaseSection->setOffsetY(yOffset / textureHeight);
		}
		if (text.size() > 0) {
			// Text
			if (!dObjText)
				dObjText = new TextDrawObject(text);
			else if (change) {
				dObjText->clear();
				dObjText = new TextDrawObject(text);
			}
			auto textSize = Get_Text_Size(text, "Black");
			if (!rendToText || (rendToText && !rtt))
				generate_GUI_Text(dObjText, xStart, yStart, textOffset, width, height, text, textType, ellipsis, isCenteredText);
			else
				generate_GUI_Text(dObjText, xStart, yStart, textOffset, width, height, text, textType, ellipsis, isCenteredText, rtt);
		}
	}
}

void DropDownElement::get_Draw_Object(DrawObjects*& objs, bool skipIfRTT) {
	if (!dObj)
		if (!rendToText)
			createObject();
		else
			createObjectNoRestriction();
	if (dObj)
		if (skipIfRTT && !rendToText)
			objs->addObject(dObj);
		else if (!skipIfRTT)
			objs->addObject(dObj);
	if(staticBaseSection)
		if (skipIfRTT && !rendToText)
			objs->addObject(staticBaseSection);
		else if (!skipIfRTT)
			objs->addObject(staticBaseSection);
}

void DropDownElement::set_Size_Based_On_Text()
{
	int textSize = Get_Text_Size(text, textType) + 30;
	if (hover && hover->getWidth() != textSize) {
		scaleX = textSize / hover->getWidth();
		//width = textSize;
	}

	//createObjectNoRestriction();
}

void DropDownElement::handleHover(double& xPosRef, double& yPosRef)
{
	if (v != clicked && v != hover && isHover) {
		setV(hover);
		if (staticBaseSection && dObj)
			dObj->setDraw(true);
		setUpdate(true);
		if (hoverLambda != NULL) {
			hoverLambda(xPosRef, yPosRef);
		}
	}
	else if (!isHover && v != clicked && v == hover) {
		setV(section);
		if (staticBaseSection && dObj)
			dObj->setDraw(false);
		setUpdate(true);
		if (hoverLambda != NULL) {
			hoverLambda(xPosRef, yPosRef);
		}
	}
}

void DropDownElement::handleClicked(int& mouseButton, int& mouseState)
{
	
	if (mouseButton == MOUSE_LEFT_CLICK && mouseState == MOUSE_RELEASE) {
		if (isClicked) {
			if (clicked)
				setV(clicked);
			else
				setV(section);
			setUpdate(true);
		}
		else {
			setV(hover);
			setUpdate(true);
		}
		if (clickLambda != NULL)
			clickLambda(mouseButton, mouseState);
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

void GUILabel::createObject()
{
	bool update = false;
	if (getV()) {
		if (!dObj) {
			dObj = new DrawObject((xStart / (double(screenWidthPixels) / 2)), 0.0 + (yStart / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
			update = true;
		}
		else {
			dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
			dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
			dObj->setID(v->getID());
			dObj->set_VAO(v->getVAO());
			dObj->set_Text_Pos(v->getTextPos());
		}
	}
	if (textRef.size() > 0) {
		if (textRef != text)
			text = textRef;
		if (!dObjText)
			dObjText = new TextDrawObject(text);
		generate_GUI_Text(dObjText, xStart, yStart + 5, textStartOffset, width - 16, height, text, textType, ellipsis, isCenteredText);
	}
	if (update)
		if (DrawObjectGroup* group = dynamic_cast<DrawObjectGroup*>(dObjs)) {
			group->clear();
			if (dObj)
				group->add(dObj);
			if (focusDObj)
				group->add(focusDObj);
			if (dObjText)
				group->add(dObjText);
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

void GUIGroup::createObject()
{
	if (!rendToText) {
		for (auto e : elements) {
			e->createObject();
		}
	}
}

void GUIGroup::createObjectNoRestriction()
{
	for (auto e : elements) {
		if (rendToText && rtt)
			e->setRenderToText(rtt, rendToText);
		e->createObjectNoRestriction();
	}
}

void GUIGroup::get_Draw_Object(DrawObjects*& objs, bool skipIfRTT)
{
	if (DrawObjectGroup* group = dynamic_cast<DrawObjectGroup*>(this->dObjs)) {
		group->clear();
		for (auto e : elements) {
			e->createObject();
			group->add(e->get_Draw_Object_Group());
		}
		objs->addObject(group);
	}
}

void GUIGroup::addElement(GUIElement * e)
{

	elements.push_back(e);
	if (rendToText && rtt) {
		e->setRenderToText(rtt, rendToText);
		e->createObjectNoRestriction();
	}
	else
		e->createObject();
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

bool GUIGroup::testHover(double& xPosRef, double& yPosRef)
{
	bool test =
		(
			xPosRef >= xStart &&
			xPosRef <= xStart + (width * scale) &&
			yPosRef >= yStart &&
			yPosRef <= yStart + (height * scale)
			);
	if (test && !skipHover) {

		if (!withinArea && mouseEnterExitLambda != NULL) {
			mouseEnterExitLambda(withinArea = true);
		}

		if (!rendToText) {
			isHover = true;
			handleHover(xPosRef, yPosRef);
		}
		else {
			double x = xPosRef - xStart + xRendToTextOffset, y = yPosRef - yStart + yRendToTextOffset;
			isHover = true;
			handleHover(x, y);
		}
	}
	else if (!test) {

		if (withinArea && mouseEnterExitLambda != NULL) {
			mouseEnterExitLambda(withinArea = false);
		}

		if (!rendToText) {
			if (isHover) {
				isHover = false;
				handleHover(xPosRef, yPosRef);
			}
		}
		else {
			double x = xPosRef - xStart + xRendToTextOffset, y = yPosRef - yStart + yRendToTextOffset;
			if (isHover) {
				isHover = false;
				handleHover(x, y);
			}
		}
	}
	return test;
}

bool GUIGroup::testClicked(double & xPos, double & yPos, int mouseButton, int mouseState, bool & isClickedAbove, GUIElement*& focusElement)
{
	double x = xPos, y = yPos;
	if (rendToText) {
		x = xPos - xStart + xRendToTextOffset;
		y = yPos - yStart + yRendToTextOffset;
	}
	bool test = (xPos >= xStart && xPos <= xStart + width && yPos >= yStart && yPos <= yStart + height);
	bool wasClicked = false;
	if (test) {
		for (auto e : elements) {
			e->testClicked(x, y, mouseButton, mouseState, isClickedAbove, focusElement);
		}
		if (clickLambda != NULL) {
			clickLambda(mouseButton, mouseState);
		}
	}
	return wasClicked;
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

bool GUIGroup::getUpdate()
{
	for (auto e : elements) {
		if (e->getUpdate())
			return true;
	}
	return false;
}

void GUIGroup::setUpdate(bool value)
{
	if (!value) {
		for (auto e : elements) {
			if (e->getUpdate()) {
				e->setUpdate(false);
			}
		}
	}
}

void GUIGroup::checkAnimation()
{
	if (animation)
		animation(update);
	for (auto e : elements) {
		e->checkAnimation();
	}
}

void GUIGroup::handleHover(double& xPosRef, double& yPosRef)
{
	if (hoverLambda != NULL)
		hoverLambda(xPos, yPos);


	for (auto& e : elements) {
		e->testHover(xPosRef, yPosRef);
	};
}

void GUIGroup::handleClicked(int& mouseButton, int & mouseState)
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
	auto lib = findByName(objLibraries, "GUI");
	lib->addObject("ScrollbarBackground_" + name, tempBackground = new Vertices("ScrollbarBackground_" + name, backgroundPart->getWidth(), scrollbarLength, backgroundPart));
	if(ignoreEndPartsBar)
		lib->addObject("ScrollbarBarBody_" + name, tempBarBody = new Vertices("ScrollbarBarBody_" + name, barMiddlePart->getWidth(), scrollbarLength, barMiddlePart));
	else
		lib->addObject("ScrollbarBarBody_" + name, tempBarBody = new Vertices("ScrollbarBarBody_" + name, barMiddlePart->getWidth(), scrollbarLength - (barTopPart->getHeight() + barBottomPart->getHeight()), barMiddlePart));
};

ScrollbarVertical::~ScrollbarVertical()
{
	delete topButton;
	delete bottomButton;
	auto lib = findByName(objLibraries, "GUI");
	if (tempBackground != nullptr) {
		lib->delete_Verteces("ScrollbarBackground_" + name);
		tempBackground = nullptr;
	}
	if (tempBarBody != nullptr) {
		lib->delete_Verteces("ScrollbarBarBody_" + name);
		tempBarBody = nullptr;
	}
	if (background)
		delete background;
	if (barTop)
		delete barTop;
	if (barMiddle)
		delete barMiddle;
	if (barBottomPart)
		delete barBottom;
}

void ScrollbarVertical::createObject()
{
	
	if (!rendToText) {
		int heightReached = 0, heightLimit = height - bottomButton->getHeight();
		{ // Create background and buttons
			// Top button
			topButton->setStartY(this->yStart);
			topButton->createObject();
			heightReached += topButton->getHeight();

			// Background
			if(!background)
				background = new DrawObject(xStart / (double(screenWidthPixels) / 2), (yStart + heightReached) / (double(screenHeightPixels) / 2), tempBackground->getID(), tempBackground->getVAO(), tempBackground->getTextPos());
			else {
				background->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				background->setYPosition(((yStart + yDrawOffset + heightReached) / (double(screenHeightPixels) / 2)));
				background->setID(tempBackground->getID());
				background->set_VAO(tempBackground->getVAO());
				background->set_Text_Pos(tempBackground->getTextPos());
			}
			heightReached += tempBackground->getHeight();

			// Bottom button
			bottomButton->setStartY(this->yStart + heightReached);
		    bottomButton->createObject();
		}

		
		{ // Create the scrollbar bar
			heightReached = scrollbarPosition, heightLimit = height - bottomButton->getHeight();
			if (!ignoreEndPartsBar) {
				if (!barTop)
					barTop = new DrawObject(xStart / (double(screenWidthPixels) / 2), (yStart + heightReached) / (double(screenHeightPixels) / 2), barTopPart->getID(), barTopPart->getVAO(), barTopPart->getTextPos());
				else {
					barTop->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
					barTop->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
					barTop->setID(barTopPart->getID());
					barTop->set_VAO(barTopPart->getVAO());
					barTop->set_Text_Pos(barTopPart->getTextPos());
				}
				heightReached += barTopPart->getHeight();
			}
			
			if (!barMiddle)
				barMiddle = new DrawObject(xStart / (double(screenWidthPixels) / 2), (yStart + heightReached) / (double(screenHeightPixels) / 2), tempBarBody->getID(), tempBarBody->getVAO(), tempBarBody->getTextPos());
			else {
				barMiddle->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				barMiddle->setYPosition(((yStart + yDrawOffset + heightReached) / (double(screenHeightPixels) / 2)));
				barMiddle->setID(tempBarBody->getID());
				barMiddle->set_VAO(tempBarBody->getVAO());
				barMiddle->set_Text_Pos(tempBarBody->getTextPos());
			}
			heightReached += tempBarBody->getHeight();

			if (!ignoreEndPartsBar) {
				if (!barBottom)
					barBottom = new DrawObject(xStart / (double(screenWidthPixels) / 2), (yStart + heightReached) / (double(screenHeightPixels) / 2), barBottomPart->getID(), barBottomPart->getVAO(), barBottomPart->getTextPos());
				else {
					barBottom->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
					barBottom->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
					barBottom->setID(barBottomPart->getID());
					barBottom->set_VAO(barBottomPart->getVAO());
					barBottom->set_Text_Pos(barBottomPart->getTextPos());
				}				
				heightReached += barBottomPart->getHeight();
			}
		}
		
	}
}

void ScrollbarVertical::createObjectNoRestriction()
{
}

void ScrollbarVertical::get_Draw_Object(DrawObjects*& objs, bool skipIfRTT)
{
	if (!background || !barMiddle)
		if (!rendToText)
			createObject();
		else
			createObjectNoRestriction();
	DrawObject* temp = nullptr;
	for (int i = 0; i < 2; i++) {
		if(i == 0)
			temp = topButton->get_Draw_Object();
		else
			temp = bottomButton->get_Draw_Object();
		
		if (temp)
			if (skipIfRTT && !rendToText)
				objs->addObject(temp);
			else if (!skipIfRTT)
				objs->addObject(temp);
	}
	if (background)
		if (skipIfRTT && !rendToText)
			objs->addObject(background);
		else if (!skipIfRTT)
			objs->addObject(background);
	if (barTop)
		if (skipIfRTT && !rendToText)
			objs->addObject(barTop);
		else if (!skipIfRTT)
			objs->addObject(barTop);
	if (barMiddle)
		if (skipIfRTT && !rendToText)
			objs->addObject(barMiddle);
		else if (!skipIfRTT)
			objs->addObject(barMiddle);
	if (barBottom)
		if (skipIfRTT && !rendToText)
			objs->addObject(barBottom);
		else if (!skipIfRTT)
			objs->addObject(barBottom);
}
void ScrollbarVertical::get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT)
{
	if (!dObjText)
		if (!rendToText)
			createObject();
		else
			createObjectNoRestriction();
	if (dObjText)
		if (skipIfRTT && !rendToText)
			objs->addObject(dObjText);
		else if (!skipIfRTT)
			objs->addObject(dObjText);
};

bool ScrollbarVertical::testHover(double& xPosRef, double& yPosRef)
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
			updateScrollbarPosition((scrollbarPosition + (yPosRef - mouseYPos)) * incrementSize, true);
		mouseYPos = yPosRef;
	}
	return test;
	
}

bool ScrollbarVertical::testClicked(double& xPosRef, double& yPosRef, int mouseButton, int mouseState, bool& isClickedAbove, GUIElement* focusElement)
{
	topButton->testClicked(xPosRef, yPosRef, mouseButton, mouseState, isClickedAbove, focusElement);
	bottomButton->testClicked(xPosRef, yPosRef, mouseButton, mouseState, isClickedAbove, focusElement);
	bool test = (xPosRef >= xStart && xPosRef <= xStart + width && yPosRef >= yStart + scrollbarPosition && yPosRef <= yStart + scrollbarPosition + scrollbarLength);
	if (mouseButton == MOUSE_LEFT_CLICK && mouseState == MOUSE_PRESS && !isClicked && test) {
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
	if (!b) {
		if(topButton->getUpdate())
			topButton->setUpdate(false);
		if (bottomButton->getUpdate())
			bottomButton->setUpdate(false);
		update = false;
		if (rendToText && rtt)
			createObjectNoRestriction();
		else
			createObject();
	}
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
	setIncrementSize( double(end) / getScrollbarIncrementsPoints());
	if (incrementSize > 10000) incrementSize = 0;
	if (incrementSize != 0)
		scrollbarPosition = pos / incrementSize;
	else
		scrollbarPosition = 0;
	updateScrollbarLengthAndPosition(length, pos);
}

void ScrollbarVertical::updateScrollbarPosition(double pos, bool useLambda = true)
{
	if (incrementSize != 0) {
		pos /= incrementSize;
	}
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

void ScrollbarVertical::change_Scrollbar_Position(int pos, bool useLambda = true)
{
	if (scrollbarPosition != pos) {
		changeInPosition = pos;
		scrollbarPosition += pos;
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

ScaleableElement::ScaleableElement(std::string name, int xStart, int yStart, int width, int height, Vertices* vertices) : GUIElement(name, vertices, xStart, yStart, width, height, "")
{
	if (v->getWidth() != width || v->getHeight() != height) {
		scaleX = width / v->getWidth();
		scaleY = height / v->getHeight();
	}
}

ScaleableElement::~ScaleableElement()
{
}


void ScaleableElement::createObject()
{
	if (!rendToText && v) {
		if(!dObj)
			dObj = new DrawObject(xStart / (double(screenWidthPixels) / 2), (yStart) / (double(screenHeightPixels) / 2), v->getID(), v->getVAO(), v->getTextPos());
		else {
			dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
			dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
			dObj->setID(v->getID());
			dObj->set_VAO(v->getVAO());
			dObj->set_Text_Pos(v->getTextPos());
		}
		if (rtt != nullptr) 
			dObj->setRendToText(rtt);
		dObj->setOffsetX(xOffset / textureWidth);
		dObj->setOffsetY(yOffset / textureHeight);
		dObj->set_Scale_X(scaleX);
		dObj->set_Scale_Y(scaleY);
	}
}

void ScaleableElement::createObjectNoRestriction()
{
}

void ScaleableElement::handleClicked(int& mouseButton, int& mouseState)
{
	if (clickLambda != NULL)
		clickLambda(mouseButton, mouseState);
}

TextField::TextField(std::string name, int xStart, int yStart, int width, int height, Vertices* field, Vertices* fieldFocus) : field(field), GUIElement(name, xStart, yStart, width, height)
{
	set_FocusAble(true);
	set_Focus_Vertices(fieldFocus);

	setV(field);
	textStartYOffset = (height - textSize) / 2;
	rowLength = width - (textStartXOffset + textSize);

	change_Size(width, height);

	isCenteredText = 3;

	setAnimationLambda([this](bool& update)
		{
			if (this->getFocus() && marker) {
				__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				auto then = this->getTimeStamp();
				auto marker = this->getMarker();
				if ((then == 0 || (now - then) > 500)) {
					this->setTimeStamp(now);
					marker->setDraw(!marker->getDraw());
				}
			}
		});
}

TextField::~TextField()
{
}

/**
*
* Draw the contents of the TextField.
*
*@param ob The buffer which stores the objects to be drawn onto the screen.
*/
void TextField::createObject()
{
	if (!rendToText) {
		if (v && show) {
			if(!dObj)
				dObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
			else {
				dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
				dObj->setID(v->getID());
				dObj->set_VAO(v->getVAO());
				dObj->set_Text_Pos(v->getTextPos());
			}
			if (rtt != nullptr) 
				dObj->setRendToText(rtt);
			dObj->setOffsetX(xOffset / textureWidth);
			dObj->setOffsetY(yOffset / textureHeight);
			dObj->set_Scale_X(scaleX);
			dObj->set_Scale_Y(scaleY);

		}
		if (focusVertices) {
			if (!focusDObj) {
				focusDObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), focusVertices->getID(), focusVertices->getVAO(), focusVertices->getTextPos());
				focusDObj->setDraw(false);
			}
			else
			{
				focusDObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				focusDObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
				focusDObj->setID(focusVertices->getID());
				focusDObj->set_VAO(focusVertices->getVAO());
				focusDObj->set_Text_Pos(focusVertices->getTextPos());
			}
			focusDObj->setOffsetX(xOffset / textureWidth);
			focusDObj->setOffsetY(yOffset / textureHeight);
			if (scaleX != 1.0)
				focusDObj->set_Scale_X(scaleX);
			if (scaleY != 1.0)
				focusDObj->set_Scale_Y(scaleY);
		}
		{ // Do text stuff
			int textStartOffset = 0, textWidth = 0;
			double x = xStart, y = yStart;

			if (!dObjText) {
				dObjText = new TextDrawObject(text);
				markerPos = text.size();
			}
			else if(dObjText->get_Text() != text) {
				markerPos = text.size();
			}
			generate_GUI_Text(dObjText, x, y, textStartOffset, width, height, text, textType, ellipsis, isCenteredText);

			if (!marker && dObjText) {
				auto& letterInformation = letterLibrary.getLetterInformation(std::string(1, '|') + textType);
				if (letterInformation.size() > 0) {
					marker = new DrawObject(0, 0, letterInformation[2], letterInformation[0], letterInformation[1]);
					marker->setDraw(false);
				}
			}
			
			auto textSize = Get_Text_Size(text, textType);
			if (marker) {
				textWidth = letterLibrary.getLetterInformation(std::string(1, '1') + textType)[5];
				if (isCenteredText == 1) {
					x += (width / 2) - (textSize / 2);
					y += (height - textWidth) / 2.;
				}
				else {
					if (isCenteredText == 2)
						x += (width / 2.) - (textSize / 2.);
					else {
						x += textStartOffset;
					}
					if (isCenteredText == 3)
						y += (height - textWidth) / 2.;
				}
				x += Get_Text_Size_At_Pos(text, markerPos, textType);
				x /= (double(screenWidthPixels) / 2); y /= (double(screenHeightPixels) / 2);
				marker->setXPosition(x);
				marker->setYPosition(y);
			}
		}
	}
}

void TextField::get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT)
{
	if (!dObjText)
		if (!rendToText)
			createObject();
		else
			createObjectNoRestriction();
	if (dObjText)
		if (skipIfRTT && !rendToText)
			objs->addObject(dObjText);
		else if (!skipIfRTT)
			objs->addObject(dObjText);
	if (marker)
		if (skipIfRTT && !rendToText)
			objs->addObject(marker);
		else if (!skipIfRTT)
			objs->addObject(marker);
}

void TextField::change_Width(int width)
{
	scaleX = width / v->getWidth();
}

void TextField::change_Height(int height)
{
	scaleY = height / v->getHeight();
}

void TextField::change_Size(int width, int height)
{
	if (v->getWidth() != width || v->getHeight() != height) {
		scaleX = width / v->getWidth();
		scaleY = height / v->getHeight();
	}
}

void TextField::setFocus(bool b)
{
	focus = b;
	if(focusDObj)
		if (focus) {
			if (marker)
				marker->setDraw(true);
			focusDObj->setDraw(true);
		}
		else {
			if (marker)
				marker->setDraw(false);
			focusDObj->setDraw(false);
		}
}

/**
* Handle incoming clicks.
*
*@param mouseState The state of the mouse (left click == 1, right click = 0, middle click = 2).
*/
void TextField::handleClicked(int& mouseButton, int& mouseState)
{
	if(mouseButton == MOUSE_LEFT_CLICK && mouseState == MOUSE_RELEASE){
		if (!focus) {
			if (marker)
				marker->setDraw(false);
			setV(field);
		}
		else {
			if (marker)
				marker->setDraw(true);
		}
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
		if(lambda)
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

bool TextField::testClicked(double & xPos, double & yPos, int mouseButton, int mouseState, bool& isClickedAbove, GUIElement*& focusElement)
{
	bool test =
		(
			xPos >= xStart &&
			xPos <= xStart + (width * scale) &&
			yPos >= yStart &&
			yPos <= yStart + (height * scale)
			);
	if (show && test) {
		if (mouseState == MOUSE_PRESS) {
			if (!isClicked)
				isClickedAbove = true;
			setFocus(!getFocus());
			if (focusElement && focusElement != this) {
				focusElement->setFocus(false);
				focusElement = nullptr;
			}
			if (getFocus())
				focusElement = this;
		}
		handleClicked(mouseButton, mouseState);
	}
	return test;
}

TextArea::TextArea(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string text) : GUIElement(name, v, xStart, yStart, width, height, text)
{

}

void TextArea::createObject()
{
	if (!rendToText) {
		if (v && show) {
			if (!dObj)
				dObj = new DrawObject(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)), 0.0 + ((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)), v->getID(), v->getVAO(), v->getTextPos());
			else {
				dObj->setXPosition(((xStart + xDrawOffset) / (double(screenWidthPixels) / 2)));
				dObj->setYPosition(((yStart + yDrawOffset) / (double(screenHeightPixels) / 2)));
				dObj->setID(v->getID());
				dObj->set_VAO(v->getVAO());
				dObj->set_Text_Pos(v->getTextPos());
			}
			if (rtt != nullptr)
				dObj->setRendToText(rtt);
			dObj->setOffsetX(xOffset / textureWidth);
			dObj->setOffsetY(yOffset / textureHeight);
			dObj->set_Scale_X(scaleX);
			dObj->set_Scale_Y(scaleY);

		}
		{ // Do text stuff
			if (!dObjText)
				dObjText = new TextDrawObject(text);
			generate_GUI_Text(dObjText, xStart, yStart, 0, width, height, text, textType, ellipsis, isCenteredText);

		}
	}
}

GUIDialog::GUIDialog(std::string name, Vertices* base, Vertices* topBar, Vertices* buttonClose, Vertices* buttonCloseHover, Vertices* buttonCloseClick, int xStart, int yStart, int width, int height, int topBarHeight, int topBarButtonWidth) : base(base), topBar(topBar), GUIElement(name, v, xStart, yStart, width, height, "")
{
	closeBtn = new Button("closeButton", buttonClose, nullptr, buttonCloseHover, buttonCloseClick, xStart + width - topBarButtonWidth, yStart, topBarButtonWidth, topBarHeight, "X",
		[](int& mouseButton, int& mouseState)
		{

		});
}

GUIDialog::~GUIDialog()
{
	if (dObjs)
		delete dObjs;
	if (closeBtn)
		delete closeBtn;
}

void GUIDialog::get_Draw_Object(DrawObjects*& objs, bool skipIfRTT)
{
	if (!dObjs)
		createObject();
	else
		createObjectNoRestriction();

	if (dObjs)
		if (skipIfRTT && !rendToText)
			objs->addObject(dObjs);
		else if (!skipIfRTT)
			objs->addObject(dObjs);
}
