#include "Utils/Axes.hpp"
#include "Core/Camera/Camera.hpp"
#include "GL/glew.h"
#include "glm/mat4x4.hpp"

void Axes::DrawGrid()
{
	int Count = 100;
	float Length = 5;
	glBegin(GL_LINES);
	glColor4f(25.0f/255.0f, 35.0f/255.0f, 46.0f/255.0f, 0.25f);
	for(int x = -Count; x<Count; x++)
	{
		if(x != 0)
		{
			glVertex3f(x*Length, 0, 100*5);
			glVertex3f(x*Length, 0, -100*5);
		}
	}
	for(int z = -Count; z<Count; z++)
	{
		if(z != 0)
		{
			glVertex3f(100*Length, 0, z*Length);
			glVertex3f(-100*Length, 0, z*Length);
		}
	}
	glEnd();
}

void Axes::DrawAxes()
{
	float Length = 500;
	glBegin(GL_LINES);
	// draw line for X axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-Length, 0, 0);
	glVertex3f(Length, 0.0f, 0.0f);
	// draw line for Y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, -Length, 0);
	glVertex3f(0.0f, Length, 0.0f);
	// draw line for Z axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 0, -Length);
	glVertex3f(0.0f, 0.0f, Length);
	glEnd();
}

void Axes::LoadCameraMatrix()
{
	const Camera* ActiveCamera = Camera::GetActiveCamera();
	if (ActiveCamera)
	{
		glm::mat4x4 CameraMatrix = ActiveCamera->GetProjectionMatrix() * ActiveCamera->GetViewMatrix();
		glLoadIdentity();
		glMultMatrixf(&CameraMatrix[0][0]);
	}
}