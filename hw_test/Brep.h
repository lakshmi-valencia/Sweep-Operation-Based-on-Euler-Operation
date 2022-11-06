#ifndef BREP
#define BREP

#include<cstdlib>
#include<iostream>
#include<glm/glm.hpp>

using namespace std;

// Half Edge Boundary Representation Class Defination

class Solid;
class Face;
class Loop;
class HalfEdge;
class Vertex;
class Edge;

static int out = 0;

class Solid
{
public:
	
	int solidno;					
	static int total_solidnum;		

	Solid* nexts;	// ��һ��ʵ��
	Solid* prevs;	// ��һ��ʵ��

	Vertex* sverts; // ʵ�������ж���
	Edge* sedges; // ʵ�����������
	Face* sfaces; // ʵ��������
	//Loop* sloops; // ʵ�����л�

	
	int numvertex;
	int numedge;
	int numface;
	//int numloop;

	
	Solid();	
	virtual ~Solid();	

	Vertex* findVertex(glm::vec3 pos);		//�ҵ���Ӧλ�õĵ�

	bool deleteVertex(Vertex* v);			// ɾȥ��
	bool deleteEdge(Edge* eg);				// ɾȥ�����
	bool deleteFace(Face* face);			// ɾȥ��
	//bool deleteLoop(Loop* lp);				// ɾȥ��

	bool addVertexToSolid(Vertex* v);		// ��Ӷ���
	bool addEdgeToSolid(Edge* eg);			// ��������
	bool addFaceToSolid(Face* face);		// �����
	// bool addLoopToSolid(Loop* lp);			// ��ӻ�

};

class Face
{
public:

	int faceno;					// ���
	static int total_facenum;	// �ܹ��м�����

	Solid* fsolid; // �����ڵ�ʵ��
	Loop* floops;  // ��������loop��ɵ������ͷ�ڵ�
	Face* nextf;   // ��һ����
	Face* prevf;   // ǰһ����


	Face();		
	~Face();	

	int Size();			//��ѯface��loop�ĸ���
	void setInnerLoop();	//�����ڻ�
	Loop* GetOuterLoop();	//�õ���һ���⻷
	bool addLoopToFace(Loop* lp);	// ���loop

};

class Loop
{
public:

	int loopno;					// loop�����
	static int total_loopnum;	// �ܹ������˼�����

	Face* lface; // loop���ڵ���
	HalfEdge* lhedge; //  loop�а����ɵ������ͷ�ڵ�
	Loop* nextl;	// ��һ����
	Loop* prevl;	// ��һ����

	bool is_inner_loop;	//����Ƿ�Ϊ�ڻ�


	Loop();	
	~Loop(); 

	bool addHalfEdgeToLoop(HalfEdge* he);	// ��Ӱ�ߣ������ӳɹ�������true
	bool isVertexInLoop(Vertex* v);			// �ж�vertex�Ƿ���loop��
	HalfEdge* findHalfEdge(Vertex* v, HalfEdge* he = NULL);	// ��he��ʼѰ����vΪ���İ�� 

};

class Edge
{
public:
	
	int edgeno;					// edge���
	static int total_edgenum;	// �ܹ������˼��������

	HalfEdge* he1; // ����ߵİ��1
	HalfEdge* he2; // ����ߵİ��2
	Edge* nexte;   // ��һ�������
	Edge* preve;   // ��һ�������

	
	Edge();		
	~Edge();    

};

class HalfEdge
{
public:

	int halfedgeno;					// ������
	static int total_halfegnum;		// �ܹ��������������

	Edge* edg;		// ������ڵ������
	Vertex* hestartv;	// ��ߵ����
	Loop* wloop;    // ������ڵ�loop
	HalfEdge* adjacent; // ָ��ͬһ����ߵİ��
	HalfEdge* next;		// ��һ�����
	HalfEdge* prev;		// ��һ�����


	HalfEdge();			
	~HalfEdge();		


};

class Vertex
{
public:

	int vertexno;					// vertex ���
	static int total_vertexnum;		// ��̬��������¼�ܹ������˼���vertex

	Vertex* nextv;	// ��һ��vertex
	Vertex* prevv;  // ��һ��vertex
	glm::vec3 pos;	//��ά�ռ�λ��

	Vertex();
	Vertex(glm::vec3 pos);	
};


#endif // !BREP

