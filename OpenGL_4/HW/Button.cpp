
#include "Button.h"
#include "InputManager.h"

Button::Button(C2DSprite* sprite)
{
	this->sprite = sprite;
	transform = new Transform;

	pushed = false;
	activeTouch = true;
}


Button::~Button() 
{
	
}


void Button::Init(vec2 pos, float rot, vec2 scale, vec4 defaultColor) 
{	
	mat4 viewMat, projMat;		// Use orthographic
	this->transform->position = vec3(pos, -1);
	this->transform->rotation = vec3(0, 0, rot);
	this->transform->scale = vec3(scale, 1);

	this->sprite->setShadingMode(ShadingMode::UI);
	this->sprite->setDefaultColor(defaultColor);
	this->sprite->setShader();
	this->sprite->setViewMatrix(viewMat);
	this->sprite->setProjectionMatrix(projMat);
}


void Button::Draw() 
{
	this->sprite->draw();
}


void Button::Update(float delta) 
{	
	this->sprite->setTRSMatrix(this->transform->GetTRSMatrix());
	this->sprite->update(delta);

	//Detect touch
	if (!activeTouch && !InputManager::getInstance()->isMouseClick(0)) {
		activeTouch = true;
	}
	
}


void Button::Destroy() 
{
	delete this->sprite;
	delete this->transform;
}


void Button::UpdateActiveTouch() 
{
	auto input = InputManager::getInstance();
	if (input->isMouseClick(0)) {
		activeTouch = false;
	}
	if (!activeTouch && !input->isMouseClick(0)) {
		activeTouch = true;
	}
}


vec2 Button::ScreenToUICoordinate(vec2 point)
{
	vec2 result;
	result.x = 2.0f * (float)point.x / SCREEN_SIZE - 1.0f;
	result.y = 2.0f * (float)(SCREEN_SIZE - point.y) / SCREEN_SIZE - 1.0f;
	return result;
}


void Button::Register(void (*callback) ()) 
{
	Tick = callback;
}


bool Button::isTouched() 
{
	auto input = InputManager::getInstance();
	if (!input->isMouseClick(0) || !activeTouch) return false;

	vec2 mousePoint(input->getMouseX(), input->getMouseY());
	vec2 uiPoint = ScreenToUICoordinate(mousePoint);
	// ¸Ó«ö¶s³Q«ö¨ì
	if (sprite->_LL.x <= uiPoint.x && uiPoint.x <= sprite->_TR.x && sprite->_LL.y <= uiPoint.y && uiPoint.y <= sprite->_TR.y)
	{
		pushed = !pushed;
		sprite->setColor(pushed ? vec4(0.5, 0.5, 0.5, 1.0f) : sprite->_DefaultColor);
		activeTouch = false;
		return true;
	}

	return false;
}