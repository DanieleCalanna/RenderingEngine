#pragma once

#include "Core/Entities/Actor.hpp"

class Scene : public Actor
{

public:
	Scene();
	~Scene();

private:



	void AttachToActor(Actor* ActorTarget) override {};
	void DetachFromParent() override {};

	void SetRelativeTransform(const Transform& NewRelativeTransform) override {};
	void SetWorldTransform(const Transform& NewWorldTransform)override {};
};
