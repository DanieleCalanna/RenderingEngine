#pragma once

#include "Core/Components/Component.hpp"

class CameraController : public Component 
{

private:

	float XRotation = 0;
	float zRotation = 0;

public:

	virtual void Start() override;
	virtual void Update() override;

};
