#ifndef DRAW_OBJECTS_H
#define DRAW_OBJECTS_H

#include <string>
#include <vector>
#include <algorithm>
#include "Vertices.h"
#include "RendToText.h"
#pragma warning(disable: 4244)

class DrawObject {
public:
	DrawObject(float x, float y, int id, size_t VAO, int texturePos) : x(x), y(y), id(id), VAO(VAO), texturePos(texturePos) {};
	DrawObject(DrawObject*& o) { *this = *o; };
	virtual ~DrawObject() {};
	size_t getID() { return id; };
	void setID(int value ) { id = value; };
	size_t getVAO() { return VAO; };
	void set_VAO(int i) { VAO = i; };
	int getTexturePos() { return texturePos; };
	void set_Text_Pos(int i) { texturePos = i; };
	float getXPosition() { return x; };
	float getYPosition() { return y; };
	void setXPosition(float v) { x = v; };
	void setYPosition(float v) { y = v; };
	bool& getDraw() { return draw; };
	void setDraw(bool value) { draw = value; };
	void setAnimationState(bool b) { animationState = b; };
	bool& getAnimationState() { return animationState; };
	void setOutline(bool v) { outline = v; };
	bool& getOutline() { return outline; };
	double getScale() { return scale; }
	void setScale(double value) { scale = value; };
	double get_Scale_X() { return scaleX; }
	void set_Scale_X(double value) { scaleX = value; };
	double get_Scale_Y() { return scaleY; }
	void set_Scale_Y(double value) { scaleY = value; };
	double get_Zoom() { return zoom; }
	void set_Zoom(double value) { zoom = value; };
	void setOffsetX(double x) { textOffsetX = x; };
	void setOffsetY(double y) { textOffsetY = y; };
	double getTextOffsetX() { return textOffsetX; };
	double getTextOffsetY() { return textOffsetY; };
	void setRendToText(RendToText* v) { rtt = v; };
	RendToText* getRendToText() { return rtt; };
	void renderWorldTile(double& x, double& y);
	virtual void renderGUI(std::string name);
	void renderOutlineTile();
	void renderOutlineItem();
protected:
	bool animationState{ true };
private:
	bool draw{ true }, normalRender{ true }, outline{ false };
	RendToText* rtt{ nullptr };
	double x{ 0.0 }, y{ 0.0 }, scale{ 1.0 }, textOffsetX{ 0 }, textOffsetY{ 0 }, scaleX{ 1.0 }, scaleY{ 1.0 }, zoom{ 1.0 };
	int id{ 0 };
	size_t VAO{ 0 };
	int texturePos{ 0 };
	void rendToText(bool zoomWorld);
	void resetRenderMode();
};

class AnimationObject : public DrawObject {
public:
	AnimationObject(float x, float y, int id, int VAO, int texturePos, int animationDelay, __int64 timeStamp) : animationDelay(animationDelay), timeStamp(timeStamp), DrawObject(x, y, id, VAO, texturePos) {};
	AnimationObject(AnimationObject* a) : DrawObject(a->getXPosition(), a->getYPosition(), a->getID(), a->getVAO(), a->getTexturePos()) { *this = *a; };
	~AnimationObject() {};
	void checkAnimation();
	void insertAnimationID(int id) { animationIDs.push_back(id); };
	void setEndAnimation(int value) { endAnimation = value; };
private:
	int animationDelay, pos{ 0 }, endAnimation{ 0 };
	__int64 timeStamp{ 0 };
	std::vector<int> animationIDs;
};

class TextDrawObject : public DrawObject {
public:
	TextDrawObject(std::string text): text(text), DrawObject(0., 0., 0, 0, 0) {};
	~TextDrawObject();

	void clear();

	std::string get_Text() { return text; };
	void set_Text(std::string s) { text = s; };

	std::vector<DrawObject*>& get_Objects() { return objects; };

	//using DrawObject::renderGUI;
	void renderGUI(std::string name);
private:
	std::string text{ "" };
	std::vector<DrawObject*> objects;
};

class DrawObjects {
public:
	DrawObjects(std::string name) : name(name) {};
	~DrawObjects() { clearObjects(); };
	std::string getName() { return name; };
	void addObject(DrawObject* o);
	void setObjects(std::vector<DrawObject*> input);
	std::vector<DrawObject*>& getObjects() { return objects; };
	std::vector<DrawObject*> getCopyObjects() { return objects; };
	void clearObjects();
	void checkAnimations();
private:
	std::string name;
	std::vector<DrawObject*> objects;
	std::vector<AnimationObject*> aObjects;
};

static DrawObjects* getObjectByName(std::vector<DrawObjects*>& ob, std::string name) {
	auto it = std::find_if(ob.begin(), ob.end(), [name](DrawObjects* objects) {
		return (objects->getName() == name);
	});
		return *it;
}

void checkAnimations();
void changeElementSizeAndTexture(Vertices*& v, int xTopLeft, int yTopLeft, int xTopRight, int yTopRight, int xBottomLeft, int yBottomLeft, int xBottomRight, int yBottomRight, int xTopLeftText, int yTopLeftText, int xTopRightText, int yTopRightText, int xBottomLeftText, int yBottomLeftText, int xBottomRightText, int yBottomRightText);
void restoreElement(Vertices*& v);
#endif