#include "Camera.h"

void updateCameraPosition(double xPos, double yPos) {
	xCameraPos += xPos;
	yCameraPos += yPos;

	// Projection matrix : 45° Field of View, 4 : 3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(zoom), float(1920 / 1080), 0.1f, 100.0f);
	//= glm::ortho((-1.0f + float(xCameraPos)), (1.0f + float(xCameraPos)), (-1.0f + float(yCameraPos)), (1.0f + float(yCameraPos)), .1f , 100.f);

	// Camera matrix
	//glLoadIdentity();
	View = glm::lookAt(
		glm::vec3(xCameraPos, yCameraPos, 1), // Camera is at (4,3,-3), in World Space
		glm::vec3(xCameraPos, yCameraPos, 0.0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(View));

}

void setCameraPosition(double xPos, double yPos) {
	xCameraPos = xPos;
	yCameraPos = yPos;

	// Projection matrix : 45° Field of View, 4 : 3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(FOV), float(1920 / 1080), 0.0f, 10.0f);
	//= glm::ortho((-1.0f + float(xCameraPos)), (1.0f + float(xCameraPos)), (-1.0f + float(yCameraPos)), (1.0f + float(yCameraPos)), .1f , 100.f);

	// Camera matrix
	//glLoadIdentity();
	View = glm::lookAt(
		glm::vec3(xCameraPos, yCameraPos, zoom), // Camera is at (4,3,-3), in World Space
		glm::vec3(xCameraPos, yCameraPos, 0.0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(View));

}

void setCameraZoom(float scale) {

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(zoom), 1.0f, 0.1f, 10.0f);
	// Camera matrix
	View = glm::lookAt(
		glm::vec3(0, 0, 1.0f), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	//MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

}
