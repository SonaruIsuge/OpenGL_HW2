#pragma once

#include "Transform.h"
#include "../Common/C2DSprite.h"

#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 

//typedef void (*callback) ();


class Button 
{
private:
	bool pushed;
	bool activeTouch;

	vec2 ScreenToUICoordinate(vec2 point);

public:
	Transform* transform;
	C2DSprite* sprite;

	void (*Tick) ();

	Button(C2DSprite* sprite);
	virtual ~Button();

	void Init(vec2 pos, float rot, vec2 scale, vec4 defaultColor);
	void Draw();
	void Update(float delta);
	void Destroy();
	void UpdateActiveTouch();
	bool getButtonStatus() { return pushed; }
	bool isTouched();
	void Register(void (*callback)(void));
};