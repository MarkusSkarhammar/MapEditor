#include "Render.h"


void render(game_state const& interpolated_state, GLFWwindow* window) {

	// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// Update camera based on a interpolated state
	setCameraPosition(interpolated_state.xCameraPos, interpolated_state.yCameraPos);

	bool overTen(false);
	size_t from = 0, obj = 0;
	size_t to = z;
	if (z > 6) {
		from = 7;
		to = z;
	}
	std::string currentName = "";

	//----------------------------------------------------------
	//						Draw game stuff
	//----------------------------------------------------------
	// Draw order tiles > Borders > Decoration > Entities > Doodads
	glUseProgram(program);
	glUniform1f(ambientStrength, ambientStrengthValue);

	double x(0), y(0);
	for (int floorAt = from; floorAt <= to; floorAt++) {

		if (instanceDraw.doInstanceDrawing(floorAt)) { // Draw Tiles through the means of instance drawing
			instanceDraw.drawInstancing(floorAt);
		}

		for (auto item : itemsToDraw) { // Draw Items ontop of the tiles
			auto object = item.second;
			if (item.first == floorAt && object) { // Make sure to draw only items on the current floor
				x = 0.0 + (width * object->getXPosition());
				y = 0.0 - (height * object->getYPosition());
				if (object->getDraw()) { 
					object->renderWorldTile(x, y);
				}
			}
		}

	}

	//----------------------------------------------------------
	//						Draw Outlined item
	//----------------------------------------------------------

	if (!(eraseToggle || destroyToggle || destroyTileToggle || copyToggle || cutToggle)) {
		if (outlinedItem != nullptr && outlinedItem->getObject() && outlinedItem->getObject()->getOutline()) {
			if(outlineItemBool)
				outlinedItem->getObject()->renderOutlineItem();
			else if(outlineTileBool)
				outlinedItem->getObject()->renderOutlineTile();
		}else if (!outlineItemBool && outlineTileBool && outLinedTile) 
				outLinedTile->getObject()->renderOutlineTile();
	}

	//----------------------------------------------------------
	//						Draw GUI
	//----------------------------------------------------------
	glUseProgram(program);
	//glBindTexture(GL_TEXTURE_2D, fbo_palette_modifier_left_texture);

	for (auto& objects : objects) {
		currentName = objects->getName();
		if (!((currentName.compare("GUI_LeftPanel_DropDown_") == 0 || currentName.compare("GUI_LeftPanel_DropDown_Text_") == 0) && !clickPaletteDropDown)) {
			for (auto& object : objects->getObjects()) {
				object->renderGUI(currentName);
			}
		}
	}


	glfwSwapBuffers(window);
	glfwPollEvents();
}

void prepareDraw() {

	instanceDraw.clearElements();
	itemsToDraw.clear();

	bool test = false;
	size_t from = 0;
	size_t to = z;
	if (z > 6) {
		from = 7;
		to = z;
	}

	double x(0), y(0);
	for (int floorAt = from; floorAt <= to; floorAt++) {
		for (auto& t : world.getFloor(floorAt).getSection(currentSection)) {
			auto object = t->getObject();
			if (object != nullptr || t->getID() == -1) {
				test = false;
				x = 0.0 + (width * t->getX());
				y = 0.0 - (height * t->getY());
				if (zoom == 1 && x < xCameraPos + 1.0f && x >= xCameraPos - 1.1f && y <= yCameraPos + 1.3f && y > yCameraPos - 1.1f)
					test = true;
				else if (zoom > 1.0 && x < xCameraPos + 1.0f / (1. / zoom) && x >= xCameraPos - 1.2f / (1. / zoom) && y <= yCameraPos + 1.2f / (1. / zoom) && y >(yCameraPos - 1.2f) / (1. / zoom)) // && y <= (yCameraPos + .5f) / (1. / zoom) && y > (yCameraPos - 1.2f) / (1 - 1. / zoom)
					test = true;
				else if (zoom < 1.0 && x < xCameraPos + 1.0f / (1. / zoom) && x >= xCameraPos - 1.4f / (1. / zoom) && y <= yCameraPos + 1.8f / (1. / zoom) && y > yCameraPos - 1.8f / (1. / zoom))
					test = true;
				if (test) {

					if (object && object->getDraw()) {
						instanceDraw.insertElement(object->getID(), floorAt, object->getVAO(), object->getTexturePos(), object);
					}

					for (auto& i : t->getAllItems()) {
						itemsToDraw.push_back(std::pair<int, DrawObject*>(floorAt, i->getObject()));
					}
				}
			}
		}
	}

	instanceDraw.generateTranslations();
}

void Instancing::insertElement(int ID, int floor, int VAO, int textPos, DrawObject* ob)
{
	std::vector<Element*>::iterator it = std::find_if(elements.begin(), elements.end(), [ID, floor](Element* e) {
		return (e->getID() == ID && e->getFloor() == floor);
		});
	if (it != elements.end()) {
		(*it)->insertObject(ob);
	}
	else {
		auto e = new Instancing::Element(ID, floor, VAO, textPos);
		e->insertObject(ob);
		elements.push_back(e);

	}
}

void Instancing::generateTranslations()
{
	for (auto e : elements) {
		e->generateTranslations();
	}
}

void Instancing::clearElements()
{
	for (auto e : elements)
		e->clear();
}

void Instancing::drawInstancing(int floor)
{
	glUniform1i(instancing, 1);
	float x = 0.0;
	float y = 0.0;
	for (auto e : elements) {

		auto ob = e->getTranslations();
		if (ob.size() > 0 && e->getFloor() == floor) {
			glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * ob.size(), &ob[0], GL_STATIC_DRAW);

			
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glVertexAttribDivisor(2, 1);
			

			x = 0.0;
			y = 0.0;
			Model = glm::mat4(1.0f);
			Model = glm::translate(Model, glm::vec3(x, y, 0.0f));


			// Handle scaling
			Model = glm::scale(Model, glm::vec3(1.0, 1.0, 1.0));

			//glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
			glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Model));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glUniform1i(gLayer, e->getTextPos());

			glBindVertexArray(e->getVAO());
			glDrawArraysInstanced(GL_TRIANGLES, 0 + (4 * e->getID() % 1024), 3, ob.size());
			glDrawArraysInstanced(GL_TRIANGLES, 1 + (4 * e->getID() % 1024), 3, ob.size());
		}

	}
	glUniform1i(instancing, 0);
}

bool Instancing::doInstanceDrawing(int floor)
{
	for (auto e : elements) {
		if (e->getObjects().size() > 0 && e->getTranslations().size() > 0 && e->getFloor() == floor)
			return true;
	}
	return false;
}

Instancing::Element::Element(int ID, int floor, int VAO, int textPos) : ID(ID), floor(floor), VAO(VAO), textPos(textPos)
{
}

void Instancing::Element::generateTranslations()
{
	this->clearTranslations();
	float x{ 0.f }, y{ 0.f };
	for (auto ob : objects) {
		x = 0.0 + (width * ob->getXPosition());
		y = 0.0 - (height * ob->getYPosition());
		glm::vec2 translation;
		translation.x = (float)x;
		translation.y = (float)y;
		translations.push_back(translation);
	}
}

void Instancing::Element::clear()
{
	objects.clear();
	objects.shrink_to_fit();
	translations.clear();
	translations.shrink_to_fit();
}

void Instancing::Element::clearTranslations()
{
	translations.clear();
	translations.shrink_to_fit();
}
