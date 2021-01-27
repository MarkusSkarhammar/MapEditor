#include "ObjectLibrary.h"
#include "../pugixml/pugixml.hpp"
#include "Global.h"

ObjectLibrary::ObjectLibrary(std::string name): name(name)
{
	VBO = GLuint(0); VAO = GLuint(0);
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO); // Generate 1 buffer

	//updateVBO();
}

ObjectLibrary::ObjectLibrary(ObjectLibrary* other, float textureSizeWidth, float textureSizeHeight)
{
	rtt = true;
	name = other->getName();
	VBO = GLuint(0); VAO = GLuint(0);
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO); // Generate 1 buffer

	Vertices *newObj = nullptr, *oldObj = nullptr;
	for (auto o : other->getObjects()) {
		oldObj = o->getVertices();
		newObj = new Vertices(oldObj, textureSizeWidth, textureSizeHeight, vertecesPos, vertecesTextPos, VAO, VBO);
		newObj->setTextPos(oldObj->getTextPos());
		objects.push_back(new Object(o->getID(), o->getName(), newObj, this));
	}

	if(vertecesPos.size() > 0)
		updateVBO();
}

ObjectLibrary::~ObjectLibrary()
{
	for (auto* o : objects) {
		delete o;
	}
	objects.clear();

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ObjectLibrary::updateVBO()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	//glBufferData(GL_ARRAY_BUFFER, temp.getVerteces().size() * sizeof(float), &temp.getVerteces()[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertecesPos.size() * sizeof(float) + vertecesTextPos.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertecesPos.size() * sizeof(float), &vertecesPos[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertecesPos.size() * sizeof(float), vertecesTextPos.size() * sizeof(float), &vertecesTextPos[0]);
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(vertecesPos.size() * sizeof(float)));
}

Object* ObjectLibrary::addObject(int ID, std::string name, Vertices* v)
{
	auto obj = new Object(ID, name, v, this);
	objects.push_back(obj);
	updateVBO();
	return obj;
}

Object* ObjectLibrary::addObject(std::string name, Vertices* v)
{
	auto obj = new Object(objects.size(), name, v, this);
	objects.push_back(obj);
	updateVBO();
	return obj;
}

void ObjectLibrary::delete_Verteces(std::string name)
{
	bool found = false;
	for (auto it = objects.begin(); it != objects.end(); it++) {
		if ((*it)->getName() == name) {
			(*it)->delete_Verteces();
			delete (*it);
			it = objects.erase(it);
			found = true;
			if (it != objects.end())
				(*it)->decrement_ID_And_Verteces_ID();
			else
				break;
		}
		else if (found) {
			(*it)->decrement_ID_And_Verteces_ID();
		}
	}
	updateVBO();
}

Object::Object(int ID, std::string name, Vertices* v, ObjectLibrary* lib): ID(ID), name(name), v(v), lib(lib)
{
}

Object::Object(Object* other)
{
	ID = other->getID();
	name = other->getName();
	v = new Vertices(other->getVertices());
	lib = other->getLib();
}

Object::Object(Object* other, bool tempObject)
{
	ID = other->getID();
	name = other->getName();
	v = new Vertices(other->getVertices(), true);
	lib = other->getLib();
}

Object::~Object()
{
	if(v)
		delete v;
}

void Object::delete_Verteces()
{
	removeVertices(v);
}

void Object::decrement_ID_And_Verteces_ID()
{
	ID--;
	v->decrementID();
}

void Object::set_Update(bool b)
{
	update = b; 
	if (b)
		lib->set_Update(b);
}

bool Object::operator==(const Object* other)
{
	if (ID == other->ID && lib == other->lib 
		&& v->getXStartText() == other->v->getXStartText() 
		&& v->getYStartText() == other->v->getYStartText() 
		&& v->getWidth() == other->v->getWidth() 
		&& v->getHeight() == other->v->getHeight()
		&& v->getTextPos() == v->getTextPos())
		return true;
	return false;
}

bool Object::operator!=(const Object* other)
{
	return !(this==other);
}

void setupObjectLibraries()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("./resources/ObjectLibrary/ObjectLibrary.xml");

	std::string type, ID, name;
	for (pugi::xml_node libraries = doc.child("Libraries").child("Library"); libraries; libraries = libraries.next_sibling("Library"))
	{
		type = libraries.attribute("id").as_string();
		ObjectLibrary* objLib = new ObjectLibrary(type);

		for (pugi::xml_node library = libraries.child("Object"); library; library = library.next_sibling("Object")) {
			ID = library.attribute("id").as_string();
			name = library.attribute("name").as_string();

			int xStartText = 0, yStartText = 0, widthText = 0, heightText = 0, width = 0, height = 0, letterPixelsBefore = 0, letterPixelsSize = 0, letterYDifference = 0;
			bool doubleSize = false;
			std::string textureName;

			for (pugi::xml_node attribute = library.child("attribute"); attribute; attribute = attribute.next_sibling("attribute"))
			{
				if (attribute.attribute("xStartText")) {
					xStartText = attribute.attribute("xStartText").as_int();
				}
				else if (attribute.attribute("yStartText")) {
					yStartText = attribute.attribute("yStartText").as_int();
				}
				else if (attribute.attribute("widthText")) {
					widthText = attribute.attribute("widthText").as_int();
				}
				else if (attribute.attribute("heightText")) {
					heightText = attribute.attribute("heightText").as_int();
				}
				else if (attribute.attribute("width")) {
					width = attribute.attribute("width").as_int();
				}
				else if (attribute.attribute("height")) {
					height = attribute.attribute("height").as_int();
				}
				else if (attribute.attribute("textureName")) {
					textureName = attribute.attribute("textureName").as_string();
				}
				else if (attribute.attribute("letterPixelsBefore")) {
					letterPixelsBefore = attribute.attribute("letterPixelsBefore").as_int();
				}
				else if (attribute.attribute("letterPixelsSize")) {
					letterPixelsSize = attribute.attribute("letterPixelsSize").as_int();
				}
				else if (attribute.attribute("doubleSize")) {
					doubleSize = attribute.attribute("doubleSize").as_int();
				}
				else if (attribute.attribute("letterYDifference")) {
					letterYDifference = attribute.attribute("letterYDifference").as_int();
				}
			}

			auto obj = objLib->addObject(std::stoi(ID), name, new Vertices(name, xStartText, yStartText, widthText, heightText, width, height, objLib->getVertecesPos(), objLib->getVertecesPosText(), findByName(paths, textureName), objLib->getVAO(), objLib->getVBO(), objLib->getObjects().size(), doubleSize));
			if (type == "Letters") {
				letterLibrary.insertLetter(obj->getName(), objLib->getVAO(), obj->getVertices()->getTextPos(), obj->getVertices()->getID(), letterPixelsBefore, letterPixelsSize, width, letterYDifference);
			}
		}
		objLibraries.push_back(objLib);
	}

}

Object* getObjectFromLibrary(std::string category, int ID)
{
	typename std::vector<ObjectLibrary*>::iterator it = std::find_if(objLibraries.begin(), objLibraries.end(), [&category](ObjectLibrary* element) {
		return (element->getName() == category);
		});
	if (it != objLibraries.end()) {
		auto& list = (*it)->getObjects();
		typename std::vector<Object*>::iterator it2 = std::find_if(list.begin(), list.end(), [&ID](Object* element) {
			return (element->getID() == ID);
			});
		if (it2 != list.end()) {
			return *it2;
		}
		else
			return nullptr;
	}
	else
		return nullptr;
}

Object* getObjectFromLibrary(std::vector<ObjectLibrary*> list, std::string category, int ID)
{

	typename std::vector<ObjectLibrary*>::iterator it = std::find_if(list.begin(), list.end(), [&category](ObjectLibrary* element) {
		return (element->getName() == category);
		});
	if (it != list.end()) {
		auto& list = (*it)->getObjects();
		typename std::vector<Object*>::iterator it2 = std::find_if(list.begin(), list.end(), [&ID](Object* element) {
			return (element->getID() == ID);
			});
		if (it2 != list.end()) {
			return *it2;
		}
		else
			return nullptr;
	}
	else
		return nullptr;
}

Object* getObjectFromLibrary(std::string NAME)
{
	for (auto l : objLibraries) {
		for (auto o : l->getObjects()) {
			if (o->getName() == NAME)
				return o;
		}
	}
	return nullptr;
}

Object* getObjectFromLibrary(std::vector<Object*> list, std::string NAME)
{
	auto it = std::find_if(list.begin(), list.end(), [&NAME](Object* element) {
		return (element->getName() == NAME);
		});
	if (it != list.end())
		return *it;
	else
		return nullptr;
}

Object* getGUIObjectFromLibrary(std::string name)
{
	for (auto& objLib : objLibraries) {
		if (objLib->getName().substr(0, 3) == "GUI") {
			for (auto& obj : objLib->getObjects()) {
				if (obj->getName() == name)
					return obj;
			}
		}
	}
	return nullptr;
}

void update_All_Libraries(std::vector<ObjectLibrary*> updatedList)
{
	for (auto& lib : updatedList) {
		std::vector<Object*> objectsToUpdate;
		if (lib->get_Update()) {
			for (auto& obj : lib->getObjects()) {
				if (obj->get_Update()) {
					obj->set_Update(false);
					objectsToUpdate.push_back(obj);
				}
			}
			lib->set_Update(false);
			for (auto& libRTTElement : rendToTextLibrary.getLibraries()) {
				for (auto& libRTT : libRTTElement->get_Library_Ref()) {
					if (libRTT->getName() == lib->getName()) {
						for (auto& obj : objectsToUpdate) {
							auto search = getObjectFromLibrary(libRTT->getObjects(), obj->getName());
							if (search) {
								auto objV = obj->getVertices();
								search->getVertices()->setTextPos(objV->getTextPos());
								search->getVertices()->set_All(objV->getXStartText(), objV->getYStartText(), objV->getWidthText(), objV->getHeightText());
							}
						}
					}
				}
			}
		}
	}
}

int LetterLibrary::getWidthInPixels(std::string s, std::string textType)
{
	std::string string;
	std::vector<int> letter;
	int size = 0;
	for (auto& c : s) {
		letter = lettersMap[string = c + textType];
		if(letter.size() > 0)
		size += letter[4] - letter[3] + 1;
	}
	return size;
}
