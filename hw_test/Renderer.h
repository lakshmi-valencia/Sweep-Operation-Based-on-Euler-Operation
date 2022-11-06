#ifndef EULER_RENDERER_H
#define EULER_RENDERER_H


#include"Shader.h"
#include"SolidProcess.h"
#include"poly2tri/poly2tri.h"


#include<vector>
#include<iostream>

using namespace std;
/*
把solid的每个面都三角化，并把三角化的顶点存在triangles中，以便main函数中进行绘制
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

	void GenerateTris();	//把solid的每个面都三角化
	void Polygon2Tri(Face* face);	//把face三角化
};



#endif

