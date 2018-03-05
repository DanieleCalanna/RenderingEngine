#include "GL/glew.h"
#include "Utils/Axes.hpp"
#include "Core/Components/CameraComponent.hpp"

void Axes::glDrawGrid(){
	int count = 100;
	float length = 5;
	glBegin(GL_LINES);
	glColor4f(25.0f/255.0f, 35.0f/255.0f, 46.0f/255.0f, 0.25f);
	for(int x = -count; x<count; x++){
		if(x != 0){
			glVertex3f(x*length, 0, 100*5);
			glVertex3f(x*length, 0, -100*5);
		}
	}
	for(int z = -count; z<count; z++){
		if(z != 0){
			glVertex3f(100*length, 0, z*length);
			glVertex3f(-100*length, 0, z*length);
		}
	}
	glEnd();
}

void Axes::glDrawAxes(){
	float length = 500;
	glBegin(GL_LINES);
	// draw line for x axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-length, 0, 0);
	glVertex3f(length, 0.0f, 0.0f);
	// draw line for y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, -length, 0);
	glVertex3f(0.0f, length, 0.0f);
	// draw line for Z axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 0, -length);
	glVertex3f(0.0f, 0.0f, length);
	glEnd();
}

void Axes::glLoadCameraMatrix(){
	glm::mat4x4 cameraMatrix = CameraComponent::getInstance().GetProjectionMatrix() * CameraComponent::getInstance().GetViewMatrix();
	glLoadIdentity();
	glMultMatrixf(&cameraMatrix[0][0]);
}