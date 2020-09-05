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
	~Object();
	std::string getName() { return name; };
	int getID() { return ID; };
	Vertices* getVertices() { return v; };
	ObjectLibrary* getLib() { return lib; };
private:
	int ID{ -1 };
	std::string name{ "" };
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
	std::vector<float>& getVertecesPos() { return vertecesPos; };
	std::vector<float>& getVertecesPosText() { return vertecesTextPos; };
	GLuint getVAO() { return VAO; };
	GLuint getVBO() { return VBO; };
private:
	std::string name{ "" };
	GLuint VBO{ 0 }, VAO{ 0 };
	std::vector<float> vertecesPos, vertecesTextPos;
	std::vector<Object*> objects;
};


class LetterLibrary {
public:
	LetterLibrary(std::string name) : name(name) {};
	std::string getName() { return name; };
	void insertLetter(std::string name, int VAO, int textPos, int pos, int letterPixelsBefore, int letterPixelsSize, int letterWidth){
		lettersMap[name] = { VAO, textPos, pos, letterPixelsBefore, letterPixelsSize, letterWidth };
	};
	std::vector<int>& getLetterInformation(std::string name) {
		return lettersMap[name];
	}
	int getWidthInPixels(std::string s, std::string textType);
private:
	std::string name{ "" };
	std::unordered_map<std::string, std::vector<int>> lettersMap; // [0] = name, [1] = VAO, [2] = pos(in objectLibrary), [3] = empty pixels before first pixel in the texture, [4] = pixels from start to farthest pixel in the texture, [5] = letter width
};

void setupObjectLibraries();
Object* getObjectFromLibrary(std::string category, int ID);
Object* getObjectFromLibrary(std::string NAME);
Object* getGUIObjectFromLibrary(std::string name);

#endif