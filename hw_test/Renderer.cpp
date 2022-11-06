#include "Renderer.h"

class Plane2D {
public:
	glm::vec3 o;	//坐标系的原点和三个轴的单位向量
	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;
	Plane2D(Face* face);
	glm::vec2 Space3DToPlane2D(const glm::vec3 p);
	glm::vec3 Plane2DToSpace3D(const glm::vec2 p);
};
inline Plane2D::Plane2D(Face* face) 
{
	HalfEdge* he = face->floops->lhedge;
	HalfEdge* head = he;
	glm::vec3 v1 = he->hestartv->pos;
	he = he->next;
	glm::vec3 v2 = he->hestartv->pos;
	he = he->next;
	this->o = v1;
	this->x = glm::normalize(v2 - v1);
	while (he != head) {
		glm::vec3 v3 = he->hestartv->pos;
		if (glm::length(glm::cross(v3 - v1, v3 - v2)) > 0.001f) {
			this->z = glm::normalize(glm::cross(x, v3 - v2));
			this->y = glm::normalize(glm::cross(z, x));
			break;
		}
	}
}
inline glm::vec2 Plane2D:: Space3DToPlane2D(const glm::vec3 p)
{
	if (glm::dot(p - o, z) > 0.001f)
		cout << "ERROR: p is not on the plane" << endl;
	glm::vec3 vec = p - o;
	return glm::vec2(glm::dot(vec, x), glm::dot(vec, y));

}
inline glm::vec3 Plane2D:: Plane2DToSpace3D(const glm::vec2 p) {
	return o + p.x * x + p.y * y;
}
Renderer::Renderer()
{
	solid = NULL;
}

Renderer::Renderer(Solid* solid)
{
	this->solid = solid;
}

void Renderer::GenerateTris()
{
	Face* face = solid->sfaces;
	do {
		face->setInnerLoop();
		Polygon2Tri(face);
		face = face->nextf;
	} while (face!=NULL&&face != solid->sfaces);
}

void Renderer::Polygon2Tri(Face* face)
{
	Plane2D plane(face);
	vector<p2t::Point*> allPoints;
	vector<p2t::Point*> outPoints;
	vector<p2t::Point*> inPoints;

	Loop* outlp = face->GetOuterLoop();		//处理外环
	if (outlp == NULL)
		cout << "ERROR: the face has no outerloop" << endl;
	HalfEdge* he = outlp->lhedge;
	do {
		glm::vec3 temp = he->hestartv->pos;
		glm::vec2 p = plane.Space3DToPlane2D(temp);
		p2t::Point* point = new p2t::Point(p.x, p.y);
		allPoints.push_back(point);
		outPoints.push_back(point);
		he = he->next;
	} while (he != outlp->lhedge);
	p2t::CDT cdt(move(outPoints));
	
	//处理内环
	for (Loop* loop = face->floops; loop != NULL; loop = loop->nextl) {
		if (loop->is_inner_loop) {		
			HalfEdge* he = loop->lhedge;
			inPoints.clear();
			do {
				glm::vec3 temp = he->hestartv->pos;
				glm::vec2 p = plane.Space3DToPlane2D(temp);
				p2t::Point* point = new p2t::Point(p.x, p.y);
				allPoints.push_back(point);
				inPoints.push_back(point);
				he = he->next;
			} while (he != loop->lhedge);
			cdt.AddHole(move(inPoints));
		}
	}

	cdt.Triangulate();
	auto tris = cdt.GetTriangles();
	for (auto tri : tris) {
		Triangle triangle;
		for (int i = 0; i < 3; i++) {
			auto temp = tri->GetPoint(i);
			triangle.v[i] = plane.Plane2DToSpace3D(glm::vec2(temp->x, temp->y));
		}
		triangles.push_back(triangle);
	}
	for (auto p : allPoints)
		delete p;

}
