#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <algorithm>
#include "Item.h"

class Object {
public:
	Object(float x, float y, size_t id, size_t VAO, size_t texturePos) : x(x), y(y), id(id), VAO(VAO), texturePos(texturePos) {};
	Object(Object*& o) { *this = *o; };
	virtual ~Object() {};
	size_t getID() { return id; };
	void setID(int value ) { id = value; };
	size_t getVAO() { return VAO; };
	size_t getTexturePos() { return texturePos; };
	float getXPosition() { return x; };
	float getYPosition() { return y; };
	bool& getDraw() { return draw; };
	void setDraw(bool value) { draw = value; };
private:
	bool draw{ true };
	float x{ 0.0 }, y{ 0.0 };
	size_t id{ 0 }, VAO{ 0 }, texturePos{0};
};

class AnimationObject : public Object {
public:
	AnimationObject(float x, float y, int id, int VAO, int texturePos, int animationDelay, __int64 timeStamp) : animationDelay(animationDelay), timeStamp(timeStamp), Object(x, y, id, VAO, texturePos) {};
	AnimationObject(AnimationObject* a): Object(a->getXPosition(), a->getYPosition(), a->getID(), a->getVAO(), a->getTexturePos()){ *this = *a; };
	~AnimationObject() {};
	void checkAnimation();
	void insertAnimationID(int id) { animationIDs.push_back(id); };
	void setEndAnimation(int value) { endAnimation = value; };
	void setItem(Item*& i) { item = i; };
private:
	Item * item = nullptr;
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

#endif