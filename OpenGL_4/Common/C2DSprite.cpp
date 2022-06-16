#include "C2DSprite.h"

C2DSprite::C2DSprite()
{
	_iNumVtx = QUAD_NUM;
	_pPoints = NULL; _pNormals = NULL; _pTex = NULL; _pColors = NULL;

	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];

	// 此處改成正面為面向正 Z 方向 
	_pPoints[0] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	_pPoints[1] = vec4( 0.5f, -0.5f, 0.0f, 1.0f);
	_pPoints[2] = vec4( 0.5f,  0.5f, 0.0f, 1.0f);
	_pPoints[3] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	_pPoints[4] = vec4( 0.5f,  0.5f, 0.0f, 1.0f);
	_pPoints[5] = vec4(-0.5f,  0.5f, 0.0f, 1.0f);

	_pNormals[0] = vec3(0, 0, 1.0f);  // Normal Vector 的 W 為 0
	_pNormals[1] = vec3(0, 0, 1.0f);
	_pNormals[2] = vec3(0, 0, 1.0f);
	_pNormals[3] = vec3(0, 0, 1.0f);
	_pNormals[4] = vec3(0, 0, 1.0f);
	_pNormals[5] = vec3(0, 0, 1.0f);

	_pColors[0] = vec4(1.0f, 1.0f, 1.0f, 1.0f);  // (r, g, b, a)
	_pColors[1] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_pColors[2] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_pColors[3] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_pColors[4] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_pColors[5] = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	_OriginalLL = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	_OriginalTR = vec4( 0.5f, 0.5f, 0.0f, 1.0f);
	//_bPushed = false;

	// 設定材質
	setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	setKaKdKsShini(0, 0.8f, 0.2f, 1);

	// 預設為 -1.0f, 由 Normal 決定顏色
	_fColor[0] = -1.0f; _fColor[1] = -1.0f; _fColor[2] = -1.0f; _fColor[3] = 1;	

	// set shader's name
	//setShaderName("vsVtxNormal.glsl", "fsVtxNormal.glsl");
	// Create and initialize a buffer object 
	//createBufferObject();
}


C2DSprite::~C2DSprite()
{
	updateMatrix();
}


void C2DSprite::update(float dt)
{
	updateMatrix();
}


void C2DSprite::update(float dt, point4 vLightPos, color4 vLightI)
{
	update(dt);
}


void C2DSprite::update(float dt, const LightSource& Lights)
{
	update(dt);
}


void C2DSprite::setTRSMatrix(mat4 &mat)
{
	_mxTRS = mat;
	_bTRSUpdated = true;
	// 計算 Sprite 在世界座標的左下角與右上角的位置
	_LL = _mxTRS*_OriginalLL;
	_TR = _mxTRS*_OriginalTR;
}


// 傳入的點已經是對應到 (-1,-1) (1,1) 之間的 2D 座標
//bool C2DSprite::onTouches(const vec2 &tp)
//{
//	if (_LL.x <= tp.x && tp.x <= _TR.x && _LL.y <= tp.y && tp.y <= _TR.y) // 該按鈕被按到
//	{
//		if ( !_bPushed ) {
//			// 變成灰階
//			setColor(vec4(0.5, 0.5, 0.5, 1.0f));
//			_bPushed = true;
//		}
//		else {
//			_bPushed = false;
//			setColor(_DefaultColor);
//		}
//		return true;		// 傳回有點到按鈕
//	}
//	else return false;	// 傳回沒有點到按鈕
//}


void C2DSprite::setDefaultColor(vec4 vColor)
{
	_DefaultColor = vColor;
	setColor(vColor);
}


void C2DSprite::draw()
{
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM);
}


void C2DSprite::drawW()
{
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM);
}
