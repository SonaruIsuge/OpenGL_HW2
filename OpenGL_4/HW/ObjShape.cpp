#include "ObjShape.h"



ObjShape::ObjShape(ShapeData data)
{
	_iNumVtx = data.vtxNum;
	_pPoints = NULL; _pNormals = NULL; _pTex = NULL;

	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];
	_pTex = new vec2[_iNumVtx];	

	
	for (int i = 0; i < _iNumVtx; i++) { 
		_pPoints[i] = data.points[i];
		_pNormals[i] = data.normals[i];
		_pTex[i] = data.texs[i];
		//_pColors[i] = vec4(-1.0f, -1.0f, -1.0f, 1.0f); 
	}

	// 設定材質
	//setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//setKaKdKsShini(0, 0.8f, 0.2f, 1);
}


void ObjShape::draw()
{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Change to wireframe mode
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Return to solid mode
}

void ObjShape::drawW()
{
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
}

// 此處所給的 vLightPos 必須是世界座標的確定絕對位置
void ObjShape::update(float dt, point4 vLightPos, color4 vLightI)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(vLightPos, vLightI);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(vLightPos, vLightI);
	else {
		_vLightInView = _mxView * vLightPos;	// 將 Light 轉換到鏡頭座標再傳入 shader
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_AmbientProduct = _Material.ka * _Material.ambient * vLightI;
		_DiffuseProduct = _Material.kd * _Material.diffuse * vLightI;
		_SpecularProduct = _Material.ks * _Material.specular * vLightI;
	}
}

void ObjShape::update(float dt, const LightSource& Lights)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(Lights);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(Lights);
	else {
		_vLightInView = _mxView * Lights.position;		// 將 Light 轉換到鏡頭座標再傳入 shader
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_AmbientProduct = _Material.ka * _Material.ambient * Lights.ambient;
		_DiffuseProduct = _Material.kd * _Material.diffuse * Lights.diffuse;
		_SpecularProduct = _Material.ks * _Material.specular * Lights.specular;
	}
}

// 呼叫沒有給光源的 update 代表該物件不會進行光源照明的計算
void ObjShape::update(float dt)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算
}