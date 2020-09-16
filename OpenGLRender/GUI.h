#ifndef GUI_H
#define	GUI_H

#include <string>
#include <vector>
#include "DrawObject.h"
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
	virtual bool testClicked(double& xPos, double& yPos, int mouseState, bool& isClicked);
	virtual void createObject(DrawObjects* ob);
	virtual void checkAnimation();
	virtual void createObjectNoRestriction(DrawObjects* ob);
	virtual void testScroll(double& scroll);
	std::string& getName();
	Vertices*& getV() { return v; };
	void setV(Vertices* p) { v = p; };
	virtual void setUpdate(bool value);
	virtual bool getUpdate();
	bool getHover();
	void setHover(bool b) { isHover = b; };
	bool getClicked();
	void toggleClicked();
	void setText(std::string s);
	void setCenteredText(bool b) { isCenteredText = b; };
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
	int getRendToTextOffsetX() { return xRendToTextOffset; };
	int getRendToTextOffsetY() { return yRendToTextOffset; };
	void setRenderToText() { rendToText = true; };
	void setRenderToText(RendToText* v) { rtt = v; };
	void setRenderToText(RendToText* v, bool b) { rtt = v; rendToText = b; };
	RendToText* getRenderToText() { return rtt; };
	void setTextureWidth(double v) { textureWidth = v; };
	void setTextureHeight(double v) { textureHeight = v; };
	void setWidth(int v) { width = v; };
	void setHeight(int v) { height = v; };
	int getWidth() { return width; };
	int getHeight() { return height; };
	int getXStart() { return xStart; };
	int getYStart() { return yStart; };
	void setCheckOutsideOfArea() { checkOutsideOfArea = true; };
	void setXDrawOffset(int v) { xDrawOffset = v; };
	void setYDrawOffset(int v) { yDrawOffset = v; };
	void setScale(double v) { scale = v; };
	double getScale() { return scale; };
	void setScrollLambda(std::function<void(double& v)> lambda) { scrollLambda = lambda; };
	void setHoverLambda(std::function<void(double& xPos, double& yPos)> lambda) { hoverLambda = lambda; };
	void setClickLambda(std::function<void(int& mouseState)> lambda) { clickLambda = lambda; };
	void setAnimationLambda(std::function<void()> lambda) { animation = lambda; };
	void addMiscellaneousID(std::string note, int v) { miscellaneousIDs.push_back(std::pair<std::string, int>(note, v)); };
	void addMiscellaneousID(std::string note, int v, int at) {
		if (miscellaneousIDs.size() <= at) miscellaneousIDs.push_back(std::pair<std::string, int>(note, v));
		else
			miscellaneousIDs.at(at) = std::pair<std::string, int>(note, v);
	};
	int getMiscellaneousID(int pos) { return miscellaneousIDs.at(pos).second; };
	std::vector<std::pair<std::string, int>>& getMiscellaneousIDsList() { return miscellaneousIDs; };
	virtual std::vector<GUIElement*> getElements() { return std::vector<GUIElement*>(); };
	virtual void handleKeyStroke(int& key, int& action, int& mods);
	bool getFocus() { return focus; };
	void setFocus(bool b) { focus = b; };
	void setAnimationBool(bool b) { doAnimation = b; };
	bool getAnimationBool() { return doAnimation; };
	void setTimeStamp(__int64 time) { timeStamp = time; };
	__int64 getTimeStamp() { return timeStamp; };
	int getTextOffset() { return textOffset; };
	void setTextOffset(int v) { textOffset = v; };
	std::string getText() { return text; };
protected:
	std::string name{ "" }, text{ "" };
	RendToText* rtt = nullptr;
	bool isClicked{ false }, isHover{ false }, isCenteredText{ false }, show{ true }, rendToText{ false }, checkOutsideOfArea{ false }, update{ false }, focus{ false }, doAnimation{ false };
	int xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 }, xStartText{ 0 }, yStartText{ 0 }, xOffset{ 0 }, yOffset{ 0 }, xRendToTextOffset{ 0 }, yRendToTextOffset{ 0 }, xDrawOffset{ 0 }, yDrawOffset{ 0 }, textOffset{ 0 };
	double textureWidth{ 2048.f }, textureHeight{ 2048.f }, scale{ 1.0 };
	__int64 timeStamp{ 0 };
	std::vector<std::pair<std::string, int>> miscellaneousIDs = {};
	Vertices* v = nullptr;
	std::function<void(double& v)> scrollLambda{ NULL };
	std::function<void(double& xPos, double& yPos)> hoverLambda{ NULL };
	std::function<void(int& mouseState)> clickLambda{ NULL };
	std::function<void()> animation{ NULL };
private:
	virtual void handleHover();
	virtual void handleClicked(int& mouseState);
	virtual void handleAnimation();
};

class ScaleableElement : public GUIElement {
public:
	ScaleableElement(std::string name, int xStart, int yStart, int width, int height, Vertices* vertices);
	~ScaleableElement();
	void createObject(DrawObjects* ob);
	void createObjectNoRestriction(DrawObjects* ob);
private:
};

class Button : public GUIElement {
public:
	Button(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::string text, std::function<void(int& mouseState)> lambda);
	void createObject(DrawObjects* ob);
	void createObjectNoRestriction(DrawObjects* ob);
	bool testClicked(double& xPos, double& yPos, int mouseState, bool& isClicked);
protected:
	Vertices *button{ nullptr }, *hover{ nullptr }, *clicked{ nullptr };
private:
	void handleHover();
	void handleClicked(int& mouseState);
};

class ExpandingButton : public Button {
public:
	ExpandingButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, float scale, std::string text, std::function<void(int& mouseState)> lambda);
	void createObject(DrawObjects* ob);
private:
	float scaling{ 1.0 };
};

class ToggleButton : public GUIElement {
public:
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda);
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStartText, int yStartText, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda);
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda, std::function<void(double& x, double& y)> hoverLambda);
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStartText, int yStartText, int xStart, int yStart, int width, int height, std::function<void(int& mouseState)> lambda, std::function<void(double& x, double& y)> hoverLambda);
	void createObject(DrawObjects* ob);
	void createObjectNoRestriction(DrawObjects* ob);
	void resetToggle();
	bool& getToggle() { return toggle; };
	void doToggle(int mouseState);
	void setPressed(bool b);
	bool getCleanReset() { return cleanReset; };
	void toggleCleanReset() { cleanReset = !cleanReset; };
	void toggleHover() { doHover = !doHover; };
	void toggleAlwaysShowBase() { alwaysShowBase = !alwaysShowBase; };
	void setOffset(int v) { offset = v; };
	void setButton(Vertices* v) { button = v; };
	Vertices* getButton() { return button; };
	Vertices* getHoverVertices() { return hover; };
	Vertices* getClickedVertices() { return clicked; };
	void setHoverVertices(Vertices* v) { hover = v; };
	void setClicked(Vertices* v) { clicked = v; };
	void setClickedOrder(int order) { if(clickedOrder == 0) clickedOrder = order; };
	int& getClickedOrder() { return clickedOrder; };
private:
	Vertices *button, *hover, *clicked;
	bool toggle{ false }, cleanReset{ true }, doHover{ true }, alwaysShowBase{ false };
	int offset{ 0 }, clickedOrder{ 0 };
	void handleHover();
	void handleClicked(int& mouseState);
};

class ToggleButtonGroup : public GUIElement {
public:
	ToggleButtonGroup(std::string name) : GUIElement(name) {};
	ToggleButtonGroup(std::string name, int xStart, int yStart, int width, int height) : GUIElement(name, xStart, yStart, width, height) {};
	~ToggleButtonGroup();
	void createObject(DrawObjects *ob);
	void createObjectNoRestriction(DrawObjects *ob);
	void testScroll(double& scroll);
	ToggleButton* getElementByName(std::string name);
	ToggleButton* getElementByClicked();
	ToggleButton* getElementByHover();
	std::vector<ToggleButton*> getElementsByClicked();
	std::vector<ToggleButton*> getElementsByClicked(bool checkClickedOrder);
	std::vector<ToggleButton*>& getToggleButtons() { return toggleButtons; };
	void addElement(GUIElement* e);
	void addElementOrReplace(GUIElement* e);
	bool getUpdate();
	void testHover(double& xPos, double& yPos);
	bool testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	void resetAll();
	void resetHover();
	void clearAll();
	void setUpdate(bool b);
	void setCheckOutside() { checkOutside = true; };
	void setDoNotResetAfterToggle(bool v) { doNotResetAfterToggle = v; for (auto b : toggleButtons) { if((doNotResetAfterToggle && !b->getCleanReset()) || (!doNotResetAfterToggle && b->getCleanReset())) b->toggleCleanReset(); } };
	bool getDoNotResetAfterToggle() { return doNotResetAfterToggle; }
	void testClickedAt(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	void setClickedOrder(ToggleButton* tb);
	void setClickedOrder(int order) { clickedOrder = order; };
	int& getClickedOrder() { return clickedOrder; };
	void resetClickedOrder();
	ToggleButton* getCurrentlyPressed() { return currentlyPressed; };
private:
	std::vector<ToggleButton*> toggleButtons;
	ToggleButton* currentlyPressed{nullptr};
	bool checkOutside{ false }, doNotResetAfterToggle{ false };
	void handleHover();
	void handleClicked(int& mouseState);
	int clickedOrder{ 0 };
};

class GUIPanel {
public:
	GUIPanel(std::string name);
	GUIPanel(std::string name, DrawObjects* ob, int x, int y, int width, int height);
	~GUIPanel();
	void setObjects(DrawObjects *ob) { this->ob = ob; };
	void clear();
	void addElement(GUIElement *e);
	GUIElement* getElementByName(std::string name);
	std::vector<GUIElement*> getEveryElement();
	void removeElementByName(std::string name);
	void createObjects();
	void reCreateObjects();
	void clearObjects();
	void setDimensions(int x, int y, int width, int height);
	void checkHover(double& xPos, double& yPos);
	void checkClicked(double& xPos, double& yPos, int mouseState);
	void checkScroll(double& xPos, double& yPos, double& scroll);
	void checkKeyStroke(int& key, int& action, int& mods);
	void checkUpdates();
	void setCheckIfOutside(bool value) { checkIfOutside = value; };
	void toggleShow() { show = !show; };
	bool getShow() { return show; };
	bool containsLabel();
private:
	int xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 };
	bool checkIfOutside{ false }, isClicked{ false }, show{ false }, testedLabel{ false }, isLabel{ false };
	DrawObjects *ob{ nullptr };
	std::string name;
	std::vector<GUIElement*> elements;
};

class DropDownElement : public GUIElement {
public:
	DropDownElement(std::string name, Vertices* section, Vertices* hover, int xStart, int yStart, int width, int height, std::function<void()> const& lambda);
	virtual void createObject(DrawObjects *ob);
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
	void createObject(DrawObjects* ob);
	void testHover(double& xPos, double& yPos);
	bool testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	void setPadding(int i) { padding = i; };
	bool getUpdate();
	void setUpdate(bool value);
	bool getShowDropDown() { return showOthers; };
	void toggleShowDropDown() { showOthers = !showOthers; update = true; };
	ToggleButton* getButton() { return tb; };
	std::vector<DropDownElement*> geDropDownElements() { return items; };
private:
	std::vector<DropDownElement*> items;
	int xStart, yStart, width, height, currentSelected{ -1 }, padding { 0 };
	Vertices* topSection, *middleSection, *bottomSection, *hover;
	ToggleButton* tb{ nullptr };
	bool showOthers{ false };
	void handleHover();
	void handleClicked(int& mouseState);
};

class GUIText : public GUIElement {
public:
	GUIText(std::string name, int xStart, int yStart, int width, int height, std::string text) : GUIElement(name, xStart, yStart, width, height) { setText(text); };
	void createObject(DrawObjects & ob);
private:
};

class GUILabel : public GUIElement {
public:
	GUILabel(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string& textRef) : textRef(textRef), GUIElement(name, v, xStart, yStart, width, height, "") { };
	void createObject(DrawObjects* ob);
	void setTextStartOffset(int offset) { textStartOffset = offset; };
	int getTextStartOffset() { return textStartOffset; };
	void setTextType(int type) { textType = type; };
	std::string getTextType() { return textType; };
private:
	int textStartOffset{ 0 };
	std::string textType{ "Black" };
	DrawObject* o = nullptr;
	std::string& textRef;
};

class GUIGroup : public GUIElement {
public:
	GUIGroup(std::string name, int xStart, int yStart, int width, int height): GUIElement(name, xStart, yStart, width, height) {};
	~GUIGroup();
	void clear();
	void createObject(DrawObjects* ob);
	void createObjectNoRestriction(DrawObjects* ob);
	void addElement(GUIElement* e);
	GUIElement* getElementByName(std::string name);
	void removeElementByName(std::string name);
	std::vector<GUIElement*> getElements() { return elements; };
	void testHover(double& xPos, double& yPos);
	bool testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	void updateStartX(int x);
	void setStartY(int y);
	int getAmount();
private:
	std::vector<GUIElement*> elements;
	void handleHover();
	void handleClicked(int& mouseState);
};

class ScrollbarVertical : public GUIElement {
public:
	ScrollbarVertical(std::string name, int xStart, int yStart, int width, int height, Button* topButton, Button* bottomButton,
		Vertices* backgroundPart,
		Vertices* barTopPart, Vertices* barBottomPart,
		Vertices* barMiddlePart, Vertices* barMiddlePartHover, Vertices* barMiddlePartClicked);
	~ScrollbarVertical();
	void createObject(DrawObjects *ob);
	void createObjectNoRestriction(DrawObjects *ob);
	void testHover(double& xPos, double& yPos);
	bool testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	bool getUpdate();
	void setUpdate(bool b);
	void updateScrollbarLengthAndPosition(int length, int pos);
	void updateScrollbarLengthAndPositionAndIncrementSize(int length, int pos, int end);
	void updateScrollbarPosition(double pos, bool useLambda);
	void setIncrementSize(double size) { incrementSize = size; };
	double getIncrementSize() { return incrementSize; };
	double getScrollbarPosition() { return scrollbarPosition - topButton->getHeight(); };
	double getScrollbarPositionRaw() { return scrollbarPosition; };
	int getScrollbarMaxLength() { return height - (topButton->getHeight() + bottomButton->getHeight()); };
	int getScrollbarIncrementsPoints() { return height - (scrollbarLength + bottomButton->getHeight() + topButton->getHeight()); };
	int isScrollbarAtMaxPosition() { return scrollbarLength + scrollbarPosition >= height - bottomButton->getHeight(); };
	double getChange() { return changeInPosition; };
	void setLambda(std::function<void()> l) { incrementLambda = l; };
	void setTestOnClick(bool b) { testOnClick = b; }; // Used for triggering lambda on a mouseclick and not just on mouse movement.
	void reset() { changeInPosition = 0; };
private:
	std::string name;
	bool ignoreEndPartsBar{ false }, testOnClick{ false };
	int scrollbarLength{ 0 }, mouseYPos{ 0 }, scrollbarLengthMin{ 10 };
	double incrementSize{ 0.0 }, scrollbarPosition{ 0 }, changeInPosition{ 0 };
	Button* topButton, * bottomButton;
	Vertices* backgroundPart,
		* barTopPart, 
		* barBottomPart, 
		* barMiddlePart, * barMiddlePartHover, * barMiddlePartClicked,
		* tempBackground{ nullptr }, * tempBarBody{ nullptr };
	std::function<void()> incrementLambda{ NULL };
};

class TextField: public GUIElement{
public:
	TextField(std::string name, int xStart, int yStart, int width, int height, Vertices *field, Vertices *fieldFocus);
	~TextField();
	void createObject(DrawObjects* ob);
	void handleKeyStroke(int& key, int& action, int& mods);
	bool testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	DrawObject*& getMarker() { return marker; };
	void setTextStartXOffset(int offset) { textStartXOffset = offset; };
	int getTextStartXOffset() { return textStartXOffset; };
	void setTextStartYOffset(int offset) { textStartYOffset = offset; };
	int getTextStartYOffset() { return textStartYOffset; };
	bool getTriggerOnKeyStroke() { return triggerOnKeyStroke; };
	void setTriggerOnKeyStroke(bool b) { triggerOnKeyStroke = b; };
	void setLambda(std::function<void()> l) { lambda = l; };
	void triggerLambda() { if (lambda) lambda(); };
	std::string getText() { return text; };
private:
	int textStartXOffset{ 3 }, textStartYOffset{ 0 }, markerPos{ 0 }, textSize{ 16 }, rowLength{ 0 };
	bool triggerOnKeyStroke{ false }, repeat{ false };
	Vertices *field, *fieldFocus;
	DrawObject* marker{ nullptr };
	std::function<void()> lambda{ NULL };

	void handleClicked(int& mouseState);
};

#endif