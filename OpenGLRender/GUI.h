#ifndef GUI_H
#define	GUI_H

#include <string>
#include <vector>
#include "Object.h"
#include <functional>

class GUIElement {
public:
	GUIElement(std::string name);
	GUIElement(std::string name, int xStart, int yStart, int width, int height): name(name), xStart(xStart), yStart(yStart), width(width), height(height) {};
	GUIElement(std::string name, int vao, int textPos, int id, int xStart, int yStart, int width, int height, std::string text);
	virtual void testHover(double& xPos, double& yPos);
	virtual void testClicked(double& xPos, double& yPos, int mouseState);
	virtual void createObject(Objects & ob);
	std::string& getName();
	void setID(int& id);
	int& getID();
	void setUpdate(bool value);
	virtual bool getUpdate();
	bool getHover();
	bool getClicked();
	void setText(std::string s);
protected:
	bool isClicked{ false };
	int VAO{ 0 }, textPos{ 0 }, id{ 0 }, xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 };
private:
	bool isHover{ false }, update{ false };
	std::string name{ "" }, text{ "" };
	virtual void handleHover();
	virtual void handleClicked(int& mouseState);
};

class Button : public GUIElement {
public:
	Button(std::string name, int vao, int textPos, int id1, int id2, int id3, int xStart, int yStart, int width, int height, std::string text, std::function<void()> const& lambda);
private:
	int buttonID, hoverID, clickedID;
	std::function<void()> const lambda;
	void handleHover();
	void handleClicked(int& mouseState);
};

class ToggleButton : public GUIElement {
public:
	ToggleButton(std::string name, int vao, int textPos, int id1, int id2, int id3, int xStart, int yStart, int width, int height, std::string text, std::function<void()> const& lambda);
	void resetToggle();
	bool& getToggle() { return toggle; };
private:
	int buttonID, hoverID, clickedID;
	bool toggle{ false };
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
	void testClicked(double& xPos, double& yPos, int mouseState);
	void resetAll();
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
	void setDimensions(int x, int y, int width, int height);
	void checkHover(Objects& ob, double& xPos, double& yPos);
	void checkClicked(Objects& ob, double& xPos, double& yPos, int mouseState);
	void checkUpdates(Objects& ob);
	void setCheckIfOutside(bool value) { checkIfOutside = value; };
private:
	int xStart{ 0 }, yStart{ 0 }, width{ 0 }, height{ 0 };
	bool checkIfOutside{ false };
	std::string name;
	std::vector<GUIElement*> elements;
};

#endif