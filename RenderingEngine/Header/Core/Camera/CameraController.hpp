#pragma once

#include "Core/Components/Component.hpp"

class CameraController : public Component 
{
public:
	CameraController();
	virtual void Start() override;
	virtual void Update() override;
};
