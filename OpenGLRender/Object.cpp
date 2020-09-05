#include "Object.h"
#include <chrono>
#include "Global.h"
#include "Item.h"

void AnimationObject::checkAnimation()
{
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (animationState && endAnimation != getID() && now - timeStamp > animationDelay) {
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
	int from = 0, to = z;
	if (z > 6) {
		from = 7;
		to = z;
	}
	int section = 0;
	bool skip = false, skipRight = false, skipDown = false;

	double x(0), y(0);
	for (int floorAt = from; floorAt <= to; floorAt++) {
		for (int sections = 0; sections < 4; sections++) {
			skip = false;
			switch (sections)
			{
			case 1:
				if (currentSection % 40 != 39)
					section++;
				else {
					skip = true;
					skipRight = true;
				}
				break;
			case 2:
				if (currentSection < 1560)
					section = currentSection + SECTIONS_WIDTH;
				else {
					skip = true;
					skipDown = true;
				}
				break;
			case 3:
				if (!skipRight && !skipDown)
					section = currentSection + SECTIONS_WIDTH + 1;
				else
					skip = true;
				break;
			default:
				section = currentSection;
				break;
			}
			if (!skip)
				for (auto& t : world.getFloor(floorAt).getSection(section)) {
					auto& object = t->getObject();
					x = 0.0 + (width * object->getXPosition());
					y = 0.0 - (height * object->getYPosition());
					if (x < xCameraPos + 1.0f * zoom && x >= xCameraPos - 1.1f * zoom && y <= yCameraPos + 1.2f * zoom && y > yCameraPos - 1.0f * zoom && object->getDraw()) {
						for (auto& i : t->getAllItems()) {
							if (AnimationObject* check = dynamic_cast<AnimationObject*>(i->getObject())) {
								check->checkAnimation();
							}
						}
					}
				}
		}
	}
	/*
	for (auto& o : objects) {
		o.checkAnimations();
	}
	*/
}

void changeElementSizeAndTexture(Vertices*& v, int xLeftDifference, int yLeftDifference, int xRightDifference, int yRightDifference)
{

	VertecesHandler*& vh = v->getVertecesHandler();
	//glBindVertexArray(vh.getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, vh->getVBO());
	{ // Texture
		auto it = vh->getVertecesText().begin() + (v->getID() * 8);
		double amountX = (*(it + 2) - *(it)) * 0.5;
		// X
		*(it + (1 * 2)) -= amountX;
		// Y
		//textCoords.at(1 * 2 + 1) -= (amount / double(screenHeightPixels / 2));
		// X
		*(it + (3 * 2)) -= amountX;
		// Y
		//textCoords.at(3 * 2 + 1) -= (amount / double(screenHeightPixels / 2));
		std::vector<float> textCoords;
		for (auto it = vh->getVertecesText().begin() + (v->getID() * 8); it != vh->getVertecesText().begin() + (v->getID() * 8) + 8; it++) {
			textCoords.push_back(*it);
		}
		glBufferSubData(GL_ARRAY_BUFFER, (vh->getVerteces().size() + (v->getID() * 8)) * sizeof(float), 8 * sizeof(float), &textCoords[0]);
	}
	{ // Position
		auto it = vh->getVerteces().begin() + (v->getID() * 8);
		double amountX = (*(it + 2) - *(it)) * 0.5;
		// X
		*(it + (1 * 2)) -= amountX;
		// Y
		//textCoords.at(1 * 2 + 1) -= (amount / double(screenHeightPixels / 2));
		// X
		*(it + (3 * 2)) -= amountX;
		// Y
		//textCoords.at(3 * 2 + 1) -= (amount / double(screenHeightPixels / 2));
		std::vector<float> coords;
		for (auto itTemp = it; itTemp != it + 8; itTemp++) {
			coords.push_back(*itTemp);
		}
		glBufferSubData(GL_ARRAY_BUFFER, (v->getID() * 8) * sizeof(float), 8 * sizeof(float), &coords[0]);
	}
}