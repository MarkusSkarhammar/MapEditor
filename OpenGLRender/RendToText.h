#ifndef RENDER_TO_TEXTURE
#define	RENDER_TO_TEXTURE

// Include GLEW
#include <GL/glew.h>

#include <string>
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
private:
	std::string name;
	double width, height;
	GLuint fbo{ 0 }, rbo{ 0 }, text{ 0 };
};

#endif