#pragma once

#include <string>

class Actor;

class Component 
{
	friend class Actor;

private:
	std::string Name;

	Actor* Owner;
	bool bEnabled = true;

	void SetOwner(Actor* const& NewOwner);

protected:
	Component(std::string Name);
	~Component();

public:

	Actor* GetOwner() const;

	bool IsEnabled() const;
	void SetEnabled(bool bNewEnabled);

	virtual void Construct() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void Clear() {}

	void SetName(std::string NewName);
	std::string GetName() const;
};