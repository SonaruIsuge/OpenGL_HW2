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

	// �]�w����
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

// ���B�ҵ��� vLightPos �����O�@�ɮy�Ъ��T�w�����m
void ObjShape::update(float dt, point4 vLightPos, color4 vLightI)
{
	updateMatrix(); // �o��@�w�n���A�i��x�}����s�A�A�i������C��p��

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(vLightPos, vLightI);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(vLightPos, vLightI);
	else {
		_vLightInView = _mxView * vLightPos;	// �N Light �ഫ�����Y�y�ЦA�ǤJ shader
		// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
		_AmbientProduct = _Material.ka * _Material.ambient * vLightI;
		_DiffuseProduct = _Material.kd * _Material.diffuse * vLightI;
		_SpecularProduct = _Material.ks * _Material.specular * vLightI;
	}
}

void ObjShape::update(float dt, const LightSource& Lights)
{
	updateMatrix(); // �o��@�w�n���A�i��x�}����s�A�A�i������C��p��

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(Lights);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(Lights);
	else {
		_vLightInView = _mxView * Lights.position;		// �N Light �ഫ�����Y�y�ЦA�ǤJ shader
		// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
		_AmbientProduct = _Material.ka * _Material.ambient * Lights.ambient;
		_DiffuseProduct = _Material.kd * _Material.diffuse * Lights.diffuse;
		_SpecularProduct = _Material.ks * _Material.specular * Lights.specular;
	}
}

// �I�s�S���������� update �N��Ӫ��󤣷|�i������ө����p��
void ObjShape::update(float dt)
{
	updateMatrix(); // �o��@�w�n���A�i��x�}����s�A�A�i������C��p��
}