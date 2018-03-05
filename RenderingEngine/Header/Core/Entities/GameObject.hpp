#pragma once

class GameObject;

#include <Core/Transform.hpp>
#include <Core/Components/Component.hpp>
#include <list>

class GameObject {
private:
	bool destroyed = false;

public:
	
	Transform transform;

	GameObject* parent = NULL;
	std::list<GameObject*> children;
	std::list<Component*> components;

	GameObject();
	GameObject(Transform const &transform);

	void AddChild(GameObject &childObject);
	void RemoveChild(GameObject &childObject);
	void AddComponent(Component &component);

	void Start();
	void Update();
	void Clear();

	void Destroy();
	bool IsDestroyed();

	Transform GetWorldTransform();
};