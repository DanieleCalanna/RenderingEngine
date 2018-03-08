#include "Core/Components/Component.hpp"

void Component::SetOwner(Actor * const& NewOwner)
{
	Owner = NewOwner;
}

Component::Component(Actor * Owner)
{
	this->Owner = Owner;
}

Component::~Component() {}

Actor* Component::GetOwner() const { return Owner; }

bool Component::IsEnabled() const { return bEnabled; }

void Component::SetEnabled(bool bNewEnabled) { bEnabled = bNewEnabled; }
