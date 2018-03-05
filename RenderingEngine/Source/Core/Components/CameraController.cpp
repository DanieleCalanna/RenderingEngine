#include <Core/Components/CameraController.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Core/Window.hpp>


void CameraController::Start(){
	xRotation = (float)(gameObject->transform.Rotation[0]*180/3.1415927f);
}
void CameraController::Update(){

	double speed = (Window::GetWindow().yWheelOffset+1)/20+1;
	if(speed<1) speed = 1;

	xRotation += Window::GetWindow().getMouseY()/8;
	if(xRotation>90) xRotation = 90;
	if(xRotation<-90) xRotation = -90;
	zRotation += Window::GetWindow().getMouseX()/8;
	
	float x = (float)sin(zRotation*3.1415927f/180.0f) * 0.5f *speed;
	float y = (float)sin(xRotation*3.1415927f/180.0f) * 0.5f *speed;
	float z = (float)cos(zRotation*3.1415927f/180.0f) * 0.5f *speed;
	
	
	gameObject->transform.Rotation[0] = xRotation;
	gameObject->transform.Rotation[1] = zRotation;
	
	if(Window::GetWindow().GetKeyDown(GLFW_KEY_W))
	{
		gameObject->transform.Location = glm::vec3(
				gameObject->transform.Location.x+x,
				gameObject->transform.Location.y-y,
				gameObject->transform.Location.z-z);
	}
	if(Window::GetWindow().GetKeyDown(GLFW_KEY_S))
	{
		gameObject->transform.Location = glm::vec3(
				gameObject->transform.Location.x-x,
				gameObject->transform.Location.y+y,
				gameObject->transform.Location.z+z);
	}
	if(Window::GetWindow().GetKeyDown(GLFW_KEY_D)){
		gameObject->transform.Location = glm::vec3(
				gameObject->transform.Location.x+z,
				gameObject->transform.Location.y,
				gameObject->transform.Location.z+x);
	}
	if(Window::GetWindow().GetKeyDown(GLFW_KEY_A)){
		gameObject->transform.Location = glm::vec3(
				gameObject->transform.Location.x-z,
				gameObject->transform.Location.y,
				gameObject->transform.Location.z-x);
	}
}