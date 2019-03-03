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
	GUIElement(std::string name, int xStart, int yStart, int width, int height): name(name), xStart(xStart), yStart(yStart), width(width), height(height) {};
	GUIElement(std::string name, int vao, int textPos, int id, int xStart, int yStart, int width, int height, std::string text);
	virtual void testHover(double& xPos, double& yPos);
	virtual void testClicked(double& xPos, double& yPos, int mouseState, bool& isClicked);
	virtual void createObject(Objects & ob);
	std::string& getName();
	void setID(int& id);
	int& getID();
	virtual void setUpdate(bool value);
	virtual bool getUpdate();
	bool getHover();
	void setHover(bool b) { isHover = b; };
	bool getClicked();
	void setText(std::string s);
	void setCentered(bool b) { isCenteredText = b; };
	void toggleShow() { show = !show; };
	bool getShow() { return show; };
protected:
	std::string name{ "" }, text{ "" };
	bool isClicked{ false }, isHover{ false }, isCenteredText{ false }, show{ true };
	int VAO{ 0 }, textPos{ 0 }, id{ -1 }, xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 };
private:
	bool update{ false };
	virtual void handleHover();
	virtual void handleClicked(int& mouseState);
};

class Button : public GUIElement {
public:
	Button(std::string name, int vao, int textPos, int id1, int id2, int id3, int xStart, int yStart, int width, int height, std::string text, std::function<void()> const& lambda);
protected:
	int buttonID, hoverID, clickedID;
private:
	std::function<void()> const lambda;
	void handleHover();
	void handleClicked(int& mouseState);
};

class ExpandingButton : public Button {
public:
	ExpandingButton(std::string name, int vao, int textPos, int id1, int id2, int id3, int xStart, int yStart, int width, int height, float scale, std::string text, std::function<void()> const& lambda);
	void createObject(Objects& ob);
private:
	float scaling{ 1.0 };
};

class ToggleButton : public GUIElement {
public:
	ToggleButton(std::string name, int vao, int textPos, int id1, int id2, int id3, int xStart, int yStart, int width, int height, std::function<void()> const& lambda);
	ToggleButton(std::string name, int id1, int id2, int id3, int xStart, int yStart, int width, int height, std::function<void()> const& lambda, bool isTile);
	ToggleButton(std::string name, int id1, int id2, int id3, int xStart, int yStart, int width, int height, std::function<void()> const& lambda, bool isTile, bool doubleSize);
	void createObject(Objects & ob);
	void resetToggle();
	bool& getToggle() { return toggle; };
	void toggleCleanreset() { cleanReset = !cleanReset; };
	void toggleHover() { doHover = !doHover; };
private:
	int buttonID, hoverID, clickedID;
	bool toggle{ false }, isTile{ false }, doubleSize{ false }, cleanReset{ true }, doHover{ true };
	std::function<void()> const lambda;
	void handleHover();
	void handleClicked(int& mouseState);
};

class ToggleButtonGroup : public GUIElement {
public:
	ToggleButtonGroup(std::string name) : GUIElement(name) {};
	~ToggleButtonGroup();
	void createObject(Objects & ob);
	ToggleButton* getElementByName(std::string name);
	void addElement(GUIElement* e);
	bool getUpdate();
	void testHover(double& xPos, double& yPos);
	void testClicked(double& xPos, double& yPos, int mouseState, bool& isClickedAbove);
	void resetAll();
	void clearAll();
private:
	std::vector<ToggleButton*> toggleButtons;
	ToggleButton* currentlyPressed{nullptr};
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
	void checkUpdates(Objects& ob);
	void setCheckIfOutside(bool value) { checkIfOutside = value; };
	void toggleShow() { show = !show; };
	bool getShow() { return show; };
private:
	int xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 };
	bool checkIfOutside{ false }, isClicked{ false }, show{ true };
	std::string name;
	std::vector<GUIElement*> elements;
};

class DropDownElement : public GUIElement {
public:
	DropDownElement(std::string name, int vao, int textPos, int sectionID, int hoverID, int xStart, int yStart, int width, int height, std::function<void()> const& lambda);
	void setCurrentID(int ID);
	virtual void createObject(Objects & ob);
	void runLambda() { lambda(); };
private:
	std::function<void()> const lambda;
	int sectionID, hoverID;
	void handleHover();
	void handleClicked(int& mouseState);
};

class DropDown : public GUIElement{
public:
	DropDown(std::string name, int vao, int textPos, int topSectionID, int middleSectionID, int bottomSectionID, int hoverID, int xStart, int yStart, int widthEntry, int heightEntry);
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
	int vao, textPos, xStart, yStart, width, height, currentSelected{ -1 }, padding { 0 };
	int topSectionID, middleSectionID, bottomSectionID, hoverID;
	void handleHover();
	void handleClicked(int& mouseState);
};

class GUIText : public GUIElement {
public:
	GUIText(std::string name, int xStart, int yStart, int width, int height, std::string text) : GUIElement(name, xStart, yStart, width, height) { setText(text); };
	void createObject(Objects & ob);
private:
};

#endif