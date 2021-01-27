#ifndef OBJECT_LIBRARY_H
#define OBJECT_LIBRARY_H

#include <string>
#include <vector>
#include "Vertices.h"
#include <unordered_map>

class ObjectLibrary;

class Object {
public:
	Object(int ID, std::string name, Vertices* v, ObjectLibrary* lib);
	Object(Object* other);
	Object(Object* other, bool tempObject);
	~Object();
	std::string getName() { return name; };
	int getID() { return ID; };
	Vertices* getVertices() { return v; };
	ObjectLibrary* getLib() { return lib; };
	void delete_Verteces();
	void decrement_ID_And_Verteces_ID();
	bool get_Update() { return update; };
	void set_Update(bool b);
	bool operator==(const Object* other);
	bool operator!=(const Object* other);
private:
	int ID{ -1 };
	std::string name{ "" };
	bool update{ false };
	Vertices* v;
	ObjectLibrary* lib;
};

class ObjectLibrary {
public:
	ObjectLibrary(std::string name);
	ObjectLibrary(ObjectLibrary* other, float textureSizeWidth, float textureSizeHeight);
	~ObjectLibrary();
	void updateVBO();
	std::string getName() { return name; };
	std::vector<Object*>& getObjects() { return objects; };
	Object* addObject(int ID, std::string name, Vertices* v);
	Object* addObject(std::string name, Vertices* v);
	std::vector<float>& getVertecesPos() { return vertecesPos; };
	std::vector<float>& getVertecesPosText() { return vertecesTextPos; };
	GLuint getVAO() { return VAO; };
	GLuint getVBO() { return VBO; };
	void delete_Verteces(std::string name);
	bool get_RTT() { return rtt; };
	bool get_Update() { return update; };
	void set_Update(bool b) { update = b; };
private:
	std::string name{ "" };
	bool rtt{ 0 }, update{ false };
	GLuint VBO{ 0 }, VAO{ 0 };
	std::vector<float> vertecesPos, vertecesTextPos;
	std::vector<Object*> objects;

};


class LetterLibrary {
public:
	LetterLibrary(std::string name) : name(name) {};
	std::string getName() { return name; };
	void insertLetter(std::string name, int VAO, int textPos, int pos, int letterPixelsBefore, int letterPixelsSize, int letterWidth, int letterYDifference){
		lettersMap[name] = { VAO, textPos, pos, letterPixelsBefore, letterPixelsSize, letterWidth, letterYDifference};
	};
	std::vector<int>& getLetterInformation(std::string name) {
		return lettersMap[name];
	}
	int getWidthInPixels(std::string s, std::string textType);
	std::unordered_map<std::string, std::vector<int>> get_Map() { return lettersMap; };
private:
	std::string name{ "" };
	std::unordered_map<std::string, std::vector<int>> lettersMap; // [0] = name, [1] = VAO, [2] = pos(in objectLibrary), [3] = empty pixels before first pixel in the texture, [4] = pixels from start to farthest pixel in the texture, [5] = letter width, [6] = y direction change in pixels
};

void setupObjectLibraries();
Object* getObjectFromLibrary(std::string category, int ID);
Object* getObjectFromLibrary(std::vector<ObjectLibrary*> list, std::string category, int ID);
Object* getObjectFromLibrary(std::string NAME);
Object* getObjectFromLibrary(std::vector<Object*> list, std::string NAME);
Object* getGUIObjectFromLibrary(std::string name);


void update_All_Libraries(std::vector<ObjectLibrary*> updatedList);

#endif