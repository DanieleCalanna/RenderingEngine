#pragma once

#include <vector>

class Transform;
class Component;

class Actor
{
	friend class Actor;

public:

	Actor(std::string ActorName);
	~Actor();

	virtual void AttachToActor(Actor* ActorTarget);
	virtual void DetachFromParent();

	#define AddComponentM(CompClass, ...){CompClass* NewComponent = new CompClass(##__VA_ARGS__); Components.push_back((Component*)NewComponent);}

	Component* AddComponent(Component* NewComponent);
	template <class ChildComponent> Component* RemoveComponent(bool bDelete = false)
	{
		for (int Index = Components.size()-1; Index >= 0; Index--)
		{
			Component* CurrentComponent = Components[Index];
			if (!CurrentComponent)
			{
				Components.erase(Components.begin() + Index);
				continue;
			}
			if (typeid(*CurrentComponent) == typeid(ChildComponent))
			{
				Components.erase(Components.begin() + Index);
				if (bDelete)
				{
					delete CurrentComponent;
					return nullptr;
				}
				return CurrentComponent;
			}
		}
		return nullptr;
	}

	template <class ChildComponent> ChildComponent* AddComponent(std::string Name)
	{
		ChildComponent* NewComponent = new ChildComponent();
		NewComponent->SetOwner(this);
		NewComponent->SetName(Name);
		NewComponent->Construct();
		Components.push_back((Component*)NewComponent);
		return NewComponent;
	}
	template <class ChildComponent> ChildComponent* GetComponent()
	{
		for (Component* Component : Components)
		{
			if (typeid(*Component) == typeid(ChildComponent))
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
	
	virtual void SetRelativeTransform(const Transform& NewRelativeTransform);
	Transform GetRelativeTransform() const;
	virtual void SetWorldTransform(const Transform& NewWorldTransform);
	Transform GetWorldTransform() const;

	std::string GetName() const;

private:

	std::string Name;

	Transform * RelativeTransform = nullptr;

	Actor* Parent = nullptr;
	std::vector<Actor*> ChildrenActor;
	std::vector<Component*> Components;

	void AddChildActor(Actor* ChildToAdd);
	void RemoveChildActor(Actor* ChildToRemove);
};
