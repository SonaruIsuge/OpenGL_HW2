
#include "Object.h"


Object::Object(CShape* shape) 
{
	this->shape = shape;
	this->transform = new Transform;
}


Object::~Object() {}


void Object::Init(vec3 pos, vec3 rot, vec3 scale, ShadingMode mode)
{
	this->transform->position = pos;
	this->transform->rotation = rot;
	this->transform->scale = scale;

	this->shape->setShadingMode(mode);
	this->shape->setShader();
}


void Object::Draw() 
{
	this->shape->draw();
}


void Object::Update(float delta, LightSource light) 
{
	this->shape->setTRSMatrix(this->transform->GetTRSMatrix());
	this->shape->update(delta, light);
}


void Object::Update(float delta) 
{
	this->shape->setTRSMatrix(this->transform->GetTRSMatrix());
	this->shape->update(delta);
}


void Object::Destroy()
{
	delete this->shape;
	delete this->transform;
}