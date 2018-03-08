#include <Core/Entities/Actor.hpp>
#include <Core/Transform.hpp>
#include <Core/Components/Component.hpp>
#include <algorithm>

Actor::Actor() 
{
	RelativeTransform = new Transform();
}

Actor::~Actor() 
{
	for (Component* Component : Components)
	{
		delete Component;
	}
	Components.clear();

	DetachFromParent();

	for (Actor* ChildActor : ChildrenActor)
	{
		delete ChildActor;
	}
	ChildrenActor.clear();

	delete RelativeTransform;
}

void Actor::AttachToActor(Actor * ActorTarget)
{
	DetachFromParent();
	if (ActorTarget)
	{
		Parent = ActorTarget;
		ActorTarget->AddChildActor(this);
	}
}

void Actor::DetachFromParent()
{
	if (Parent)
	{
		Parent->RemoveChildActor(this);
		Parent = nullptr;
	}
}


void Actor::AddChildActor(Actor* ChildActor)
{
	if (ChildActor)
	{
		ChildrenActor.push_back(ChildActor);
	}
}

void Actor::RemoveChildActor(Actor* ChildActor)
{
	if (ChildActor)
	{
		ChildrenActor.remove(ChildActor);
	}
}
/*
template <class ChildComponent> ChildComponent* Actor::AddComponent()
{
	Component* NewComponent = new ChildComponent(this);
	ChildrenActor.push_back(NewComponent);
	return NewComponent;
}

template<class ChildComponent> ChildComponent* Actor::GetComponent()
{
	for (Component* Component : Components)
	{
		if (typeid(Component) == typeid(ChildComponent)) //TO-DO test
		{
			return Component;
		}
	}
	return nullptr;
}
*/
void Actor::Start()
{
	for (Component* Component : Components)
	{
		if (Component->IsEnabled())
		{
			Component->Start();
		}
	}

	for (Actor* Actor : ChildrenActor)
	{
		Actor->Start();
	}
}

void Actor::Update()
{
	for (Component* Component : Components)
	{
		if (Component->IsEnabled())
		{
			Component->Update();
		}
	}

	for (Actor* Actor : ChildrenActor)
	{
		Actor->Update();
	}
}

void Actor::Clear()
{
	for (Component* Component : Components)
	{
		if (Component->IsEnabled())
		{
			Component->Clear();
		}
	}

	for (Actor* Actor : ChildrenActor)
	{
		Actor->Clear();
	}
}

Actor* Actor::GetParent() const { return Parent; }

void Actor::SetRelativeTransform(const Transform& NewRelativeTransform)
{
	*RelativeTransform = NewRelativeTransform;
}

Transform Actor::GetRelativeTransform() const { return *RelativeTransform; }

void Actor::SetWorldTransform(const Transform & NewWorldTransform)
{
	if (Parent)
	{
		*RelativeTransform = Transform::Combine(NewWorldTransform, Transform::Negate(Parent->GetWorldTransform()));
	}
	else
	{
		*RelativeTransform = NewWorldTransform;
	}
}

Transform Actor::GetWorldTransform() const
{
	if (Parent)
	{
		return Transform::Combine(Parent->GetWorldTransform(), *RelativeTransform);
	}
	return *RelativeTransform;
}
