#pragma once

#include "Transform.h"
#include "../Common/CShape.h"


class Object 
{
protected:


private:


public:
	Transform* transform;
	CShape* shape;

	Object(CShape* shape);
	virtual ~Object();

	void Init(vec3 pos, vec3 rot, vec3 scale, ShadingMode mode = ShadingMode::FLAT_SHADING_CPU);
	void Draw();
	void Update(float delta, LightSource light);
	void Update(float delta);
	void Destroy();
};