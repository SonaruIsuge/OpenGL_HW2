
#include "Application.h"
#include "../Common/TypeDefine.h"
#include "../Common/CQuad.h"
#include "../Common/CSolidCube.h"
#include "../Common/CSolidSphere.h"
#include "../Common/CWireSphere.h"
#include "../Common/CWireCube.h"
#include "../Common/CChecker.h"
#include "../Common/CCamera.h"
#include "../Common/CShaderPool.h"
#include "../Common/C2DSprite.h"
#include "ObjLoader.h"
#include "ObjShape.h"
#include "Object.h"
#include "Button.h"
#include "UIController.h"


// For Objects
CChecker* g_pChecker;
Object* g_pLight;
Object* LeftWall, * RightWall;
Object* FrontWall, * BackWall;
Object* tank;
Object* plain;
Object* ame;
Object* ship;
UIController* uiControl;

// For View Point
GLfloat g_fRadius = 10.0;
GLfloat g_fTheta = 60.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;


// For single light source
float g_fElapsedTime = 0;
float g_fLightRadius = 6;
float g_fLightTheta = 0;

float g_fLightR = 0.95f;
float g_fLightG = 0.95f;
float g_fLightB = 0.95f;

LightSource g_Light1 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(g_fLightRadius, g_fLightRadius, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0	,	// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
	LightType::OMNI_LIGHT
};

bool g_bAutoRotating = false;
bool g_bColorOn = false;

// 函式原型宣告
void SetRoom();
void SetUI();

Application::Application(){}
Application::~Application(){}

void Test1() { printf("1\n"); }
void Test2() { printf("2\n"); }
void Test3() { printf("3\n"); }
void Test4() { printf("4\n"); }

// ---------------------------------------------
// main function
void Application::Start() 
{
	point4  eye(0, 2, 8, 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	vec3 pos = vec3();
	vec3 rot = vec3();
	vec3 scale = vec3(1);

	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);

	SetRoom();

	pos = vec3(0, 0.01f, -8);
	rot = vec3();
	scale = vec3(0.015f);
	tank = new Object( new ObjShape(ObjLoader::Instance()->GetObjData("HW/Tank.obj")) );
	tank->Init(pos, rot, scale, ShadingMode::PHONG_SHADING);

	ObjLoader::Instance()->ClearData();

	pos = vec3(0.0f, 0.01f, 0.0f);
	rot = vec3();
	scale = vec3(0.01f);
	ame = new Object( new ObjShape(ObjLoader::Instance()->GetObjData("HW/Ame.obj")) );
	ame->Init(pos, rot, scale, ShadingMode::PHONG_SHADING);

	ObjLoader::Instance()->ClearData();

	pos = vec3(7, 1.5f, 4);
	rot = vec3(0, 90, 0);
	scale = vec3(0.5f);
	plain = new Object(new ObjShape(ObjLoader::Instance()->GetObjData("HW/Plain.obj")));
	plain->Init(pos, rot, scale, ShadingMode::PHONG_SHADING);

	ObjLoader::Instance()->ClearData();

	pos = vec3(-7, 1.0f, 4);
	rot = vec3(0, 90, 0);
	scale = vec3(0.05);
	ship = new Object(new ObjShape(ObjLoader::Instance()->GetObjData("HW/Ship.obj")));
	ship->Init(pos, rot, scale, ShadingMode::PHONG_SHADING);

	ObjLoader::Instance()->ClearData();

	// 設定 代表 Light 的 WireSpherew
	pos = vec3(g_Light1.position.x, g_Light1.position.y, g_Light1.position.z);
	rot = vec3();
	scale = vec3(1);
	g_pLight = new Object(new CWireSphere(0.25f, 6, 3));
	g_pLight->Init(pos, rot, scale);
	g_pLight->shape->setColor(g_Light1.diffuse);
	g_pLight->shape->setLightingDisable();

	SetUI();


	// Projection Matrix 設定
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	g_pChecker->setProjectionMatrix(mpx);
	g_pLight->shape->setProjectionMatrix(mpx);
	LeftWall->shape->setProjectionMatrix(mpx);
	RightWall->shape->setProjectionMatrix(mpx);
	FrontWall->shape->setProjectionMatrix(mpx);
	BackWall->shape->setProjectionMatrix(mpx);
	tank->shape->setProjectionMatrix(mpx);
	ame->shape->setProjectionMatrix(mpx);
	plain->shape->setProjectionMatrix(mpx);
	ship->shape->setProjectionMatrix(mpx);

}


void Application::Draw() 
{
	g_pChecker->draw();
	g_pLight->Draw();
	LeftWall->Draw();
	RightWall->Draw();
	FrontWall->Draw();
	BackWall->Draw();
	tank->Draw();
	ame->Draw();
	plain->Draw();
	ship->Draw();
	uiControl->Draw();
}


void updateLightPosition(float dt)
{
	mat4 mxT;
	// 每秒繞 Y 軸轉 90 度
	g_fElapsedTime += dt;
	g_fLightTheta = g_fElapsedTime * (float)M_PI_2;
	if (g_fLightTheta >= (float)M_PI * 2.0f) {
		g_fLightTheta -= (float)M_PI * 2.0f;
		g_fElapsedTime -= 4.0f;
	}
	g_Light1.position.x = g_fLightRadius * cosf(g_fLightTheta);
	g_Light1.position.z = g_fLightRadius * sinf(g_fLightTheta);
	g_pLight->transform->position.x = g_Light1.position.x;
	g_pLight->transform->position.z = g_Light1.position.z;
}


void updateCamera() 
{
	auto camera = CCamera::getInstance();
	g_fPhi = (float)-M_PI * (InputManager::getInstance()->getMouseX() - HALF_SIZE) / (HALF_SIZE);  // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI * (float)InputManager::getInstance()->getMouseY() / SCREEN_SIZE;  // up: 0 , down: PI
	point4  eye(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);	
	camera->updateViewLookAt(eye, at);
}


void Application::Update(float delta) 
{
	//updateCamera();

	mat4 mvx;		// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();
	camera->UpdateLookByMouse();
	camera->Move(delta);

	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		g_pChecker->setViewMatrix(mvx);
		g_pLight->shape->setViewMatrix(mvx);
		LeftWall->shape->setViewMatrix(mvx);
		RightWall->shape->setViewMatrix(mvx);
		FrontWall->shape->setViewMatrix(mvx);
		BackWall->shape->setViewMatrix(mvx);		
		tank->shape->setViewMatrix(mvx);
		ame->shape->setViewMatrix(mvx);
		plain->shape->setViewMatrix(mvx);
		ship->shape->setViewMatrix(mvx);
	}

	// Part 2 : 重新計算 Light 的位置
	if (g_bAutoRotating) updateLightPosition(delta);
	
	// 如果需要重新計算時，在這邊計算每一個物件的顏色
	g_pChecker->update(delta, g_Light1);
	g_pLight->Update(delta);
	LeftWall->Update(delta, g_Light1);
	RightWall->Update(delta, g_Light1);
	FrontWall->Update(delta, g_Light1);
	BackWall->Update(delta, g_Light1);
	tank->Update(delta, g_Light1);
	ame->Update(delta, g_Light1);
	plain->Update(delta, g_Light1);
	ship->Update(delta, g_Light1);
	uiControl->Update(delta);	
}



void Application::Destroy()
{
	delete g_pChecker;
	uiControl->Destroy();
	g_pLight->Destroy();
	LeftWall->Destroy();
	RightWall->Destroy();
	FrontWall->Destroy();
	BackWall->Destroy();
	tank->Destroy();
	ame->Destroy();
	plain->Destroy();
	ship->Destroy();
	CCamera::getInstance()->destroyInstance();
	CShaderPool::getInstance()->destroyInstance();
	InputManager::getInstance()->destroy();
}


// ---------------------------------------------
//handle input 
void Application::OnKeyboardHit(unsigned char key, int x, int y) 
{
	InputManager::getInstance()->onKeyPressed(key);

	if (InputManager::getInstance()->isKeyClick(SPACE_KEY) )
	{
		if (g_bColorOn) { // 目前有顏色，切換成灰階
			g_pChecker->setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
			g_pChecker->setKaKdKsShini(0, 0.8f, 0.2f, 1);
			//cube->shape->setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
			//cube->shape->setKaKdKsShini(0, 0.8f, 0.2f, 1);
			//sphere->shape->setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
			//sphere->shape->setKaKdKsShini(0, 0.8f, 0.2f, 1);
		}
		else { // 目前為灰階，切換成有顏色
			g_pChecker->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
			g_pChecker->setKaKdKsShini(0, 0.8f, 0.5f, 1);
			//cube->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
			//cube->shape->setKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
			//sphere->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
			//sphere->shape->setKaKdKsShini(0.15f, 0.45f, 0.55f, 5);
		}
		g_bColorOn = !g_bColorOn;
	}

	if(InputManager::getInstance()->isKeyClick(KEYCODE_A) || InputManager::getInstance()->isKeyClick(KEYCODE_a))
	{
		g_bAutoRotating = !g_bAutoRotating;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_R)) 
	{
		if (g_fLightR <= 0.95f) g_fLightR += 0.05f;		
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_r)) 
	{
		if (g_fLightR >= 0.05f) g_fLightR -= 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_G)) 
	{
		if (g_fLightG <= 0.95f) g_fLightG += 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_g)) 
	{
		if (g_fLightG >= 0.05f) g_fLightG -= 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_B)) 
	{
		if (g_fLightB <= 0.95f) g_fLightB += 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_b)) 
	{
		if (g_fLightB >= 0.05f) g_fLightB -= 0.05f;
	}
	
	g_Light1.diffuse = vec4(g_fLightR, g_fLightG, g_fLightB, 1.0f);
	g_pLight->shape->setColor(g_Light1.diffuse);

	if (InputManager::getInstance()->isKeyClick(ESCAPE_KEY)) 
	{
		Destroy();
	}
}


void Application::OnKeyboardRelease(unsigned char key, int x, int y) 
{
	InputManager::getInstance()->ReleaseKey(key);
}



void Application::OnMouseClick(int button, int state, int x, int y) 
{
	InputManager::getInstance()->onMouseClick(button, state);
}


void Application::OnMouseMoveWithoutClick(int x, int y) 
{
	InputManager::getInstance()->onMouseMove(x, y);
}


void Application::OnMouseMoveWithClick(int x, int y)
{
	InputManager::getInstance()->onMouseMove(x, y);
}



// 函式實作
void SetRoom() 
{
	vec3 pos, rot, scale;

	g_pChecker = new CChecker(GRID_SIZE);
	g_pChecker->setShadingMode(ShadingMode::PHONG_SHADING);
	g_pChecker->setShader();

	pos = vec3(-16, 16, 0);
	rot = vec3(0, 0, -90);
	scale = vec3(32, 1, 32);
	LeftWall = new Object(new CQuad);
	LeftWall->shape->setColor(vec4(0.6f));
	LeftWall->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	LeftWall->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	LeftWall->Init(pos, rot, scale, ShadingMode::PHONG_SHADING);

	pos = vec3(16, 16, 0);
	rot = vec3(0, 0, 90);
	scale = vec3(32, 1, 32);
	RightWall = new Object(new CQuad);
	RightWall->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	RightWall->shape->setColor(vec4(0.6f));
	RightWall->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	RightWall->Init(pos, rot, scale, ShadingMode::PHONG_SHADING);

	pos = vec3(0, 16, 16);
	rot = vec3(-90, 0, 0);
	scale = vec3(32, 1, 32);
	FrontWall = new Object(new CQuad);
	FrontWall->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	FrontWall->shape->setColor(vec4(0.6f));
	FrontWall->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	FrontWall->Init(pos, rot, scale, ShadingMode::PHONG_SHADING);

	pos = vec3(0, 16, -16);
	rot = vec3(90, 0, 0);
	scale = vec3(32, 1, 32);
	BackWall = new Object(new CQuad);
	BackWall->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	BackWall->shape->setColor(vec4(0.6f));
	BackWall->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	BackWall->Init(pos, rot, scale, ShadingMode::PHONG_SHADING);
}


void SetUI() 
{
	uiControl = new UIController;

	Button* button;
	button = new Button(new C2DSprite);
	button->Init(vec2(-0.75f, -0.90f), 0, vec2(0.1f, 0.1f), vec4(1, 0, 0, 1));
	button->sprite->setLightingDisable();
	button->Register(Test1);
	uiControl->AddElement(button);
	
	button = new Button(new C2DSprite);
	button->Init(vec2(-0.25f, -0.90f), 0, vec2(0.1f, 0.1f), vec4(1, 0, 0, 1));
	button->sprite->setLightingDisable();
	button->Register(Test2);
	uiControl->AddElement(button);

	button = new Button(new C2DSprite);
	button->Init(vec2(0.25f, -0.90f), 0, vec2(0.1f, 0.1f), vec4(1, 0, 0, 1));
	button->sprite->setLightingDisable();
	button->Register(Test3);
	uiControl->AddElement(button);

	button = new Button(new C2DSprite);
	button->Init(vec2(0.75f, -0.90f), 0, vec2(0.1f, 0.1f), vec4(1, 0, 0, 1));
	button->sprite->setLightingDisable();
	button->Register(Test4);
	uiControl->AddElement(button);
}