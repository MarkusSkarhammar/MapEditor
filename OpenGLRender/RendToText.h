#ifndef RENDER_TO_TEXTURE
#define	RENDER_TO_TEXTURE

// Include GLEW
#include <GL/glew.h>
#include "ObjectLibrary.h"
#include "Vertices.h"

#include <iostream>

class RendToText
{
public:
	RendToText(std::string name, double width, double height);
	~RendToText();
	std::string getName() { return name; }
	GLuint& getTextureID() { return text; };
	GLuint& getFboID() { return fbo; };
	double getWidth() { return width; };
	double getHeight() { return height; };
	Vertices* getVerticesByName(std::string name);
private:
	std::string name;
	double width, height;
	GLuint fbo{ 0 }, rbo{ 0 }, text{ 0 };
	std::vector<ObjectLibrary*>& list;
};

class RendToTextObjLibrary {
public:
	RendToTextObjLibrary() {};
	~RendToTextObjLibrary();
	std::vector<ObjectLibrary*>& setupNewLibrary(int width, int height);
	std::vector<ObjectLibrary*> getLibrary(int width, int height);
	std::vector<ObjectLibrary*>& get_Library_Ref(int width, int height);
	ObjectLibrary* get_Specific_Library(int VAO, int VBO);
	LetterLibrary* get_Letter_Library(int width, int height);

	class RendToTextObjLibraryElement {
	public:
		RendToTextObjLibraryElement(int width, int height);
		~RendToTextObjLibraryElement();
		std::vector<ObjectLibrary*> getLibrary() { return library; };
		std::vector<ObjectLibrary*>& get_Library_Ref() { return library; };
		LetterLibrary* get_Letter_Library() { return letterLib; };
		int getWidth() { return width; };
		int getHeight() { return height; };
	private:
		int width{ 0 }, height{ 0 };
		std::vector<ObjectLibrary*> library;
		LetterLibrary* letterLib;
	};

	std::vector<RendToTextObjLibraryElement*>& getLibraries() { return libraries; };

private:
	std::vector<RendToTextObjLibraryElement*> libraries;
};

#endif