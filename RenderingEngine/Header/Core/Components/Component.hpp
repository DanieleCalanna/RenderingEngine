#pragma once

class Actor;

class Component 
{
	friend class Actor;

private:
	Actor* Owner;
	bool bEnabled = true;

	void SetOwner(Actor* const& NewOwner);

protected:
	Component(Actor* Owner);
	Component() {};
	~Component();

public:

	Actor* GetOwner() const;

	bool IsEnabled() const;
	void SetEnabled(bool bNewEnabled);

	virtual void Start() {}
	virtual void Update() {}
	virtual void Clear() {}
};