#pragma once

class Component;

#include <Core/Entities/GameObject.hpp>
#include <iostream>

class Component {
public:
	GameObject* gameObject;
	bool Enabled = true;
	
	Component(){}
	~Component(){}

	virtual void Start(){}
	virtual void Update(){}
	virtual void Clear(){}
};