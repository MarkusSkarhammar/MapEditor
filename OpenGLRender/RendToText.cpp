#include "RendToText.h"
#include "Global.h"

RendToText::RendToText(std::string name, double width, double height) : name(name), width(width), height(height), list(rendToTextLibrary.get_Library_Ref(width, height))
{

	// Framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Renderbuffer object setup
	{
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	// Render to text texture setup
	{
		glGenTextures(1, &text);
		glBindTexture(GL_TEXTURE_2D, text);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, text, 0);
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RendToText::~RendToText()
{
	glDeleteTextures(1, &text);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
}

Vertices* RendToText::getVerticesByName(std::string name)
{
	for (auto l : list) {
		for (auto o : l->getObjects()) {
			if (o->getName() == name)
				return o->getVertices();
		}
	}
	return nullptr;
}

RendToTextObjLibrary::~RendToTextObjLibrary()
{
	for (auto& lib : libraries) {
			delete lib;
	}
}

std::vector<ObjectLibrary*>& RendToTextObjLibrary::setupNewLibrary(int width, int height)
{
	if (width == screenWidth && height == screenHeight) {
		return objLibraries;
	}
	auto element = new RendToTextObjLibraryElement(width, height);
	libraries.push_back(element);
	return element->get_Library_Ref();
}

std::vector<ObjectLibrary*> RendToTextObjLibrary::getLibrary(int width, int height)
{
	std::vector<RendToTextObjLibraryElement*>::iterator it = std::find_if(libraries.begin(), libraries.end(), [width, height](RendToTextObjLibraryElement* lib) {
		return (lib->getWidth() == width && lib->getHeight() == height);
		});
	if (it != libraries.end()) {
		return (*it)->getLibrary();
	}
	else
		return setupNewLibrary(width, height);
}

std::vector<ObjectLibrary*>& RendToTextObjLibrary::get_Library_Ref(int width, int height)
{
	std::vector<RendToTextObjLibraryElement*>::iterator it = std::find_if(libraries.begin(), libraries.end(), [width, height](RendToTextObjLibraryElement* lib) {
		return (lib->getWidth() == width && lib->getHeight() == height);
		});
	if (it != libraries.end()) {
		return (*it)->get_Library_Ref();
	}
	else
		return setupNewLibrary(width, height);
}

ObjectLibrary* RendToTextObjLibrary::get_Specific_Library(int VAO, int VBO)
{
	
	for (auto rttlib : libraries) {
		for (auto lib : rttlib->getLibrary()) {
			if (lib->getVAO() == VAO && lib->getVBO() == VBO)
				return lib;
		}
	}
	return nullptr;
}

LetterLibrary* RendToTextObjLibrary::get_Letter_Library(int width, int height)
{
	std::vector<RendToTextObjLibraryElement*>::iterator it = std::find_if(libraries.begin(), libraries.end(), [width, height](RendToTextObjLibraryElement* lib) {
		return (lib->getWidth() == width && lib->getHeight() == height);
		});
	if (it != libraries.end()) {
		return (*it)->get_Letter_Library();
	}
	else
		return nullptr;
}

RendToTextObjLibrary::RendToTextObjLibraryElement::RendToTextObjLibraryElement(int width, int height) : width(width), height(height)
{
	for (auto& v : objLibraries) {
		library.push_back(new ObjectLibrary(v, width, height));
	}
	letterLib = new LetterLibrary("Letters");
	for (auto& obLib : library) {
		if (obLib->getName() == "Letters") {
			for (auto& ob : obLib->getObjects()) {
				letterLib->insertLetter(ob->getName(), obLib->getVAO(), ob->getVertices()->getTextPos(), ob->getVertices()->getID(), 0, 0, 0, 0);
			}
		}
	}
	for (auto& letter : letterLibrary.get_Map()) {
		auto& listOther = letter.second;
		auto& list = letterLib->getLetterInformation(letter.first);
		list[3] = listOther[3];
		list[4] = listOther[4];
		list[5] = listOther[5];
		list[6] = listOther[6];
	}
}

RendToTextObjLibrary::RendToTextObjLibraryElement::~RendToTextObjLibraryElement()
{
	for (auto& lib : library) {
			delete lib;
	}
    delete letterLib;
}
