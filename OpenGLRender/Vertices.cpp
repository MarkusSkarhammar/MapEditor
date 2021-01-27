#include "Vertices.h"
#include "Global.h"
#pragma warning(disable: 4244)

void removeVertices(Vertices * v)
{
	
	if (!v)
		return;
	int pos = 0;

	auto& verticesPos = v->getV();
	auto& verticesText = v->getVT();

	verticesPos.erase(verticesPos.begin() + 8 * v->getID(), verticesPos.begin() + 8 * v->getID() + 8);
	verticesText.erase(verticesText.begin() + 8 * v->getID(), verticesText.begin() + 8 * v->getID() + 8);

}

Vertices::Vertices(std::string name, float xStartText, float yStartText, float widthText, float heightText, float width, float height, std::vector<float>& v, std::vector<float>& vt, int textPos, GLuint VAO, GLuint VBO, int ID, bool doubleSize) : name(name), xStartText(xStartText), yStartText(yStartText),
width(width), height(height), widthText(widthText), heightText(heightText), v(v), vt(vt), textPos(textPos), VAO(VAO), VBO(VBO), ID(ID), doubleSize(doubleSize)
{

	float widthStart = 0.0f ;
	float heightStart = 0.0f;
	if (doubleSize) {
		widthStart = 0.0f - (64.0 / (double(screenWidthPixels) / 2));
		heightStart = 0.0f + (64.0 / (double(screenHeightPixels) / 2));
	}

	float widthPos = ((this->width) / (double(screenWidthPixels) / 2));
	float heightPos = ((this->height) / (double(screenHeightPixels) / 2));

	float textWidthStart = ((xStartText+.5f) / textureSizeWidth);
	float textHeightStart = ((yStartText+.5f) / textureSizeHeight);
	float textWidthEnd = ((xStartText+widthText-.5f) / textureSizeWidth);
	float textHeightEnd = ((yStartText+heightText-0.5f) / textureSizeHeight);

	v.push_back(widthStart); v.push_back(heightStart); vt.push_back(textWidthStart); vt.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + widthPos); v.push_back(heightStart); vt.push_back(textWidthEnd); vt.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - heightPos); vt.push_back(textWidthStart); vt.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + widthPos); v.push_back(heightStart - heightPos); vt.push_back(textWidthEnd); vt.push_back(textHeightEnd); // Bottom-right
}
Vertices::Vertices(Vertices* other, std::vector<float>& v, std::vector<float>& vt, int VAO, int VBO) : v(v), vt(vt), VAO(VAO), VBO(VBO)
{
	xStartText = other->xStartText;
	yStartText = other->yStartText;
	widthText = other->widthText;
	heightText = other->heightText;

	ID = (v.size() / 8);

	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float widthPos = (this->width / (double(screenWidthPixels) / 2));
	float heightPos = (this->height / (double(screenHeightPixels) / 2));

	float textWidthStart = (xStartText / textureSizeWidth);
	float textHeightStart = (yStartText / textureSizeHeight);
	float textWidthEnd = textWidthStart + (widthText / textureSizeWidth);
	float textHeightEnd = textHeightStart + (heightText / textureSizeHeight);


	//first triangle
	v.push_back(widthStart); v.push_back(heightStart); vt.push_back(textWidthStart); vt.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + widthPos); v.push_back(heightStart); vt.push_back(textWidthEnd); vt.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - heightPos); vt.push_back(textWidthStart); vt.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + widthPos); v.push_back(heightStart - heightPos); vt.push_back(textWidthEnd); vt.push_back(textHeightEnd); // Bottom-right

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float) + vt.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), &v[0]);

	glBufferSubData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vt.size() * sizeof(float), &vt[0]);
}
Vertices::Vertices(Vertices* other, float textureSizeWidth, float textureSizeHeight, std::vector<float>& v, std::vector<float>& vt, int VAO, int VBO) : v(v), vt(vt), VAO(VAO), VBO(VBO), textureSizeWidth(other->getTextureSizeWidth()), textureSizeHeight(other->getTextureSizeHeight())
{
	xStartText = other->xStartText;
	yStartText = other->yStartText;
	widthText = other->widthText;
	heightText = other->heightText;
	width = other->getWidth();
	height = other->getHeight();
	doubleSize = other->isDoubleSize();

	ID = (v.size() / 8);

	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float widthPos = (this->width / (double(textureSizeWidth) / 2));
	float heightPos = (this->height / (double(textureSizeHeight) / 2));

	float textWidthStart = (xStartText / this->textureSizeWidth);
	float textHeightStart = (yStartText / this->textureSizeHeight);
	float textWidthEnd = textWidthStart + (widthText / this->textureSizeWidth);
	float textHeightEnd = textHeightStart + (heightText / this->textureSizeHeight);


	//first triangle
	v.push_back(widthStart); v.push_back(heightStart); vt.push_back(textWidthStart); vt.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + widthPos); v.push_back(heightStart); vt.push_back(textWidthEnd); vt.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - heightPos); vt.push_back(textWidthStart); vt.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + widthPos); v.push_back(heightStart - heightPos); vt.push_back(textWidthEnd); vt.push_back(textHeightEnd); // Bottom-right

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float) + vt.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), &v[0]);

	glBufferSubData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vt.size() * sizeof(float), &vt[0]);
}
Vertices::Vertices(std::string name, float width, float height, Vertices* other, bool rtt) : name(name), width(width), height(height), v(other->getV()), vt(other->getVT())
{
	xStartText = other->getXStartText();
	yStartText = other->getYStartText();
	widthText = other->getWidthText();
	heightText = other->getHeightText();
	textureSizeHeight = other->textureSizeHeight;
	textureSizeWidth = other->textureSizeWidth;
	VAO = other->getVAO();
	VBO = other->getVBO();
	this->textPos = other->getTextPos();

	ID = (v.size() / 8);

	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float widthPos = (this->width / (double(screenWidthPixels) / 2));
	float heightPos = (this->height / (double(screenHeightPixels) / 2));

	if (rtt) {
		widthPos = (this->width / (double(textureSizeWidth) / 2));
		heightPos = (this->height / (double(textureSizeHeight) / 2));
	}

	float textWidthStart = (xStartText / textureSizeWidth);
	float textHeightStart = (yStartText / textureSizeHeight);
	float textWidthEnd = textWidthStart + (widthText / textureSizeWidth);
	float textHeightEnd = textHeightStart + (heightText / textureSizeHeight);


	//first triangle
	v.push_back(widthStart); v.push_back(heightStart); vt.push_back(textWidthStart); vt.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + widthPos); v.push_back(heightStart); vt.push_back(textWidthEnd); vt.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - heightPos); vt.push_back(textWidthStart); vt.push_back(textHeightEnd); // Bottom-left

	//Second triangle
	v.push_back(widthStart + widthPos); v.push_back(heightStart - heightPos); vt.push_back(textWidthEnd); vt.push_back(textHeightEnd); // Bottom-right

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float) + vt.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), &v[0]);

	glBufferSubData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vt.size() * sizeof(float), &vt[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(v.size() * sizeof(float)));
}
Vertices::Vertices(Vertices* other) : v(other->getV()), vt(other->getVT())
{
	xStartText = other->xStartText;
	yStartText = other->yStartText;
	widthText = other->widthText;
	heightText = other->heightText;
	textureSizeHeight = other->textureSizeHeight;
	textureSizeWidth = other->textureSizeWidth;

	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float textWidthStart = (xStartText / textureSizeHeight);
	float textHeightStart = (yStartText / textureSizeWidth);
	float textWidthEnd = textWidthStart + (widthText / textureSizeHeight);
	float textHeightEnd = textHeightStart + (heightText / textureSizeWidth);


	auto tPos = (ID * 8);
	//first triangle
	vt.at(tPos) = (textWidthStart); vt.at(++tPos) = (textHeightStart); // Top-left
	vt.at(++tPos) = (textWidthEnd); vt.at(++tPos) = (textHeightStart); // Top-right
	vt.at(++tPos) = (textWidthStart); vt.at(++tPos) = (textHeightEnd); // Bottom-left

	//Second triangle
	vt.at(++tPos) = (textWidthEnd); vt.at(++tPos) = (textHeightEnd); // Bottom-right

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float) + vt.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), &v[0]);

	glBufferSubData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vt.size() * sizeof(float), &vt[0]);
}

Vertices::Vertices(Vertices* other, bool thisIsATempVertices) : v(other->getV()), vt(other->getVT())
{
	xStartText = other->xStartText;
	yStartText = other->yStartText;
	widthText = other->widthText;
	heightText = other->heightText;
	textureSizeHeight = other->textureSizeHeight;
	textureSizeWidth = other->textureSizeWidth;
}

void Vertices::setXStartText(float v)
{
	xStartText = v;
	update();
}

void Vertices::setYStartText(float v)
{
	yStartText = v;
	update();
}

void Vertices::setWidthText(float v)
{
	widthText = v;
	update();
}

void Vertices::setHeightText(float v)
{
	heightText = v;
	update();
}

void Vertices::set_All(float xStart, float yStart, float width, float heigth)
{
	xStartText = xStart;
	yStartText = yStart;
	widthText = width;
	heightText = height;
	this->width = width;
	this->height = height;
	update();
}

void Vertices::copyAnotherVerticesText(Vertices* other)
{

	xStartText = other->xStartText;
	yStartText = other->yStartText;
	widthText = other->widthText;
	heightText = other->heightText;

	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float textWidthStart = (xStartText / 2048.0);
	float textHeightStart = (yStartText / 2048.0);
	float textWidthEnd = textWidthStart + (widthText / 2048.0);
	float textHeightEnd = textHeightStart + (heightText / 2048.0);


	auto tPos = (ID * 8);
	//first triangle
	vt.at(tPos) = (textWidthStart); vt.at(++tPos) = (textHeightStart); // Top-left
	vt.at(++tPos) = (textWidthEnd); vt.at(++tPos) = (textHeightStart); // Top-right
	vt.at(++tPos) = (textWidthStart); vt.at(++tPos) = (textHeightEnd); // Bottom-left

	//Second triangle
	vt.at(++tPos) = (textWidthEnd); vt.at(++tPos) = (textHeightEnd); // Bottom-right

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float) + vt.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), &v[0]);

	glBufferSubData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vt.size() * sizeof(float), &vt[0]);
}

void Vertices::changeSize(int width, int height)
{
	this->width = width;
	this->height = height;

	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float widthPos = (this->width / (double(screenWidthPixels) / 2));
	float heightPos = (this->height / (double(screenHeightPixels) / 2));

	auto vPos = (ID * 8);
	//first triangle
	v.at(vPos) = (widthStart); v.at(++vPos) = (heightStart); // Top-left
	v.at(++vPos) = (widthStart + widthPos); v.at(++vPos) = (heightStart); // Top-right
	v.at(++vPos) = (widthStart); v.at(++vPos) = (heightStart - heightPos); // Bottom-left

	//Second triangle
	v.at(++vPos) = (widthStart + widthPos); v.at(++vPos) = (heightStart - heightPos); // Bottom-right

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float) + vt.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), &v[0]);

	glBufferSubData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vt.size() * sizeof(float), &vt[0]);
}

void Vertices::change_All(float xStartText, float yStartText, float widthText, float heightText, float width, float height)
{
	this->xStartText = xStartText;
	this->yStartText = yStartText;
	this->widthText = widthText;
	this->heightText = heightText;
	this->width = width;
	this->height = height;

	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float widthPos = (this->width / (double(screenWidthPixels) / 2));
	float heightPos = (this->height / (double(screenHeightPixels) / 2));

	float textWidthStart = (xStartText / textureSizeWidth);
	float textHeightStart = (yStartText / textureSizeHeight);
	float textWidthEnd = textWidthStart + (widthText / textureSizeWidth);
	float textHeightEnd = textHeightStart + (heightText / textureSizeHeight);

	auto vPos = (ID * 8);
	//first triangle
	v.at(vPos) = (widthStart); v.at(++vPos) = (heightStart); // Top-left
	v.at(++vPos) = (widthStart + widthPos); v.at(++vPos) = (heightStart); // Top-right
	v.at(++vPos) = (widthStart); v.at(++vPos) = (heightStart - heightPos); // Bottom-left

	//Second triangle
	v.at(++vPos) = (widthStart + widthPos); v.at(++vPos) = (heightStart - heightPos); // Bottom-right

	auto tPos = (ID * 8);
	//first triangle
	vt.at(tPos) = (textWidthStart); vt.at(++tPos) = (textHeightStart); // Top-left
	vt.at(++tPos) = (textWidthEnd); vt.at(++tPos) = (textHeightStart); // Top-right
	vt.at(++tPos) = (textWidthStart); vt.at(++tPos) = (textHeightEnd); // Bottom-left

	//Second triangle
	vt.at(++tPos) = (textWidthEnd); vt.at(++tPos) = (textHeightEnd); // Bottom-right

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float) + vt.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), &v[0]);

	glBufferSubData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vt.size() * sizeof(float), &vt[0]);
}

void Vertices::update()
{
	float widthStart = 0.0f;
	float heightStart = 0.0f;

	float textWidthStart = (xStartText / textureSizeWidth);
	float textHeightStart = (yStartText / textureSizeHeight);
	float textWidthEnd = textWidthStart + (widthText / textureSizeWidth);
	float textHeightEnd = textHeightStart + (heightText / textureSizeHeight);


	auto tPos = (ID * 8);
	//first triangle
	vt.at(tPos) = (textWidthStart); vt.at(++tPos) = (textHeightStart); // Top-left
	vt.at(++tPos) = (textWidthEnd); vt.at(++tPos) = (textHeightStart); // Top-right
	vt.at(++tPos) = (textWidthStart); vt.at(++tPos) = (textHeightEnd); // Bottom-left

	//Second triangle
	vt.at(++tPos) = (textWidthEnd); vt.at(++tPos) = (textHeightEnd); // Bottom-right

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float) + vt.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), &v[0]);

	glBufferSubData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vt.size() * sizeof(float), &vt[0]);
}

/*
void VerticesContainer::addVertices(Vertices* v)
{
	vertices.push_back(v);
}

VertecesHandler::VertecesHandler(VertecesHandler* other) : name(other->name), textureID(other->getTextureID())
{
}
*/

