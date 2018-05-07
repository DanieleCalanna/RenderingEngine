#include "Core/Components/Component.hpp"
#include "Core/Entities/Actor.hpp"

void Component::SetOwner(Actor * const& NewOwner)
{
	if (Owner)
	{
		//Owner->RemoveComponent();
	}
	Owner = NewOwner;
}

Component::Component(std::string Name) { SetName(Name); }

Component::~Component() {}

Actor* Component::GetOwner() const { return Owner; }

bool Component::IsEnabled() const { return bEnabled; }

void Component::SetEnabled(bool bNewEnabled) { bEnabled = bNewEnabled; }

void Component::SetName(std::string NewName) { Name = NewName; }

std::string Component::GetName() const { return Name; }
