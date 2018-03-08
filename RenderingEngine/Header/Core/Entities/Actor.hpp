#pragma once

#include <list>

class Transform;
class Component;

class Actor
{
	friend class Actor;

public:

	Actor();
	~Actor();

	void AttachToActor(Actor* ActorTarget);
	void DetachFromParent();

	template <class ChildComponent> ChildComponent* AddComponent()
	{
		ChildComponent* NewComponent = new ChildComponent();
		NewComponent->SetOwner(this);
		Components.push_back((Component*)NewComponent);
		return NewComponent;
	}
	template <class ChildComponent> ChildComponent* GetComponent()
	{
		for (Component* Component : Components)
		{
			if (typeid(Component) == typeid(ChildComponent)) //TO-DO test
			{
				return (ChildComponent*)Component;
			}
		}
		return nullptr;
	}

	void Start();
	void Update();
	void Clear();

	Actor* GetParent() const;
	
	void SetRelativeTransform(const Transform& NewRelativeTransform);
	Transform GetRelativeTransform() const;
	void SetWorldTransform(const Transform& NewWorldTransform);
	Transform GetWorldTransform() const;

private:

	Transform * RelativeTransform = nullptr;

	Actor* Parent = nullptr;
	std::list<Actor*> ChildrenActor;
	std::list<Component*> Components;

	void AddChildActor(Actor* ChildToAdd);
	void RemoveChildActor(Actor* ChildToRemove);
};
