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
	GUIElement(std::string name, int xStart, int yStart, int width, int height) : name(name), xStart(xStart), yStart(yStart), width(width), height(height) { };
	GUIElement(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string text);
	virtual ~GUIElement() { if (dObj) delete dObj; if (dObjText) delete dObjText; };
	virtual bool testHover(double& xPos, double& yPos);
	virtual bool testClicked(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClicked);
	virtual void createObject();
	virtual void createObjectNoRestriction();
	virtual void get_Draw_Object(DrawObjects*& objs, bool skipIfRTT = true) {
		if (!dObj)
			if (!rendToText)
				createObject();
			else
				createObjectNoRestriction();
		if(dObj) 
			if (skipIfRTT && !rendToText)
				objs->addObject(dObj);
			else if (!skipIfRTT)
				objs->addObject(dObj);
	};
	DrawObject*& get_Draw_Object();
	virtual void get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT = true) {
		if (!dObjText)
			if (!rendToText)
				createObject();
			else
				createObjectNoRestriction();
		if(dObjText) 
			if(skipIfRTT && !rendToText)
				objs->addObject(dObjText); 
			else if(!skipIfRTT)
				objs->addObject(dObjText);
	};
	virtual TextDrawObject*& get_Draw_Object_Text();
	virtual void checkAnimation();
	virtual void testScroll(double& scroll);
	virtual void handleKeyStroke(int& key, int& action, int& mods);
	std::string& getName();
	Vertices*& getV() { return v; };
	void setV(Vertices* p) { v = p; };
	virtual void setUpdate(bool value);
	virtual bool getUpdate();
	virtual void handle_Update();
	bool getHover();
	void setHover(bool b) { isHover = b; };
	bool getClicked();
	void toggleClicked();
	void set_Clicked(bool b) { isClicked = b; };
	void setText(std::string s);
	void setCenteredText(int b) { isCenteredText = b; };
	void toggleShow() { show = !show; };
	bool getShow() { return show; };
	void set_Skip_Hover(bool b) { skipHover = b; };
	bool get_Skip_Hover() { return skipHover; };
	void setStartX(int x) { xStart = x; };
	virtual void setStartY(int y) { yStart = y; };
	void setOffsetX(int v) { xOffset = v; };
	int get_OffsetX() { return xOffset; };
	void setOffsetY(int v) { yOffset = v; };
	int get_OffsetY() { return yOffset; };
	virtual void setRendToTextOffsetX(int v) { xRendToTextOffset = v; };
	virtual void setRendToTextOffsetY(int v) { yRendToTextOffset = v; };
	int getRendToTextOffsetX() { return xRendToTextOffset; };
	int getRendToTextOffsetY() { return yRendToTextOffset; };
	void setRenderToText() { rendToText = true; };
	void setRenderToText(RendToText* v) { rtt = v; };
	void setRenderToText(RendToText* v, bool b) { rtt = v; rendToText = b; };
	RendToText* getRenderToText() { return rtt; };
	bool get_Render_To_Text_Bool() { return rendToText; };
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
	int get_XDrawOffset() { return xDrawOffset; };
	void setYDrawOffset(int v) { yDrawOffset = v; };
	int get_YDrawOffset() { return yDrawOffset; };
	void setScale(double v) { scale = v; };
	double getScale() { return scale; };
	float get_Scale_X() { return scaleX; }
	void set_Scale_X(float value) { scaleX = value; };
	float get_Scale_Y() { return scaleY; }
	void set_Scale_Y(float value) { scaleY = value; };
	void setScrollLambda(std::function<void(double& v)> lambda) { scrollLambda = lambda; };
	void setHoverLambda(std::function<void(double& xPos, double& yPos)> lambda) { hoverLambda = lambda; };
	void setClickLambda(std::function<void(int& mouseButton, int& mouseState)> lambda) { clickLambda = lambda; };
	void setAnimationLambda(std::function<void(bool& update)> lambda) { animation = lambda; doAnimation = true; };
	void set_Mouse_Enter_Exit_Lamda(std::function<void(bool& state)> lambda) { mouseEnterExitLambda = lambda; };
	void set_Key_Stroke_Lambda(std::function<void(int& key, int& action, int& mods)> lambda) { keyStrokeLambda = lambda; };
	void addMiscellaneousID(std::string note, int v) { miscellaneousIDs.push_back(std::pair<std::string, int>(note, v)); };
	void addMiscellaneousID(std::string note, int v, int at) {
		if (miscellaneousIDs.size() <= at) miscellaneousIDs.push_back(std::pair<std::string, int>(note, v));
		else
			miscellaneousIDs.at(at) = std::pair<std::string, int>(note, v);
	};
	int getMiscellaneousID(int pos) { return miscellaneousIDs.at(pos).second; };
	std::vector<std::pair<std::string, int>>& getMiscellaneousIDsList() { return miscellaneousIDs; };
	virtual std::vector<GUIElement*> getElements() { return std::vector<GUIElement*>(); };
	bool getFocus() { return focus; };
	void setFocus(bool b) { focus = b; };
	void setAnimationBool(bool b) { doAnimation = b; };
	bool getAnimationBool() { return doAnimation; };
	void setTimeStamp(__int64 time) { timeStamp = time; };
	__int64 getTimeStamp() { return timeStamp; };
	int getTextOffset() { return textOffset; };
	void setTextOffset(int v) { textOffset = v; };
	std::string getText() { return text; };
	virtual void change_Position(int xStart, int yStart);
	void set_Ellipsis(bool b) { ellipsis = b; };
	bool get_Ellipsis() { return ellipsis; };
	void set_Text_Type(std::string str) { textType = str; };
	std::string get_Text_Type() { return textType; };
protected:
	std::string name{ "" }, text{ "" }, textType{ "Black" };
	DrawObject* dObj = nullptr;
	TextDrawObject* dObjText = nullptr;
	RendToText* rtt = nullptr;
	bool isClicked{ false }, isHover{ false }, ellipsis{ false }, show{ true }, rendToText{ false }, checkOutsideOfArea{ false }, update{ false }, focus{ false }, doAnimation{ false },
		withinArea{ false }, skipHover{ false };
	int xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 }, xOffset{ 0 }, yOffset{ 0 }, xRendToTextOffset{ 0 }, yRendToTextOffset{ 0 }, xDrawOffset{ 0 }, yDrawOffset{ 0 }, textOffset{ 0 }, isCenteredText{ false };
	double textureWidth{ 2048.f }, textureHeight{ 2048.f }, scale{ 1.0 }, scaleX{ 1.0 }, scaleY{ 1.0 };
	__int64 timeStamp{ 0 };
	std::vector<std::pair<std::string, int>> miscellaneousIDs = {};
	Vertices* v = nullptr;
	std::function<void(double& v)> scrollLambda{ NULL };
	std::function<void(double& xPos, double& yPos)> hoverLambda{ NULL };
	std::function<void(int& mouseButton, int& mouseState)> clickLambda{ NULL };
	std::function<void(bool& update)> animation{ NULL };
	std::function<void(bool& state)> mouseEnterExitLambda{ NULL };
	std::function<void(int& key, int& action, int& mods)> keyStrokeLambda{ NULL };
private:
	virtual void handleHover(double& xPosRef, double& yPosRef);
	virtual void handleClicked(int& mouseButton, int& mouseState);
	virtual void handleAnimation(bool& update);
};

class ScaleableElement : public GUIElement {
public:
	ScaleableElement(std::string name, int xStart, int yStart, int width, int height, Vertices* vertices);
	~ScaleableElement();
	void createObject();
	void createObjectNoRestriction();
private:
	void handleClicked(int& mouseButton, int& mouseState);
};

class Button : public GUIElement {
public:
	Button(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::string text, std::function<void(int& mouseButton, int& mouseState)> lambda);
	//void createObject(DrawObjects* ob);
	//void createObjectNoRestriction(DrawObjects* ob);
	bool testClicked(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClicked);
protected:
	Vertices *button{ nullptr }, *hover{ nullptr }, *clicked{ nullptr };
private:
	void handleHover(double& xPosRef, double& yPosRef);
	void handleClicked(int& mouseButton, int& mouseState);
};

class ExpandingButton : public Button {
public:
	ExpandingButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, float scale, std::string text, std::function<void(int& mousebutton, int& mouseState)> lambda);
	void createObject(DrawObjects* ob);
private:
	float scaling{ 1.0 };
};

class ToggleButton : public GUIElement {
public:
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(int& mouseButton, int& mouseState)> lambda);
	ToggleButton(std::string name, Vertices* button, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(int& mouseButton, int& mouseState)> lambda, std::function<void(double& x, double& y)> hoverLambda);
	virtual ~ToggleButton();
	void createObject();
	void createObjectNoRestriction();
	void get_Draw_Object(DrawObjects*& objs, bool skipIfRTT = true);
	void get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT = true) {
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
	TextDrawObject*& get_Draw_Object_Text() { return dObjText; };
	void resetToggle();
	bool& getToggle() { return toggle; };
	void setToggle(bool b) { toggle = b; };
	void doToggle(int mouseButton, int mouseState);
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
	void set_Clicked(bool b) { isClicked = b; };
	void set_Clicked_Vertices(Vertices* v) { clicked = v; };
	void setClickedOrder(int order) { if(clickedOrder == 0) clickedOrder = order; };
	int& getClickedOrder() { return clickedOrder; };
private:
	Vertices *button, *hover, *clicked;
	DrawObject* base = nullptr;
	bool toggle{ false }, cleanReset{ true }, doHover{ true }, alwaysShowBase{ false };
	int offset{ 0 }, clickedOrder{ 0 };
	void handleHover(double& xPosRef, double& yPosRef);
	void handleClicked(int& mouseButton, int& mouseState);
};

class ToggleButtonGroup : public GUIElement {
public:
	ToggleButtonGroup(std::string name) : GUIElement(name) {};
	ToggleButtonGroup(std::string name, int xStart, int yStart, int width, int height) : GUIElement(name, xStart, yStart, width, height) {};
	~ToggleButtonGroup();
	void createObject();
	void createObjectNoRestriction();
	void get_Draw_Object(DrawObjects*& objs, bool skipIfRTT = true);
	void get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT = true);
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
	bool testHover(double& xPos, double& yPos);
	bool testClicked(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClickedAbove);
	void resetAll();
	void resetHover();
	void clearAll();
	void setUpdate(bool b);
	void setCheckOutside() { checkOutside = true; };
	void setResetAfterToggle(bool v) { resetAfterToggle = v; for (auto b : toggleButtons) { if((resetAfterToggle && !b->getCleanReset()) || (!resetAfterToggle && b->getCleanReset())) b->toggleCleanReset(); } };
	bool getResetAfterToggle() { return resetAfterToggle; }
	void testClickedAt(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClickedAbove);
	void setClickedOrder(ToggleButton* tb);
	void setClickedOrder(int order) { clickedOrder = order; };
	int& getClickedOrder() { return clickedOrder; };
	void resetClickedOrder();
	ToggleButton* getCurrentlyPressed() { return currentlyPressed; };
private:
	std::vector<ToggleButton*> toggleButtons;
	ToggleButton* currentlyPressed{nullptr};
	bool checkOutside{ false }, resetAfterToggle{ false };
	void handleHover();
	void handleClicked(int& mouseButton, int& mouseState);
	int clickedOrder{ 0 };
};

class GUIPanel {
public:
	GUIPanel(std::string name);
	GUIPanel(std::string name, DrawObjects* ob, int x, int y, int width, int height);
	~GUIPanel();
	void setObjects(DrawObjects *ob) { this->ob = ob; };
	DrawObjects* get_Draw_Objects() { return ob; };
	void clear();
	void addElement(GUIElement *e);
	GUIElement* getElementByName(std::string name);
	std::vector<GUIElement*> getEveryElement();
	void removeElementByName(std::string name);
	void setDimensions(int x, int y, int width, int height);
	void checkHover(double& xPos, double& yPos);
	void checkClicked(double& xPos, double& yPos, int mouseButton, int mouseState);
	void checkScroll(double& xPos, double& yPos, double& scroll);
	void checkKeyStroke(int& key, int& action, int& mods);
	void checkUpdates();
	void setCheckIfOutside(bool value) { checkIfOutside = value; };
	void toggleShow() { show = !show; };
	bool getShow() { return show; };
	bool containsLabel();
	void create_Draw_Objects();
	std::string get_Name() { return name; };
private:
	int xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 };
	bool checkIfOutside{ false }, isClicked{ false }, show{ false }, testedLabel{ false }, isLabel{ false };
	DrawObjects *ob{ nullptr };
	std::string name;
	std::vector<GUIElement*> elements;
};

class DropDownElement : public GUIElement {
public:
	DropDownElement(std::string name, Vertices* section, Vertices* hover, Vertices* clicked, int xStart, int yStart, int width, int height, std::function<void(double& xPos, double& yPos)> const& hoverLambda, std::function<void(int& mouseButton, int& mouseState)> const& clickLambda);
	~DropDownElement();
	virtual void createObject();
	virtual void createObjectNoRestriction();
	void get_Draw_Object(DrawObjects*& objs, bool skipIfRTT = true);
	DrawObject*& get_Draw_Object() { return dObj; };
	DrawObject*& get_Draw_Object_Static_Base() { return staticBaseSection; };
	void runLambda(int& mouseButton, int& mouseState) { clickLambda(mouseButton, mouseState); };
	bool is_Hover_Lambda() { if (hoverLambda != NULL) return true; else return false; };
	void run_Hover_Lambda(double& xPos, double& yPos) { hoverLambda(xPos, yPos); };
	void set_Section_Vertices(Vertices* v) { section = v; };
	Vertices* get_Section_Vertices() { return section; };
	void set_Hover_Vertices(Vertices* v) { hover = v; };
	Vertices* get_Hover_Vertices() { return hover; };
	void set_Clicked_Vertices(Vertices* v) { clicked = v; };
	Vertices* get_Clicked_Vertices() { return clicked; };
	bool get_Static_Section() { return staticSection; };
	void set_Static_Section(bool b) { staticSection = b; };
	void set_Size_Based_On_Text();
private:
	DrawObject* staticBaseSection{ nullptr };
	Vertices *section, *hover, *clicked;
	bool staticSection{ false };
	void handleHover(double& xPosRef, double& yPosRef);
	void handleClicked(int& mouseButton, int& mouseState);
};

class DropDown : public GUIElement {
public:
	DropDown(std::string name, Vertices* topSection, Vertices* topSectionHover, Vertices* topSectionClicked,
		Vertices* middleSection, Vertices* middleSectionHover, Vertices* middleSectionClicked,
		Vertices* bottomSection, Vertices* bottomSectionHover, Vertices* bottomSectionClicked,
		int xStart, int yStart, int width, int height);
	~DropDown();
	void add(std::string text, std::function<void(double& xPos, double& yPos)> const& hoverLambda, std::function<void(int& mouseButton, int& mouseState)> const& clickLambda, bool staticSection = false);
	void createObject();
	void createObjectNoRestriction();
	void get_Draw_Object(DrawObjects*& objs, bool skipIfRTT = true);
	void get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT = true);
	bool testHover(double& xPos, double& yPos);
	bool testClicked(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClickedAbove);
	void setPadding(int i) { padding = i; };
	bool getUpdate();
	void setUpdate(bool value);
	bool getShowDropDown() { return showOthers; };
	void toggleShowDropDown();
	ToggleButton* getButton() { return tb; };
	std::vector<DropDownElement*> get_Drop_Down_Elements() { return items; };
	void set_Child_X_Offset(int v) { xChildOffset = v; };
	int get_Child_X_Offset() { return xChildOffset; };
	void change_Position(int xStart, int yStart);
	int get_Highest_Child_Y_Position();
	void set_Only_One_Clickable_At_A_Time(bool b) { onlyOneClickable = b; };
	bool get_Only_One_Clickable_At_A_Time() { return onlyOneClickable; };
	void reset_Drop_Down();
	void set_Child_Text_Offset_X(int offset) { childTextOffsetX = offset; };
	int get_Child_Text_Offset_X() { return childTextOffsetX; };
	void set_Close_After_Select(bool b) { closeAfterSelect = b; }
	bool get_Close_After_Select() { return closeAfterSelect; };
	void set_Size_Based_On_Text(bool b) { baseSizeOnText = b; };
	bool get_Size_Based_On_Text() { return baseSizeOnText; };
private:
	std::vector<DropDownElement*> items;
	int currentSelected{ -1 }, padding{ 0 }, xChildOffset{ 0 }, highestY{ 0 }, childTextOffsetX{ 0 }, childWidth{ 0 };
	Vertices* topSection, * topSectionHover, * topSectionClicked = nullptr,
		* middleSection, * middleSectionHover, * middleSectionClicked = nullptr,
		* bottomSection, * bottomSectionHover, * bottomSectionClicked;
	ToggleButton* tb{ nullptr };
	DropDownElement* lastHover{ nullptr }, *lastClicked{ nullptr };
	bool showOthers{ false }, fromTopDown{ true }, onlyOneClickable{ true }, closeAfterSelect{ true }, baseSizeOnText{ false };
	void handleHover(double& xPosRef, double& yPosRef);
	void handleClicked(int& mouseButton, int& mouseState);
	void handleDraw(DropDownElement* dde);
};

class GUIText : public GUIElement {
public:
	GUIText(std::string name, int xStart, int yStart, int width, int height, std::string text) : GUIElement(name, xStart, yStart, width, height) { setText(text); };
	void createObject(DrawObjects & ob);
private:
};

class GUILabel : public GUIElement {
public:
	GUILabel(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string& textRef) : textRef(textRef), GUIElement(name, v, xStart, yStart, width, height, "") 
	{
		setAnimationLambda([this](bool& update) {
			if (this->dObjText && this->dObjText->get_Text() != this->textRef)
				this->createObject();
			});
	};
	void createObject();
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
	void createObject();
	void createObjectNoRestriction();
	void get_Draw_Object(DrawObjects*& objs, bool skipIfRTT = true);
	void get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT = true);
	void addElement(GUIElement* e);
	GUIElement* getElementByName(std::string name);
	void removeElementByName(std::string name);
	std::vector<GUIElement*> getElements() { return elements; };
	bool testHover(double& xPos, double& yPos);
	bool testClicked(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClickedAbove);
	void updateStartX(int x);
	void setStartY(int y);
	int getAmount();
	bool getUpdate();
	void setUpdate(bool value);
	void checkAnimation();
private:
	std::vector<GUIElement*> elements;
	void handleHover(double& xPosRef, double& yPosRef);
	void handleClicked(int& mouseButton, int& mouseState);
};

class ScrollbarVertical : public GUIElement {
public:
	ScrollbarVertical(std::string name, int xStart, int yStart, int width, int height, Button* topButton, Button* bottomButton,
		Vertices* backgroundPart,
		Vertices* barTopPart, Vertices* barBottomPart,
		Vertices* barMiddlePart, Vertices* barMiddlePartHover, Vertices* barMiddlePartClicked);
	~ScrollbarVertical();
	void createObject();
	void createObjectNoRestriction();
	void get_Draw_Object(DrawObjects*& objs, bool skipIfRTT = true);
	void get_Draw_Object_Text(DrawObjects*& objs, bool skipIfRTT = true);
	bool testHover(double& xPos, double& yPos);
	bool testClicked(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClickedAbove);
	bool getUpdate();
	void setUpdate(bool b);
	void updateScrollbarLengthAndPosition(int length, int pos);
	void updateScrollbarLengthAndPositionAndIncrementSize(int length, int pos, int end);
	void updateScrollbarPosition(double pos, bool useLambda);
	void change_Scrollbar_Position(int pos, bool useLambda);
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
	DrawObject *background{ nullptr }, *barTop{ nullptr }, *barMiddle{ nullptr }, *barBottom{ nullptr };
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
	void createObject();
	void handleKeyStroke(int& key, int& action, int& mods);
	bool testClicked(double& xPos, double& yPos, int mouseButton, int mouseState, bool& isClickedAbove);
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
	void change_Width(int width);
	void change_Height(int height);
	void change_Size(int width, int height);
private:
	int textStartXOffset{ 3 }, textStartYOffset{ 0 }, markerPos{ 0 }, textSize{ 16 }, rowLength{ 0 };
	bool triggerOnKeyStroke{ false }, repeat{ false };
	Vertices *field, *fieldFocus;
	DrawObject* marker{ nullptr };
	std::function<void()> lambda{ NULL };

	void handleClicked(int& mouseButton, int& mouseState);
};

class TextArea: public GUIElement {
public:
	TextArea(std::string name, Vertices* v, int xStart, int yStart, int width, int height, std::string text);
	void createObject();
private:
};

#endif