#include "DrawObject.h"
#include <chrono>
#include "Global.h"
#include "Item.h"
#pragma warning(disable: 4244)

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

void DrawObjects::addObject(DrawObject * o)
{
	if (AnimationObject* a = dynamic_cast<AnimationObject*>(o)) {
		aObjects.push_back(a);
	}
		objects.push_back(o);
}

void DrawObjects::setObjects(std::vector<DrawObject*> input)
{
	clearObjects();
	for (auto& o : input) {
		if (AnimationObject* a = dynamic_cast<AnimationObject*>(o)) {
			AnimationObject* aO = new AnimationObject(a);
			aObjects.push_back(aO);
			objects.push_back(aO);
		}
		else
			objects.push_back(new DrawObject(o));
	}
}

void DrawObjects::clearObjects()
{
	aObjects.clear();
	objects.clear();
}

void DrawObjects::checkAnimations()
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
			if (!skip && !updateWorld) {
				auto sectionPtr = world->getFloor(floorAt)->get_Section(section);
				if(sectionPtr)
					for (auto& t : sectionPtr->get_Tiles()) {
						auto object = t->getObject();
						if (object) {
							x = 0.0 + (width * object->getXPosition());
							y = 0.0 - (height * object->getYPosition());
							if (x < xCameraPos + 1.0f * zoomWorld && x >= xCameraPos - 1.1f * zoomWorld && y <= yCameraPos + 1.2f * zoomWorld && y > yCameraPos - 1.0f * zoomWorld && object->getDraw()) {
								for (auto& i : t->getAllItems()) {
									if (AnimationObject* check = dynamic_cast<AnimationObject*>(i->getObject())) {
										check->checkAnimation();
									}
								}
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

void changeElementSizeAndTexture(Vertices*& v, int xTopLeft, int yTopLeft, int xTopRight, int yTopRight, int xBottomLeft, int yBottomLeft, int xBottomRight, int yBottomRight, int xTopLeftText, int yTopLeftText, int xTopRightText, int yTopRightText, int xBottomLeftText, int yBottomLeftText, int xBottomRightText, int yBottomRightText)
{
}

void restoreElement(Vertices*& v) {
	
}

void DrawObject::rendToText(bool useWorldZoom)
{
	if (rtt != nullptr) {
		normalRender = false;
		glUseProgram(program2);
		glBindTexture(GL_TEXTURE_2D, rtt->getTextureID());
		glUniform1i(gLayer, 0);

		Model = glm::mat4(1.0f);
		if(useWorldZoom)
			Model = glm::translate(Model, glm::vec3(xCameraPos - 1.0f + x, yCameraPos + 1.0f - y, (zoomWorld - 1.0)));
		else
			Model = glm::translate(Model, glm::vec3(xCameraPos - 1.0f + x, yCameraPos + 1.0f - y, (zoom - 1.0)));

		// Handle scaling
		if (scale != 1.0)
			Model = glm::scale(Model, glm::vec3(scale, scale, scale));
		else if (scaleX != 1.0 || scaleY != 1.0)
			Model = glm::scale(Model, glm::vec3(scaleX, scaleY, 1.0));
		
		//glUniformMatrix4fv(projection2, 1, GL_FALSE, glm::value_ptr(Projection));
		//glUniformMatrix4fv(view2, 1, GL_FALSE, glm::value_ptr(View));
		//glUniformMatrix4fv(model2, 1, GL_FALSE, glm::value_ptr(Model));
		glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		if (textOffsetX >= 0 || textOffsetY >= 0) {
			//textOffsetValues = { object->getTextOffsetX(), object->getTextOffsetY() };
			//glUniform2f(textOffset2, textOffsetX, textOffsetY);
			glm::vec2 offset = glm::vec2(textOffsetX, textOffsetY);
			glBindBuffer(GL_UNIFORM_BUFFER, UBOTextureStuff);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(offset));
			glBindBuffer(GL_UNIFORM_BUFFER, 1);
		}
		
	}
}

void DrawObject::renderWorldTile(double& x, double& y) {


	Model = glm::mat4(1.0f);
	//double test = yCameraPos = round(yCameraPos * 1000) / 1000.;
	Model = glm::translate(Model, glm::vec3(x, y, 0.0f));

	// Handle scaling
	if (scale != 1.0)
		Model = glm::scale(Model, glm::vec3(scale, scale, scale));

	//glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
	glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glUniform1i(gLayer, texturePos);

	if (textOffsetX >= 0 || textOffsetY >= 0) {
		//textOffsetValues = { object->getTextOffsetX(), object->getTextOffsetY() };
		//glUniform2f(textOffset2, textOffsetX, textOffsetY);
		glm::vec2 offset = glm::vec2(textOffsetX, textOffsetY);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOTextureStuff);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(offset));
		glBindBuffer(GL_UNIFORM_BUFFER, 1);
	}

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0 + (4 * id % 1024), 3);
	glDrawArrays(GL_TRIANGLES, 1 + (4 * id % 1024), 3);
}

void DrawObject::renderGUI(string name)
{
	if (draw) {
		glUniform1i(GUI, 1);
		bool doZoom = false;
		if (zoom == 1.0)
			doZoom = true;

		rendToText(doZoom);

		if (normalRender) {

			Model = glm::mat4(1.0f);
			if (name.compare("GUI_Preview_Tiles_") == 0) Model = glm::translate(Model, glm::vec3(xCameraPos - 1.0f + x, yCameraPos + 1.0f - y, 0.0));
			else if (name.compare("world") == 0) Model = glm::translate(Model, glm::vec3(x, y, 0.0f));
			else if (doZoom) Model = glm::translate(Model, glm::vec3(xCameraPos - 1.0f + x, yCameraPos + 1.0f - y, (zoomWorld - 1.0)));
			else Model = glm::translate(Model, glm::vec3(xCameraPos - 1.0f + x, yCameraPos + 1.0f - y, (zoom - 1.0)));

			// Handle scaling
			if (scale != 1.0)
				Model = glm::scale(Model, glm::vec3(scale, scale, 1.0));
			else if (scaleX != 1.0 || scaleY != 1.0)
				Model = glm::scale(Model, glm::vec3(scaleX, scaleY, 1.0));

			//glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
			glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glUniform1i(gLayer, texturePos);

			if (textOffsetX >= 0 || textOffsetY >= 0) {
				//textOffsetValues = { object->getTextOffsetX(), object->getTextOffsetY() };
				//glUniform2f(textOffset2, textOffsetX, textOffsetY);
				glm::vec2 offset = glm::vec2(textOffsetX, textOffsetY);
				glBindBuffer(GL_UNIFORM_BUFFER, UBOTextureStuff);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(offset));
				glBindBuffer(GL_UNIFORM_BUFFER, 1);
			}
		}

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0 + (4 * id % 1024), 3);
		glDrawArrays(GL_TRIANGLES, 1 + (4 * id % 1024), 3);

		glUniform1i(GUI, 0);

		resetRenderMode();
	}
}

void DrawObject::renderOutlineTile()

{

	// Handle outline
	if (outline) {

		glUniform1i(outlineUniform, 1);

		float x = 0.0 + (width * getXPosition());
		float y = 0.0 - (height * getYPosition());
		{ // Draw object to the stencil table

			glEnable(GL_DEPTH_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			glEnable(GL_STENCIL_TEST);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			Model = glm::mat4(1.0f);
			Model = glm::translate(Model, glm::vec3(x, y, 0.0f));
			Model = glm::scale(Model, glm::vec3(scale, scale, scale));

			//glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
			glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glUniform1i(gLayer, texturePos);

			if (textOffsetX >= 0 || textOffsetY >= 0) {
				glm::vec2 offset = glm::vec2(textOffsetX, textOffsetY);
				glBindBuffer(GL_UNIFORM_BUFFER, UBOTextureStuff);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(offset));
				glBindBuffer(GL_UNIFORM_BUFFER, 1);
			}

			glBindVertexArray(VAO);

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			glDrawArrays(GL_TRIANGLES, 0 + (4 * id % 1024), 3);
			glDrawArrays(GL_TRIANGLES, 1 + (4 * id % 1024), 3);

		}

		{ // Draw the object again based on the stencil table

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

			glUniform1i(outlineUniform, 2);
			float scale = 1.1f;
			Model = glm::mat4(1.0f);
			
			if (itemAtlas.checkIfDouleSize(this->getTexturePos(), id))
				Model = glm::translate(Model, glm::vec3(x, y, 0.0f));
			else
				Model = glm::translate(Model, glm::vec3(x - width * (0.1f / 2), y + height * (0.1f / 2), 0.0f));
			Model = glm::scale(Model, glm::vec3(scale, scale, scale));
			glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0 + (4 * id % 1024), 3);
			glDrawArrays(GL_TRIANGLES, 1 + (4 * id % 1024), 3);
		}

		glUseProgram(program);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		glUniform1i(outlineUniform, 0);
	} 
}

void DrawObject::renderOutlineItem()
{
	glUniform1i(outlineUniform, 3); // Set uniform to 3 so that any non transparent pixels will be discarded

	float x = 0.0 + (width * getXPosition());
	float y = 0.0 - (height * getYPosition());

	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // We do not want to actually draw anything to the screen here, only to the stencil buffer. So disable any color so that nothing is being drawn.

	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Anything that is drawn sets the stencil buffer for that texel to one.
	glStencilMask(0xFF); // Enable drawing on the stencil buffer.

	float scale = 1.1f; // Scale up the first thing to draw
	glBindVertexArray(VAO);
	{ // First round of drawing to the stencil buffer.
		Model = glm::mat4(1.0f);
		if (itemAtlas.checkIfDouleSize(this->getTexturePos(), id))
			Model = glm::translate(Model, glm::vec3(x, y, 0.0f));
		else
			Model = glm::translate(Model, glm::vec3(x - (width * (scale - 1.f)) / 2, y + (height * (scale - 1.f)) / 2, 0.0f));
		Model = glm::scale(Model, glm::vec3(scale, scale, scale));

		//glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
		glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glUniform1i(gLayer, texturePos);

		if (textOffsetX >= 0 || textOffsetY >= 0) {
			glm::vec2 offset = glm::vec2(textOffsetX, textOffsetY);
			glBindBuffer(GL_UNIFORM_BUFFER, UBOTextureStuff);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(offset));
			glBindBuffer(GL_UNIFORM_BUFFER, 1);
		}

		glBindVertexArray(VAO);

		// Draw the first thing so that anything that is not transparent is written to the stencil buffer
		glDrawArrays(GL_TRIANGLES, 0 + (4 * id % 1024), 3);
		glDrawArrays(GL_TRIANGLES, 1 + (4 * id % 1024), 3);
	}
	
	
	{ // Second round of drawing to the stencil buffer.
		glUniform1i(outlineUniform, 4); // Set uniform to 4 so that any transparent pixels will be discarded

		//glStencilFunc(GL_EQUAL, 0, 0xFF);

		scale = 1.0f;
		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, glm::vec3(x - (width * (scale - 1.f)) / 2, y + (height * (scale - 1.f)) / 2, 0.0f));
		Model = glm::scale(Model, glm::vec3(scale, scale, scale));
		glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Draw the first thing so that anything that is not transparent is written to the stencil buffer
		glDrawArrays(GL_TRIANGLES, 0 + (4 * id % 1024), 3);
		glDrawArrays(GL_TRIANGLES, 1 + (4 * id % 1024), 3);
	}
	
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glUniform1i(outlineUniform, 2);

		scale = 1.1f;
		Model = glm::mat4(1.0f);
		if (itemAtlas.checkIfDouleSize(this->getTexturePos(), id))
			Model = glm::translate(Model, glm::vec3(x, y, 0.0f));
		else
			Model = glm::translate(Model, glm::vec3(x - (width * (scale - 1.f)) / 2, y + (height * (scale - 1.f)) / 2, 0.0f));
		Model = glm::scale(Model, glm::vec3(scale, scale, scale));
		glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0 + (4 * id % 1024), 3);
		glDrawArrays(GL_TRIANGLES, 1 + (4 * id % 1024), 3);
	}

	glUseProgram(program);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	glUniform1i(outlineUniform, 0);
}

void DrawObject::resetRenderMode()
{
	if (!normalRender) {
		glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D_ARRAY, gTileArrayTexture);
	}
}

TextDrawObject::~TextDrawObject()
{
	clear();
}

void TextDrawObject::renderGUI(std::string name)
{
	if(getDraw())
		for (auto ob : objects) {
			ob->renderGUI(name);
		}
}

void TextDrawObject::clear()
{
	for (auto ob : objects) {
		delete ob;
	}
	objects.clear();
	objects.shrink_to_fit();
}


