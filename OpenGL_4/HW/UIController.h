#pragma once

#include "../Header/Angel.h"
#include "Button.h"
#include <vector>

using namespace std;


class UIController 
{
private:
	vec2 pt;
	vector<Button*> buttons;


public:

	UIController();
	virtual ~UIController();

	void AddElement(Button* button);
	void Draw();
	void Update(float delta);  
	//void OnButtomPressed(Button* button);
	void Destroy();

};
