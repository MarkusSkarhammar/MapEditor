#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <algorithm>
#include "VertecesHandler.h"
#include "RendToText.h"
#pragma warning(disable: 4244)

class Object {
public:
	Object(float x, float y, size_t id, size_t VAO, int texturePos) : x(x), y(y), id(id), VAO(VAO), texturePos(texturePos) {};
	Object(Object*& o) { *this = *o; };
	virtual ~Object() {};
	size_t getID() { return id; };
	void setID(int value ) { id = value; };
	size_t getVAO() { return VAO; };
	int getTexturePos() { return texturePos; };
	float getXPosition() { return x; };
	float getYPosition() { return y; };
	void setXPosition(int& v) { x = v; };
	void setYPosition(int& v) { y = v; };
	bool& getDraw() { return draw; };
	void setDraw(bool value) { draw = value; };
	void setAnimationState(bool b) { animationState = b; };
	bool& getAnimationState() { return animationState; };
	float getScale() { return scale; }
	void setScale(float value) { scale = value; };
	void setOffsetX(float x) { textOffsetX = x; };
	void setOffsetY(float y) { textOffsetY = y; };
	float getTextOffsetX() { return textOffsetX; };
	float getTextOffsetY() { return textOffsetY; };
	void setRendToText(RendToText* v) { rtt = v; };
	RendToText* getRendToText() { return rtt; };
protected:
	bool animationState{ true };
private:
	bool draw{ true };
	RendToText* rtt;
	float x{ 0.0 }, y{ 0.0 }, scale{ 1.0 }, textOffsetX{ 0 }, textOffsetY{ 0 };
	size_t id{ 0 }, VAO{ 0 };
	int texturePos{ 0 };
};

class AnimationObject : public Object {
public:
	AnimationObject(float x, float y, int id, int VAO, int texturePos, int animationDelay, __int64 timeStamp) : animationDelay(animationDelay), timeStamp(timeStamp), Object(x, y, id, VAO, texturePos) {};
	AnimationObject(AnimationObject* a) : Object(a->getXPosition(), a->getYPosition(), a->getID(), a->getVAO(), a->getTexturePos()) { *this = *a; };
	~AnimationObject() {};
	void checkAnimation();
	void insertAnimationID(int id) { animationIDs.push_back(id); };
	void setEndAnimation(int value) { endAnimation = value; };
private:
	int animationDelay, pos{ 0 }, endAnimation{ 0 };
	__int64 timeStamp{ 0 };
	std::vector<int> animationIDs;
};

class Objects {
public:
	Objects(std::string name) : name(name) {};
	~Objects() { clearObjects(); };
	std::string getName() { return name; };
	void addObject(Object* o);
	void setObjects(std::vector<Object*> input);
	std::vector<Object*>& getObjects() { return objects; };
	std::vector<Object*> getCopyObjects() { return objects; };
	void clearObjects();
	void checkAnimations();
private:
	std::string name;
	std::vector<Object*> objects;
	std::vector<AnimationObject*> aObjects;
};

static Objects& getObjectByName(std::vector<Objects>& ob, std::string name) {
	auto it = std::find_if(ob.begin(), ob.end(), [name](Objects& objects) {
		return (objects.getName() == name);
	});
		return *it;
}

void checkAnimations();
void changeElementSizeAndTexture(Vertices*& v, int xTopLeft, int yTopLeft, int xTopRight, int yTopRight, int xBottomLeft, int yBottomLeft, int xBottomRight, int yBottomRight, int xTopLeftText, int yTopLeftText, int xTopRightText, int yTopRightText, int xBottomLeftText, int yBottomLeftText, int xBottomRightText, int yBottomRightText);
void restoreElement(Vertices*& v);
#endif