#include <Core/Entities/GameObject.hpp>
#include <algorithm>

GameObject::GameObject(){}
GameObject::GameObject(Transform const &transform) {this->transform = transform;}


void GameObject::AddChild(GameObject &childObject){
	children.push_back(&childObject);
	if(childObject.parent) childObject.parent->children.remove(&childObject);
	childObject.parent = this;
}

void GameObject::RemoveChild(GameObject &childObject){
	children.remove(&childObject);
	childObject.parent = NULL;
}

void GameObject::AddComponent(Component &component){
	component.gameObject = this;
	components.push_back(&component);
}

void GameObject::Start(){
	for(Component* component : components)
		if(component->Enabled)
			component->Start();

	for(GameObject* gameObject : children)
		gameObject->Start();
}
void GameObject::Update(){
	
	for(Component* component : components)
		if(component->Enabled)
			component->Update();

	for(GameObject* gameObject : children)
		gameObject->Update();
		
}
void GameObject::Clear(){
	for(Component* component : components)
		component->Clear();

	for(GameObject* gameObject : children)
		gameObject->Clear();
}

void GameObject::Destroy(){
	destroyed = true;
}
bool GameObject::IsDestroyed(){
	return destroyed;
}

Transform GameObject::GetWorldTransform(){
	if(parent == NULL) return transform;
	return Transform::Combine(parent->GetWorldTransform(), transform);
}