#pragma once

#include "Core/Entities/Actor.hpp"

class SkyBoxComponent;

class Scene : public Actor
{

public:
	Scene();
	~Scene();

	void SetAsCurrentScene();

	static Scene* GetCurrentScene();

private:

	SkyBoxComponent * SkyBox = nullptr;

	void AttachToActor(Actor* ActorTarget) override {};
	void DetachFromParent() override {};

	void SetRelativeTransform(const Transform& NewRelativeTransform) override {};
	void SetWorldTransform(const Transform& NewWorldTransform)override {};

	static Scene* CurrentScene;
};
