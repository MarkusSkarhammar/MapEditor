#ifndef GUI_H
#define	GUI_H

#include <string>
#include <vector>
#include "Object.h"
#include <functional>


class GUIText;

class GUIElement {
public:
	GUIElement(std::string name);
	GUIElement(std::string name, int xStart, int yStart, int width, int height) : name(name), xStart(xStart), yStart(yStart), width(width), height(height) { xStartText = xStart; yStartText = yStart; };
	GUIElement(std::string name, int xStartText, int yStartText, int xStart, int yStart, int width, int height) : name(name), xStartText(xStartText), yStartText(yStartText), xStart(xStart), yStart(yStart), width(width), height(height) { rendToText = true; };
	GUIElement(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string text);
	GUIElement(std::string name, Vertices* v, int xStartText, int yStartText, int xStart, int yStart, int width, int height, std::string text);
	virtual void testHover(double& xPos, double& yPos);
	virtual void testClicked(double& xPos, double& yPos, int mouseState, bool& isClicked);
	virtual void createObject(Objects & ob);
	virtual void createObjectNoRestriction(Objects & ob);
	virtual void testScroll(double& scroll);
	std::string& getName();
	Vertices*& getV() { return v; };
	void setV(Vertices* p) { v = p; };
	virtual void setUpdate(bool value);
	virtual bool getUpdate();
	bool getHover();
	void setHover(bool b) { isHover = b; };
	bool getClicked();
	void setText(std::string s);
	void setCentered(bool b) { isCenteredText = b; };
	void toggleShow() { show = !show; };
	bool getShow() { return show; };
	void setStartX(int x) { xStart = x; };
	virtual void setStartY(int y) { yStart = y; };
	void setXStartText(int x) { xStartText = x; };
	void setYStartText(int y) { yStartText = y; };
	void addToStartXText(int x) { xStartText += x; };
	void addToStartYText(int y) { yStartText += y; };
	void setOffsetX(int v) { xOffset = v; };
	void setOffsetY(int v) { yOffset = v; };
	virtual void setRendToTextOffsetX(int v) { xRendToTextOffset = v; };
	virtual void setRendToTextOffsetY(int v) { yRendToTextOffset = v; };
	void setRenderToText() { rendToText = true; };
	void setRenderToText(RendToText* v) { rtt = v; };
	void setTextureWidth(double v) { textureWidth = v; };
	void setTextureHeight(double v) { textureHeight = v; };
	void setWidth(int v) { width = v; };
	void setHeight(int v) { height = v; };
	int getXStart() { return xStart; };
	int getYStart() { return yStart; };
	void setCheckOutsideOfArea() { checkOutsideOfArea = true; };
	void setScrollLambda(std::function<void(double& v)> lambda) { scrollLambda = lambda; };
	void setHoverLambda(std::function<void(double& xPos, double& yPos)> lambda) { hoverLambda = lambda; };
	void setClickLambda(std::function<void(int& mouseState)> lambda) { clickLambda = lambda; };
protected:
	std::string name{ "" }, text{ "" };
	RendToText* rtt = nullptr;
	bool isClicked{ false }, isHover{ false }, isCenteredText{ false }, show{ true }, rendToText{ false }, checkOutsideOfArea{ false };
	int xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 }, xStartText{ 0 }, yStartText{ 0 }, xOffset{ 0 }, yOffset{ 0 }, xRendToTextOffset{ 0 }, yRendToTextOffset{ 0 };
	double textureWidth{ 2048.f }, textureHeight{ 2048.f };
	Vertices* v = nullptr;
	std::function<void(double& v)> scrollLambda{ NULL };
	std::function<void(double& xPos, double& yPos)> hoverLambda{ NULL };
	std::function<void(int& mouseState)> clickLambda{ NULL };
private:
	bool update{ false };
	virtual void handleHover();
	virtual void handleClicked(int& mouseState);
};

class Button : public GUIElement {
public:
	Button(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::string text, std::function<void(int& mouseState)> lambda);
protected:
	Vertices *button{ nullptr }, *hover{ nullptr }, *clicked{ nullptr };
private:
	void handleHover();
	void handleClicked(int& mouseState);
};

class ExpandingButton : public Button {
public:
	ExpandingButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, float scale, std::string text, std::function<void(int& mouseState)> lambda);
	void createObject(Objects& ob);
private:
	float scaling{ 1.0 };
};

class ToggleButton : public GUIElement {
public:
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda);
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStartText, int yStartText, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda);
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda, std::function<void(double& x, double& y)> hoverLambda);
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStartText, int yStartText, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda, std::function<void(double& x, double& y)> hoverLambda);
	void createObject(Objects & ob);
	void createObjectNoRestriction(Objects & ob);
	void resetToggle();
	bool& getToggle() { return toggle; };
	void toggleCleanreset() { cleanReset = !cleanReset; };
	void toggleHover() { doHover = !doHover; };
	void toggleAlwaysShowBase() { alwaysShowBase = !alwaysShowBase; };
	void setOffset(int v) { offset = v; };
	void setTextOffset(int v) { textOffset = v; };
	void setButton(Vertices* v) { button = v; };
	Vertices* getButton() { return button; };
	Vertices* getHoverVertices() { return hover; };
	Vertices* getClickedVertices() { return clicked; };
	void setHover(Vertices* v) { hover = v; };
	void setClicked(Vertices* v) { clicked = v; };
	void setXDrawOffset(int v) { xDrawOffset = v; };
	void setYDrawOffset(int v) { yDrawOffset = v; };
	void setItemID(int id ) { itemID = id; }
	int getItemID() { return itemID; };
private:
	Vertices *button, *hover, *clicked;
	bool toggle{ false }, cleanReset{ true }, doHover{ true }, alwaysShowBase{ false };
	int offset{ 0 }, textOffset{ 0 }, xDrawOffset{ 0 }, yDrawOffset{ 0 }, itemID{ -1 };
	void handleHover();
	void handleClicked(int& mouseState);
};

class ToggleButtonGroup : public GUIElement {
public:
	ToggleButtonGroup(std::string name) : GUIElement(name) {};
	ToggleButtonGroup(std::string name, int xStart, int yStart, int width, int height) : GUIElement(name, xStart, yStart, width, height) {};
	~ToggleButtonGroup();
	void createObject(Objects & ob);
	void createObjectNoRestriction(Objects & ob);
	void testScroll(double& scroll);
	ToggleButton* getElementByName(std::string name);
	ToggleButton* getElementByClicked();
	ToggleButton* getElementByHover();
	void addElement(GUIElement* e);
	bool getUpdate();
	void testHover(double& xPos, double& yPos);
	void testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	void resetAll();
	void clearAll();
	void setUpdate(bool b);
	void setCheckOutside() { checkOutside = true; };
private:
	std::vector<ToggleButton*> toggleButtons;
	ToggleButton* currentlyPressed{nullptr};
	bool checkOutside{ false };
	void handleHover();
	void handleClicked(int& mouseState);
};

class GUIPanel {
public:
	GUIPanel(std::string name);
	GUIPanel(std::string name, int x, int y, int width, int height);
	~GUIPanel();
	void clear();
	void addElement(GUIElement* e);
	GUIElement* getElementByName(std::string name);
	void removeElementByName(std::string name);
	void createObjects(Objects& ob);
	void reCreateObjects(Objects& ob);
	void setDimensions(int x, int y, int width, int height);
	void checkHover(Objects& ob, double& xPos, double& yPos);
	void checkClicked(Objects& ob, double& xPos, double& yPos, int mouseState);
	void checkScroll(Objects& ob, double& xPos, double& yPos, double& scroll);
	void checkUpdates(Objects& ob);
	void setCheckIfOutside(bool value) { checkIfOutside = value; };
	void toggleShow() { show = !show; };
	bool getShow() { return show; };
	void updateLabels(Objects & ob);
private:
	int xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 };
	bool checkIfOutside{ false }, isClicked{ false }, show{ true };
	std::string name;
	std::vector<GUIElement*> elements;
};

class DropDownElement : public GUIElement {
public:
	DropDownElement(std::string name, Vertices* section, Vertices* hover, int xStart, int yStart, int width, int height, std::function<void()> const& lambda);
	virtual void createObject(Objects & ob);
	void runLambda() { lambda(); };
private:
	std::function<void()> const lambda;
	Vertices *section, *hover;
	void handleHover();
	void handleClicked(int& mouseState);
};

class DropDown : public GUIElement{
public:
	DropDown(std::string name, Vertices* topSection, Vertices* middleSection, Vertices* bottomSection, Vertices* hover, int xStart, int yStart, int widthEntry, int heightEntry);
	~DropDown();
	void add(std::string text, std::function<void()> const& lambda);
	void createObject(Objects & ob);
	void testHover(double& xPos, double& yPos);
	void testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	void setPadding(int i) { padding = i; };
	bool getUpdate();
	void setUpdate(bool value);
private:
	std::vector<DropDownElement*> items;
	int xStart, yStart, width, height, currentSelected{ -1 }, padding { 0 };
	Vertices* topSection, *middleSection, *bottomSection, *hover;
	void handleHover();
	void handleClicked(int& mouseState);
};

class GUIText : public GUIElement {
public:
	GUIText(std::string name, int xStart, int yStart, int width, int height, std::string text) : GUIElement(name, xStart, yStart, width, height) { setText(text); };
	void createObject(Objects & ob);
private:
};

class GUILabel : public GUIElement {
public:
	GUILabel(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string text, std::string& ref) : textRef(ref), GUIElement(name, v, xStart, yStart, width, height, text) { };
	void createObject(Objects & ob);
	void update(Objects & ob);
private:
	std::string& textRef;
	Object* o = nullptr;
	int size = 0;
};

class GUIGroup : public GUIElement {
public:
	GUIGroup(std::string name, int xStart, int yStart, int width, int height): GUIElement(name, xStart, yStart, width, height) {};
	~GUIGroup();
	void clear();
	void createObject(Objects & ob);
	void createObjectNoRestriction(Objects & ob);
	void addElement(GUIElement* e);
	GUIElement* getElementByName(std::string name);
	void removeElementByName(std::string name);
	std::vector<GUIElement*> getElements() { return elements; };
	void testHover(double& xPos, double& yPos);
	void testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	void updateStartX(int x);
	void setStartY(int y);
private:
	std::vector<GUIElement*> elements;
	void handleHover();
	void handleClicked(int& mouseState);
};

#endif