#include "Object.h"
#include <chrono>
#include "Global.h"
#include "Item.h"

void AnimationObject::checkAnimation()
{
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (item->getAnimationState() && endAnimation != getID() && now - timeStamp > animationDelay) {
		if (++pos == animationIDs.size())
			pos = 0;
		setID(animationIDs.at(pos));
		timeStamp = now;
	}
}

void Objects::addObject(Object * o)
{
	if (AnimationObject* a = dynamic_cast<AnimationObject*>(o)) {
		aObjects.push_back(a);
	}
		objects.push_back(o);
}

void Objects::setObjects(std::vector<Object*> input)
{
	clearObjects();
	for (auto& o : input) {
		if (AnimationObject* a = dynamic_cast<AnimationObject*>(o)) {
			AnimationObject* aO = new AnimationObject(a);
			aObjects.push_back(aO);
			objects.push_back(aO);
		}
		else
			objects.push_back(new Object(o));
	}
}

void Objects::clearObjects()
{
	for (auto& o : objects) {
		delete o;
	}
	aObjects.clear();
	objects.clear();
}

void Objects::checkAnimations()
{
	for (auto& a : aObjects) {
		a->checkAnimation();
	}
}

void checkAnimations()
{
	for (auto& o : objects) {
		o.checkAnimations();
	}
}
