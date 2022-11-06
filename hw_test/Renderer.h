#ifndef EULER_RENDERER_H
#define EULER_RENDERER_H


#include"Shader.h"
#include"SolidProcess.h"
#include"poly2tri/poly2tri.h"


#include<vector>
#include<iostream>

using namespace std;
/*
��solid��ÿ���涼���ǻ����������ǻ��Ķ������triangles�У��Ա�main�����н��л���
*/
class Renderer
{
public:
	Solid* solid;
	struct Triangle {
		glm::vec3 v[3];
	};
	vector<Triangle> triangles;

public:
	Renderer();
	Renderer(Solid* solid);

	void GenerateTris();	//��solid��ÿ���涼���ǻ�
	void Polygon2Tri(Face* face);	//��face���ǻ�
};



#endif

