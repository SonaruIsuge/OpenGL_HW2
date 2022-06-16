
#include "UIController.h"


UIController::UIController() 
{

}


UIController::~UIController() 
{

}


void Init() 
{

}


void UIController::AddElement(Button* button) 
{
	buttons.push_back(button);
}


void UIController::Draw() 
{
	if (buttons.empty()) return;

	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->Draw();
	}
}


void UIController::Update(float delta) 
{
	if (buttons.empty()) return;

	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->Update(delta);
		if (buttons[i]->isTouched()) buttons[i]->Tick();
	}
}


void UIController::Destroy() 
{	
	
}

//void OnButtomPressed(C2DSprite* buttom)
//{
//
//}